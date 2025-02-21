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
 * 2023-12-21   Evlers      first implementation
 * 2024-05-17   Evlers      change the block transfer size to a fixed 64 bytes
 * 2024-06-05   Evlers      fix assertion caused by no response data required in whd
 * 2024-07-01   Evlers      add the function to automatically probe sdio cards
 */

#include "cyhal_sdio.h"
#include <rtdevice.h>
#include "whd_bus_sdio_protocol.h"


static rt_int32_t wlan_probe(struct rt_mmcsd_card *card);
static rt_int32_t wlan_remove(struct rt_mmcsd_card *card);


static struct rt_sdio_device_id sdio_device_id =
{
    .func_code = BACKPLANE_FUNCTION,
    .manufacturer = CYHAL_SDIO_MANUFACTURER,
    .product = CYHAL_SDIO_DRIVER_ID,
};

static struct rt_sdio_driver sdio_driver =
{
    .name = CYHAL_SDIO_DRIVER_NAME,
    .probe = wlan_probe,
    .remove = wlan_remove,
    .id = &sdio_device_id,
};

static cyhal_sdio_t *tmp_cyhal_sdio;


/** Initialize the SDIO peripheral
 *
 * @param[out] obj               The SDIO object
 * @return The status of the init request
 */
cy_rslt_t cyhal_sdio_init(cyhal_sdio_t *obj)
{
    tmp_cyhal_sdio = obj;
    tmp_cyhal_sdio->dev_id = &sdio_device_id;

    if (sdio_register_driver(&sdio_driver) == RT_ENOMEM)
    {
        return CYHAL_SDIO_RET_NO_SP_ERRORS;
    }
    return CYHAL_SDIO_RET_NO_ERRORS;
}

/** Release the SDIO peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The SDIO object
 */
void cyhal_sdio_free(cyhal_sdio_t *obj)
{
    sdio_unregister_driver(&sdio_driver);
}

/** Configure the SDIO block.
 *
 * @param[in,out] obj    The SDIO object
 * @param[in]     config The sdio configuration to apply
 * @return The status of the configure request
 */
cy_rslt_t cyhal_sdio_configure(cyhal_sdio_t *obj, const cyhal_sdio_cfg_t *config)
{
    mmcsd_set_clock(obj->card->host, config->frequencyhal_hz);
    return sdio_set_block_size(obj->card->sdio_function[obj->dev_id->func_code], config->block_size);
}

/** Sends a command to the SDIO block.
 *
 * @param[in,out] obj       The SDIO object
 * @param[in]     direction The direction of transfer (read/write)
 * @param[in]     command   The SDIO command to send
 * @param[in]     argument  The argument to the command
 * @param[out]    response  The response from the SDIO device
 * @return The status of the configure request
 */
cy_rslt_t cyhal_sdio_send_cmd(const cyhal_sdio_t *obj, cyhal_transfer_t direction, cyhal_sdio_command_t command,
                              uint32_t argument, uint32_t *response)
{
    cy_rslt_t ret;
    (void)direction;
    struct rt_mmcsd_cmd cmd;

    if (obj->card == RT_NULL)
    {
        return CYHAL_SDIO_RET_NO_SP_ERRORS;
    }

    struct rt_mmcsd_host *host = obj->card->host;
    if (host == RT_NULL)
    {
        return CYHAL_SDIO_RET_NO_SP_ERRORS;
    }

    rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = command;
    cmd.arg = argument;
    cmd.flags = RESP_SPI_R5 | RESP_R5 | CMD_AC;

    mmcsd_host_lock(obj->card->host);
    ret = mmcsd_send_cmd(host, &cmd, 0);
    mmcsd_host_unlock(obj->card->host);
    if (ret)
        return ret;

    if (cmd.resp[0] & R5_ERROR)
        return CYHAL_SDIO_RET_CMD_IDX_ERROR;
    if (cmd.resp[0] & R5_FUNCTION_NUMBER)
        return CYHAL_SDIO_FUNC_NOT_SUPPORTED;
    if (cmd.resp[0] & R5_OUT_OF_RANGE)
        return CYHAL_SDIO_RET_RESP_FLAG_ERROR;

    if (response != NULL)
    {
        *response = cmd.resp[0];
    }

    return ret;
}

/** Performs a bulk data transfer (CMD=53) to the SDIO block.
 *
 * @param[in,out] obj       The SDIO object
 * @param[in]     direction The direction of transfer (read/write)
 * @param[in]     argument  The argument to the command
 * @param[in]     data      The data to send to the SDIO device. The data buffer
 *                          should be aligned to the block size (64 bytes) if data
 *                          size is greater that block size (64 bytes).
 * @param[in]     length    The number of bytes to send
 * @param[out]    response  The response from the SDIO device
 * @return The status of the configure request
 */
cy_rslt_t cyhal_sdio_bulk_transfer(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument,
                                   const uint32_t *data, uint16_t length, uint32_t *response)
{
    struct rt_mmcsd_req req;
    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_data mmcsd_data;
    sdio_cmd_argument_t *arg = (sdio_cmd_argument_t *)&cmd.arg;

    if (obj->card == RT_NULL)
    {
        return CYHAL_SDIO_RET_NO_SP_ERRORS;
    }

    struct rt_mmcsd_host *host = obj->card->host;
    if (host == RT_NULL)
    {
        return CYHAL_SDIO_RET_NO_SP_ERRORS;
    }

    rt_memset(&req, 0, sizeof(struct rt_mmcsd_req));
    rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
    rt_memset(&mmcsd_data, 0, sizeof(struct rt_mmcsd_data));

    req.cmd = &cmd;
    req.data = &mmcsd_data;

    cmd.retries = 1;
    cmd.cmd_code = CYHAL_SDIO_CMD_IO_RW_EXTENDED;
    cmd.arg = argument;
    cmd.flags = RESP_SPI_R5 | RESP_R5 | CMD_ADTC;

    mmcsd_data.buf = (rt_uint32_t *)data;
    mmcsd_data.flags = (direction == CYHAL_READ) ? DATA_DIR_READ : DATA_DIR_WRITE;

    if ((*arg).cmd53.block_mode)
    {
        /* Block mode */
        mmcsd_data.blksize = SDIO_64B_BLOCK; /* WHD is uniformly adopts 64-byte block transmission */
        mmcsd_data.blks = (length + mmcsd_data.blksize - 1u) / mmcsd_data.blksize;
    }
    else
    {
        /* Byte mode */
        mmcsd_data.blksize = length;
        mmcsd_data.blks = 1;
        mmcsd_data.flags |= DATA_STREAM;
    }

    mmcsd_host_lock(obj->card->host);
    mmcsd_set_data_timeout(&mmcsd_data, obj->card);
    mmcsd_send_request(host, &req);
    mmcsd_host_unlock(obj->card->host);

    if (response != NULL)
    {
        *response = cmd.resp[0];
    }

    return (req.cmd->err) ? CYHAL_SDIO_RET_NO_SP_ERRORS : CYHAL_SDIO_RET_NO_ERRORS;
}

static void *cyhal_sdio_irq_handler_arg = NULL;
static cyhal_sdio_irq_handler_t cyhal_sdio_irq_handler = NULL;

static void sdio_irq_handler(struct rt_sdio_function *func)
{
    if (cyhal_sdio_irq_handler != NULL)
    {
        cyhal_sdio_irq_handler(cyhal_sdio_irq_handler_arg, CYHAL_SDIO_CARD_INTERRUPT);
    }
}

/** The sdio interrupt handler registration
 *
 * @param[in] obj         The SDIO object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 */
void cyhal_sdio_register_irq(cyhal_sdio_t *obj, cyhal_sdio_irq_handler_t handler, void *handler_arg)
{
    cyhal_sdio_irq_handler_arg = handler_arg;
    cyhal_sdio_irq_handler = handler;
}

/** Configure sdio interrupt.
 *
 * @param[in] obj      The SDIO object
 * @param[in] event    The sdio IRQ type
 * @param[in] enable   Set to non-zero to enable events, or zero to disable them
 */
void cyhal_sdio_irq_enable(cyhal_sdio_t *obj, cyhal_sdio_irq_event_t event, bool enable)
{
    mmcsd_host_lock(obj->card->host);

    if (event == CYHAL_SDIO_CARD_INTERRUPT)
    {
        if (enable)
        {
            if (cyhal_sdio_irq_handler != RT_NULL)
            {
                sdio_attach_irq(obj->card->sdio_function[obj->dev_id->func_code], sdio_irq_handler);
            }
        }
        else
        {
            sdio_detach_irq(obj->card->sdio_function[obj->dev_id->func_code]);
        }
    }

    mmcsd_host_unlock(obj->card->host);
}

static rt_int32_t wlan_probe(struct rt_mmcsd_card *card)
{
    /* save mmcsd card */
    tmp_cyhal_sdio->card = card;

    /* release the probe semaphore */
    rt_sem_release(tmp_cyhal_sdio->probe);

    return CYHAL_SDIO_RET_NO_ERRORS;
}

static rt_int32_t wlan_remove(struct rt_mmcsd_card *card)
{
    return RT_EOK;
}
