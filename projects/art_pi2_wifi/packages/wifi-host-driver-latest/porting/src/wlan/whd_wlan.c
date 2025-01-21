/*
 * MIT License
 *
 * Copyright (c) 2024 Evlers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Change Logs:
 * Date         Author      Notes
 * 2023-12-28   Evlers      first implementation
 * 2024-03-25   Evlers      add configure the mmcsd card scanning wait time
 * 2024-03-25   Evlers      add the function to initialize WiFi using threads
 * 2024-05-27   Evlers      fix the thread initialization bug and deleted the initialization return value
 * 2024-05-28   Evlers      add support for pin names
 * 2024-07-01   Evlers      add the function to automatically probe sdio cards
 */

#include "rtthread.h"
#include "rtdevice.h"

#include "cybsp.h"
#include "cyhal_sdio.h"

#include "whd.h"
#include "whd_int.h"
#include "whd_wifi_api.h"
#include "whd_buffer_api.h"
#include "whd_resource_api.h"
#include "whd_network_types.h"
#include "wiced_resource.h"

#define DBG_TAG           "whd.wlan"
#define DBG_LVL           DBG_INFO
#include "rtdbg.h"


/* WHD interface */
static whd_driver_t whd_driver;
static cyhal_sdio_t cyhal_sdio;
static whd_init_config_t whd_config =
{
    .thread_priority = CY_WIFI_WHD_THREAD_PRIORITY,
    .thread_stack_size = CY_WIFI_WHD_THREAD_STACK_SIZE,
    .country = WHD_COUNTRY_CHINA,
};
extern whd_resource_source_t resource_ops;
extern struct whd_buffer_funcs whd_buffer_ops;


struct whd_scan
{
    struct rt_wlan_device *wlan;
    whd_scan_result_t scan_result;
    whd_scan_type_t whd_scan_type;
    rt_sem_t active_sem;
};

struct drv_wifi
{
    struct rt_wlan_device *wlan;
    whd_interface_t whd_itf;
};

static struct drv_wifi wifi_sta, wifi_ap;


/* rt-thread wlan interface */

rt_inline struct drv_wifi *get_drv_wifi(struct rt_wlan_device *wlan)
{
    if (wlan == wifi_sta.wlan)
    {
        return &wifi_sta;
    }

    if (wlan == wifi_ap.wlan)
    {
        return &wifi_ap;
    }

    return RT_NULL;
}

static rt_err_t drv_wlan_init(struct rt_wlan_device *wlan)
{
    return RT_EOK;
}

static rt_err_t drv_wlan_mode(struct rt_wlan_device *wlan, rt_wlan_mode_t mode)
{
    return RT_EOK;
}

static void whd_scan_callback(whd_scan_result_t **result_ptr, void *user_data, whd_scan_status_t status)
{
    struct whd_scan *whd_scan = user_data;
    struct rt_wlan_device *wlan = whd_scan->wlan;

    /* Check if we don't have a scan result to send to the user */
    if (( result_ptr == NULL ) || ( *result_ptr == NULL ))
    {
        /* Check for scan complete */
        if (status == WHD_SCAN_COMPLETED_SUCCESSFULLY || status == WHD_SCAN_ABORTED)
        {
            if (whd_scan->whd_scan_type == WHD_SCAN_TYPE_ACTIVE)
            {
                /* The active type scan is complete, and the wlan is now signaled to perform a passive type scan */
                whd_scan->whd_scan_type = WHD_SCAN_TYPE_PASSIVE;
                memset(&whd_scan->scan_result, 0, sizeof(whd_scan->scan_result));
                rt_sem_release(whd_scan->active_sem);
            }
            else
            {
                /* The passive type scanning is complete. The scanning is complete */
                LOG_D("scan complete!");
                rt_wlan_dev_indicate_event_handle(wlan, RT_WLAN_DEV_EVT_SCAN_DONE, RT_NULL);
                rt_sem_delete(whd_scan->active_sem);
                rt_free(whd_scan);
            }
        }
        return;
    }

    /* Copy info to wlan */
    whd_scan_result_t *bss_info = *result_ptr;
    struct rt_wlan_info wlan_info;
    struct rt_wlan_buff buff;

    rt_memset(&wlan_info, 0, sizeof(wlan_info));

    rt_memcpy(&wlan_info.bssid[0], bss_info->BSSID.octet, sizeof(wlan_info.bssid));
    rt_memcpy(wlan_info.ssid.val, bss_info->SSID.value, bss_info->SSID.length);
    wlan_info.ssid.len = bss_info->SSID.length;
    if (bss_info->SSID.length)
        wlan_info.hidden = 0;
    else
        wlan_info.hidden = 1;

    wlan_info.channel = (rt_int16_t)bss_info->channel;
    wlan_info.rssi = bss_info->signal_strength;

    wlan_info.datarate = bss_info->max_data_rate * 1000;

    switch (bss_info->band)
    {
        case WHD_802_11_BAND_5GHZ:      wlan_info.band = RT_802_11_BAND_5GHZ; break;
        case WHD_802_11_BAND_2_4GHZ:    wlan_info.band = RT_802_11_BAND_2_4GHZ; break;
        case WHD_802_11_BAND_6GHZ:      wlan_info.band = RT_802_11_BAND_UNKNOWN; break;
    }

    wlan_info.security = (rt_wlan_security_t)bss_info->security;

    bss_info ++;

    buff.data = &wlan_info;
    buff.len = sizeof(wlan_info);

    rt_wlan_dev_indicate_event_handle(wlan, RT_WLAN_DEV_EVT_SCAN_REPORT, &buff);

    memset(*result_ptr, 0, sizeof(whd_scan_result_t));
}

static rt_err_t drv_wlan_scan(struct rt_wlan_device *wlan, struct rt_scan_info *scan_info)
{
    struct whd_scan *whd_scan = rt_malloc(sizeof(struct whd_scan));

    /* Let the module scan with the active type first, and then scan the nearby ap with the passive type */

    RT_ASSERT(whd_scan != NULL);
    whd_scan->wlan = wlan;
    whd_scan->whd_scan_type = WHD_SCAN_TYPE_ACTIVE;
    whd_scan->active_sem = rt_sem_create("whd scan", 0, RT_IPC_FLAG_PRIO);
    RT_ASSERT(whd_scan->active_sem != NULL);

    /* Execute an active type scan */
    if (whd_wifi_scan(get_drv_wifi(wlan)->whd_itf, whd_scan->whd_scan_type, WHD_BSS_TYPE_ANY,
                            NULL, NULL, NULL, NULL, whd_scan_callback, &whd_scan->scan_result, whd_scan) != WHD_SUCCESS)
    {
        return RT_EOK;
    }

    /* Wait until the active scan is complete */
    rt_sem_take(whd_scan->active_sem, rt_tick_from_millisecond(10000));

    /* Execute an active type scan */
    return whd_wifi_scan(get_drv_wifi(wlan)->whd_itf, whd_scan->whd_scan_type, WHD_BSS_TYPE_ANY,
                            NULL, NULL, NULL, NULL, whd_scan_callback, &whd_scan->scan_result, whd_scan);
}

static rt_err_t drv_wlan_join(struct rt_wlan_device *wlan, struct rt_sta_info *sta_info)
{
    rt_err_t ret;
    whd_ssid_t whd_ssid = { .length = sta_info->ssid.len };

    memcpy(whd_ssid.value, sta_info->ssid.val, whd_ssid.length);

    ret = whd_wifi_join(get_drv_wifi(wlan)->whd_itf, &whd_ssid, WHD_SECURITY_WPA2_AES_PSK,
                            (const uint8_t *)sta_info->key.val, sta_info->key.len);

    if (ret != WHD_SUCCESS)
    {
        rt_wlan_dev_indicate_event_handle(wlan, RT_WLAN_DEV_EVT_CONNECT_FAIL, RT_NULL);
    }

    return ret;
}

static rt_err_t drv_wlan_softap(struct rt_wlan_device *wlan, struct rt_ap_info *ap_info)
{
    rt_err_t ret;

    ret = whd_wifi_init_ap(get_drv_wifi(wlan)->whd_itf,
                            (whd_ssid_t*)&ap_info->ssid,
                            (whd_security_t)ap_info->security,
                            (const uint8_t *) ap_info->key.val,
                            ap_info->key.len, ap_info->channel);

    if (ret != WHD_SUCCESS)
    {
        return ret;
    }

    if ((ret = whd_wifi_start_ap(get_drv_wifi(wlan)->whd_itf)) == WHD_SUCCESS)
    {
        rt_wlan_dev_indicate_event_handle(wifi_ap.wlan, RT_WLAN_DEV_EVT_AP_START, 0);
    }
    else
    {
        rt_wlan_dev_indicate_event_handle(wifi_ap.wlan, RT_WLAN_DEV_EVT_AP_STOP, 0);
    }

    return ret;
}

static rt_err_t drv_wlan_disconnect(struct rt_wlan_device *wlan)
{
    rt_err_t ret = whd_wifi_leave(get_drv_wifi(wlan)->whd_itf);

    if (ret == WHD_SUCCESS)
    {
        rt_wlan_dev_indicate_event_handle(wlan, RT_WLAN_DEV_EVT_DISCONNECT, RT_NULL);
    }
    return ret;
}

static rt_err_t drv_wlan_ap_stop(struct rt_wlan_device *wlan)
{
    if (whd_wifi_stop_ap(get_drv_wifi(wlan)->whd_itf) == WHD_SUCCESS)
    {
        rt_wlan_dev_indicate_event_handle(wifi_ap.wlan, RT_WLAN_DEV_EVT_AP_STOP, 0);
    }
    return RT_EOK;
}

static rt_err_t drv_wlan_ap_deauth(struct rt_wlan_device *wlan, rt_uint8_t mac[])
{
    whd_mac_t _mac;

    memcpy(&_mac, mac, sizeof(whd_mac_t));
    return whd_wifi_deauth_sta(get_drv_wifi(wlan)->whd_itf, &_mac, WHD_DOT11_RC_RESERVED);
}

static rt_err_t drv_wlan_scan_stop(struct rt_wlan_device *wlan)
{
    whd_wifi_stop_scan(get_drv_wifi(wlan)->whd_itf);
    return RT_EOK;
}

static int drv_wlan_get_rssi(struct rt_wlan_device *wlan)
{
    int32_t rssi;

    whd_wifi_get_rssi(get_drv_wifi(wlan)->whd_itf, &rssi);

    return rssi;
}

static rt_err_t drv_wlan_set_powersave(struct rt_wlan_device *wlan, int level)
{
    if (level)
    {
        return whd_wifi_enable_powersave(get_drv_wifi(wlan)->whd_itf);
    }
    else
    {
        return whd_wifi_disable_powersave(get_drv_wifi(wlan)->whd_itf);
    }
}

static int drv_wlan_get_powersave(struct rt_wlan_device *wlan)
{
    uint32_t value = 0;

    whd_wifi_get_powersave_mode(get_drv_wifi(wlan)->whd_itf, &value);

    return value;
}

static rt_err_t drv_wlan_set_channel(struct rt_wlan_device *wlan, int channel)
{
    return whd_wifi_set_channel(get_drv_wifi(wlan)->whd_itf, channel);
}

static int drv_wlan_get_channel(struct rt_wlan_device *wlan)
{
    uint32_t channel = 0;

    whd_wifi_get_channel(get_drv_wifi(wlan)->whd_itf, &channel);

    return channel;
}

static rt_country_code_t drv_wlan_get_country(struct rt_wlan_device *wlan)
{
    if (whd_config.country == WHD_COUNTRY_CHINA)
    {
        return RT_COUNTRY_CHINA;
    }
    else
    {
        return RT_COUNTRY_UNKNOWN;
    }
}

static rt_err_t drv_wlan_set_mac(struct rt_wlan_device *wlan, rt_uint8_t mac[])
{
    whd_mac_t whd_mac;

    memcpy(whd_mac.octet, mac, sizeof(whd_mac.octet));

    return whd_wifi_set_mac_address(get_drv_wifi(wlan)->whd_itf, whd_mac);
}

static rt_err_t drv_wlan_get_mac(struct rt_wlan_device *wlan, rt_uint8_t mac[])
{
    rt_err_t ret;
    whd_mac_t whd_mac;

    if (wlan == wifi_ap.wlan)
    {
        if ((ret = whd_wifi_get_mac_address(wifi_sta.whd_itf, &whd_mac)) == WHD_SUCCESS)
        {
            /* AP interface needs to come up with MAC different from STA  */
            if (whd_mac.octet[0] & 0x02)
            {
                whd_mac.octet[0] &= (uint8_t) ~(0x02);
            }
            else
            {
                whd_mac.octet[0] |= 0x02;
            }
        }
    }
    else
    {
        ret = whd_wifi_get_mac_address(get_drv_wifi(wlan)->whd_itf, &whd_mac);
    }

    if (ret == WHD_SUCCESS)
    {
        memcpy(mac, whd_mac.octet, RT_WLAN_BSSID_MAX_LENGTH);
    }

    return ret;
}

static int drv_wlan_recv(struct rt_wlan_device *wlan, void *buff, int len)
{
    return RT_EOK;
}

static int drv_wlan_send(struct rt_wlan_device *wlan, void *buff, int len)
{
    whd_buffer_t buffer;

    /* Determines if a particular interface is ready to transceive ethernet packets */
    if (whd_wifi_is_ready_to_transceive(get_drv_wifi(wlan)->whd_itf) != WHD_SUCCESS)
    {
        return -RT_ERROR;
    }

    /* Creating a transport layer pbuf allows WHD to add link data to the pbuf header */
    if (whd_host_buffer_get(get_drv_wifi(wlan)->whd_itf->whd_driver, &buffer, WHD_NETWORK_TX, len + WHD_LINK_HEADER, 1000) != 0)
    {
        LOG_D("err whd_host_buffer_get failed\n");
        return -RT_ERROR;
    }

    /* Sets the offset of buffer data position
     * It is reserved for whd to write link header data
     */
    whd_buffer_add_remove_at_front(get_drv_wifi(wlan)->whd_itf->whd_driver, &buffer, WHD_LINK_HEADER);

    /* Copy network data to WHD */
    memcpy(whd_buffer_get_current_piece_data_pointer(get_drv_wifi(wlan)->whd_itf->whd_driver, buffer), buff, len);

    /*
     *  This function takes ethernet data from the network stack and queues it for transmission over the wireless network.
     *  The function can be called from any thread context as it is thread safe, however
     *  it must not be called from interrupt context since it might get blocked while waiting
     *  for a lock on the transmit queue.
     */
    whd_network_send_ethernet_data(get_drv_wifi(wlan)->whd_itf, buffer);

    return RT_EOK;
}

static const struct rt_wlan_dev_ops ops =
{
    .wlan_init = drv_wlan_init,
    .wlan_mode = drv_wlan_mode,
    .wlan_scan = drv_wlan_scan,
    .wlan_join = drv_wlan_join,
    .wlan_softap = drv_wlan_softap,
    .wlan_disconnect = drv_wlan_disconnect,
    .wlan_ap_stop = drv_wlan_ap_stop,
    .wlan_ap_deauth = drv_wlan_ap_deauth,
    .wlan_scan_stop = drv_wlan_scan_stop,
    .wlan_get_rssi = drv_wlan_get_rssi,
    .wlan_set_powersave = drv_wlan_set_powersave,
    .wlan_get_powersave = drv_wlan_get_powersave,
    .wlan_cfg_promisc = NULL,
    .wlan_cfg_filter = NULL,
    .wlan_cfg_mgnt_filter = NULL,
    .wlan_set_channel = drv_wlan_set_channel,
    .wlan_get_channel = drv_wlan_get_channel,
    .wlan_set_country = NULL,
    .wlan_get_country = drv_wlan_get_country,
    .wlan_set_mac = drv_wlan_set_mac,
    .wlan_get_mac = drv_wlan_get_mac,
    .wlan_recv = drv_wlan_recv,
    .wlan_send = drv_wlan_send,
};

/*
 * This function takes packets from the radio driver and passes them into the
 * lwIP stack. If the stack is not initialized, or if the lwIP stack does not
 * accept the packet, the packet is freed (dropped). If the packet is of type EAPOL
 * and if the Extensible Authentication Protocol over LAN (EAPOL) handler is registered, the packet will be redirected to the registered
 * handler and should be freed by the EAPOL handler.
 */
void cy_network_process_ethernet_data(whd_interface_t iface, whd_buffer_t buf)
{
    uint16_t ethertype;
    uint8_t *data = whd_buffer_get_current_piece_data_pointer(iface->whd_driver, buf);
    struct rt_wlan_device *wlan = wifi_ap.whd_itf == iface ? wifi_ap.wlan : wifi_sta.wlan;

    #define EAPOL_PACKET_TYPE                        (0x888E)

    if (iface->role == WHD_STA_ROLE)
    {

    }
    else if (iface->role == WHD_AP_ROLE)
    {

    }
    else
    {
        whd_buffer_release(iface->whd_driver, buf, WHD_NETWORK_RX);
        return;
    }

    ethertype = (uint16_t) (data[12] << 8 | data[13]);
    if (ethertype == EAPOL_PACKET_TYPE)
    {
        LOG_D("EAPOL_PACKET_TYPE");
    }
    else
    {
        /* If the interface is not yet set up, drop the packet */
        LOG_D("Send data up to wlan");
        rt_wlan_dev_report_data(wlan, data, whd_buffer_get_current_piece_size(iface->whd_driver, buf));
        whd_buffer_release(iface->whd_driver, buf, WHD_NETWORK_RX);
    }
}

static whd_netif_funcs_t netif_if_ops =
{
    .whd_network_process_ethernet_data = cy_network_process_ethernet_data,
};

/**
 * Event handler prototype definition
 *
 * @param[out] whd_event_header_t : whd event header
 * @param[out] uint8_t*           : event data
 * @param[out] handler_user_data  : semaphore data
 */
void *whd_event_handler(whd_interface_t ifp, const whd_event_header_t *event_header, const uint8_t *event_data, void *user_data)
{
    struct drv_wifi *wifi = (struct drv_wifi *)user_data;

    LOG_D("whd_event_handler, event_type: %u", event_header->event_type);

    switch (event_header->event_type)
    {
        case WLC_E_ASSOC:
        case WLC_E_REASSOC:
            LOG_D("Connected.");
            rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_CONNECT, RT_NULL);
        break;

        case WLC_E_LINK:
            if (event_header->reason != 0)
            {
                LOG_D("Disconnected, reason: %u", event_header->reason);
                rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_DISCONNECT, RT_NULL);
            }
        break;
    }

    return user_data;
}

static void register_whd_events (void)
{
    uint16_t event_index = 0;
    uint32_t event_list[] = { WLC_E_ASSOC, WLC_E_REASSOC, WLC_E_LINK, WLC_E_NONE };
    if (whd_wifi_set_event_handler(wifi_sta.whd_itf, event_list, whd_event_handler, &wifi_sta, &event_index) != WHD_SUCCESS)
    {
        LOG_W("Failed to register the whd event");
    }
}

static void whd_init_thread (void *parameter)
{
    static struct rt_wlan_device wlan_ap, wlan_sta;
    whd_oob_config_t oob_config =
    {
#ifndef CYBSP_USING_PIN_NAME
        .host_oob_pin      = CYBSP_HOST_WAKE_IRQ_PIN,
#else
        .host_oob_pin      = rt_pin_get(CYBSP_HOST_WAKE_IRQ_PIN_NAME),
#endif
        .dev_gpio_sel      = 0,
        .is_falling_edge   = (CYBSP_HOST_WAKE_IRQ_EVENT == CYHAL_GPIO_IRQ_FALL) ? WHD_TRUE : WHD_FALSE,
        .intr_priority     = CYBSP_OOB_INTR_PRIORITY
    };
    whd_sdio_config_t whd_sdio_config =
    {
        .sdio_1bit_mode        = WHD_FALSE,
        .high_speed_sdio_clock = WHD_FALSE,
        .oob_config            = oob_config
    };

    wifi_ap.wlan = &wlan_ap;
    wifi_sta.wlan = &wlan_sta;


    /* Creates a semaphore to wait probe the sdio card */
    cyhal_sdio.probe = rt_sem_create("sdio probe", 0, RT_IPC_FLAG_PRIO);
    RT_ASSERT(cyhal_sdio.probe != NULL);

    /* Register the sdio driver */
    if (cyhal_sdio_init(&cyhal_sdio) != CYHAL_SDIO_RET_NO_ERRORS)
    {
        LOG_E("Unable to register SDIO driver to mmcsd!");
        return;
    }

    LOG_D("Wait for sdio card registration..");

    /* Waiting card registration and delete the semaphore */
    rt_sem_take(cyhal_sdio.probe, RT_WAITING_FOREVER);
    rt_sem_delete(cyhal_sdio.probe);

    /* Initialize WiFi Host Drivers (WHD) */
    rt_kprintf("RT-Thread WiFi Host Drivers (WHD)\n");
    rt_kprintf("You can get the latest version on https://github.com/Evlers/rt-thread_wifi-host-driver\n");

    /* Initialize WiFi host drivers */
    whd_init(&whd_driver, &whd_config, &resource_ops, &whd_buffer_ops, &netif_if_ops);

    /* Attach a bus SDIO */
    if (whd_bus_sdio_attach(whd_driver, &whd_sdio_config, &cyhal_sdio) != WHD_SUCCESS)
    {
        LOG_E("Unable to Attach to the sdio bus!");
        return;
    }

#ifdef WHD_RESOURCES_IN_EXTERNAL_STORAGE_FS
    LOG_D("Wait mounting the external storage of file system..");
    extern void whd_wait_fs_mount (void);
    whd_wait_fs_mount();
#endif

    /* Switch on Wifi, download firmware and create a primary interface, returns whd_interface_t */
    if (whd_wifi_on(whd_driver, &wifi_sta.whd_itf) != WHD_SUCCESS)
    {
        LOG_E("Unable to start the WiFi module!");
        return;
    }

#ifdef CY_WIFI_DEFAULT_ENABLE_POWERSAVE_MODE
    uint32_t value = 0;
    if (whd_wifi_get_powersave_mode(wifi_sta.whd_itf, &value) != WHD_SUCCESS)
    {
        LOG_E("The powersave mode switch status cannot be get!");
        return;
    }
#endif

    /* Disables 802.11 power save mode on specified interface */
    if (whd_wifi_disable_powersave(wifi_sta.whd_itf) != WHD_SUCCESS)
    {
        LOG_E("Failed to disable the powersave mode!");
        return;
    }

#ifdef CY_WIFI_DEFAULT_ENABLE_POWERSAVE_MODE
    if (value == PM1_POWERSAVE_MODE)
    {
        /* Enables powersave mode on specified interface */
        if (whd_wifi_enable_powersave(wifi_sta.whd_itf) != WHD_SUCCESS)
        {
            LOG_E("Failed to enable the powersave mode!");
            return;
        }

        LOG_D("Enables powersave mode.");
    }
    else if (value == PM2_POWERSAVE_MODE)
    {
        /* Enables powersave mode on specified interface while attempting to maximise throughput */
        /* Note: When working in this mode, sdio communication timeout will occur, which is the normal operation of whd */
        if (whd_wifi_enable_powersave_with_throughput(wifi_sta.whd_itf, CY_WIFI_DEFAULT_PM2_SLEEP_RET_TIME) != WHD_SUCCESS)
        {
            LOG_E("Failed to enable the powersave mode!");
            return;
        }

        LOG_D("Enables powersave mode while attempting to maximise throughput.");
    }
#endif

    /* Creates a secondary interface, returns whd_interface_t */
    if (whd_add_secondary_interface(whd_driver, NULL, &wifi_ap.whd_itf) != WHD_SUCCESS)
    {
        LOG_E("Failed to create a secondary interface!");
        return;
    }

    /* Registers a handler to receive whd event callbacks. */
    register_whd_events();

    /* Register the wlan device and set its working mode */

    /* register wlan device for ap */
    if (rt_wlan_dev_register(&wlan_ap, RT_WLAN_DEVICE_AP_NAME, &ops, 0, &wifi_ap) != RT_EOK)
    {
        LOG_E("Failed to register a wlan_ap device!");
        return;
    }

    /* register wlan device for sta */
    if (rt_wlan_dev_register(&wlan_sta, RT_WLAN_DEVICE_STA_NAME, &ops, 0, &wifi_sta) != RT_EOK)
    {
        LOG_E("Failed to register a wlan_sta device!");
        return;
    }

    /* Set wlan_sta to STATION mode */
    if (rt_wlan_set_mode(RT_WLAN_DEVICE_STA_NAME, RT_WLAN_STATION) != RT_EOK)
    {
        LOG_E("Failed to set %s to station mode!", RT_WLAN_DEVICE_STA_NAME);
        return;
    }

    /* Set wlan_ap to AP mode */
    if (rt_wlan_set_mode(RT_WLAN_DEVICE_AP_NAME, RT_WLAN_AP) != RT_EOK)
    {
        LOG_E("Failed to set %s to ap mode!", RT_WLAN_DEVICE_AP_NAME);
        return;
    }
}

static int rt_hw_wifi_init (void)
{
#ifdef CY_WIFI_USING_THREAD_INIT
    rt_thread_startup(rt_thread_create("whd_init", whd_init_thread, NULL, CY_WIFI_INIT_THREAD_STACK_SIZE, 20, 10));
#else
    whd_init_thread(NULL);
#endif
    return RT_EOK;
}
INIT_ENV_EXPORT(rt_hw_wifi_init);
