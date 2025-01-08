/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-21     stackRyan    first version
 * 2022-02-17     stackRyan    migrant to rtt's drv_spi
 */
#include <board.h>
#ifdef BSP_USING_OLED_454_MIPI

#include <rtdevice.h>
#include <rthw.h>
#include "drv_ssd2828.h"

#define DBG_TAG "drv.ssd28"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define SSD2828_BUS_NAME "spi5"
#define SSD2828_DEV_NAME "spi50"

static struct rt_spi_device *ssd2828_dev;
//static uint16_t mode = LP;

static rt_err_t ssd2828_write_cmd(uint8_t cmd)
{
    if (ssd2828_dev != RT_NULL)
    {
        rt_spi_send(ssd2828_dev, &cmd, 1);
    }
    else
    {
        LOG_E("write data error");
        return -RT_ERROR;
    }
    return RT_EOK;
}

static rt_err_t ssd2828_write_data(uint16_t data)
{

    uint8_t buf[3] = {0x72, 0x00, 0xB0};
    buf[1] = (uint8_t)(data >> 8);
    buf[2] = (uint8_t)(data);

    if (ssd2828_dev != RT_NULL)
    {
        rt_spi_send(ssd2828_dev, buf, 3);
    }
    else
    {
        LOG_E("write data error");
        return -RT_ERROR;
    }
    return RT_EOK;
}

static rt_err_t ssd2828_write_reg(uint8_t cmd, uint8_t dat1, uint8_t dat2)
{
    uint8_t buf[3] = {0x70, 0x00, 0x00};
    uint16_t data = dat1 << 8 | dat2;
    buf[2] = cmd;
    if (ssd2828_dev != RT_NULL)
    {
        rt_spi_send(ssd2828_dev, buf, 3);
    }
    else
    {
        LOG_E("write data error");
        return -RT_ERROR;
    }
    ssd2828_write_data(data);
    return RT_EOK;
}

uint16_t ssd2828_read_reg(uint8_t reg)
{
    uint8_t buf[3] = {0x70, 0x00, 0x00};
    uint8_t cmd = 0x73;
    uint8_t data[2];
    uint16_t temp;
    buf[2] = reg;
    rt_spi_send(ssd2828_dev, buf, 3);
    rt_spi_send_then_recv(ssd2828_dev, &cmd, 1, data, 2);
    temp = (data[0] << 8) | data[1];
    return temp;
}

void SSD282x_send_mipi_dcs_cmd()
{
    //    0xBC,1
    //    0xBF,x
    ssd2828_write_reg(0xBC, 0x00, 0x01);
    ssd2828_write_reg(0xBF, 0x00, 0x23);
    ssd2828_write_reg(0xBC, 0x00, 0x01);
    ssd2828_write_reg(0xBF, 0x00, 0x29);
}
// MSH_CMD_EXPORT(SSD282x_send_mipi_dcs_cmd, SSD282x_send_mipi_dcs_cmd)
void ssd2828_dcs_short_write(uint8_t n)
{
    //  if (mode == LP)
    //  {
    //    ssd2828_write_reg(0x00b7, 0x02, 0x50);
    //  }
    //  else if (mode == HS)
    //  {
    //    ssd2828_write_reg(0x00b7, 0x02, 0x50 & 0XEF | 0X03);
    //  }
    //  else if (mode == VD)
    //  {
    //    ssd2828_write_reg(0x00b7, 0x02 | 0x01, 0x50 & 0XEF | 0X0B);
    //  }
    rt_thread_mdelay(10);
    ssd2828_write_reg(0xbc, 0x00, n);
    ssd2828_write_reg(0xbd, 0x00, 0x00);
    ssd2828_write_reg(0xbe, 0x00, n);
    ssd2828_write_cmd(0xbf);
}

void ssd2828_bist_lcd(void)
{
    ssd2828_write_reg(0xEE, 0x06, 0x00);
}
MSH_CMD_EXPORT(ssd2828_bist_lcd, bist test lcd)

void ssd2828_dump_reg(void)
{
    rt_kprintf("Info:REG VICR1  0xB1:%04x\n", ssd2828_read_reg(0xB1));
    rt_kprintf("Info:REG VICR2  0xB2:%04x\n", ssd2828_read_reg(0xB2));
    rt_kprintf("Info:REG VICR3  0xB3:%04x\n", ssd2828_read_reg(0xB3));
    rt_kprintf("Info:REG VICR4  0xB4:%04x\n", ssd2828_read_reg(0xB4));
    rt_kprintf("Info:REG VICR5  0xB5:%04x\n", ssd2828_read_reg(0xB5));
    rt_kprintf("Info:REG VICR6  0xB6:%04x\n", ssd2828_read_reg(0xB6));
    rt_kprintf("Info:REG CFGR   0xB7:%04x\n", ssd2828_read_reg(0xB7));
    rt_kprintf("Info:REG VC     0xB8:%04x\n", ssd2828_read_reg(0xB8));
    rt_kprintf("Info:REG PLL    0xB9:%04x\n", ssd2828_read_reg(0xB9));
    rt_kprintf("Info:REG PLLC   0xBA:%04x\n", ssd2828_read_reg(0xBA));
    rt_kprintf("Info:REG CCR    0xBB:%04x\n", ssd2828_read_reg(0xBB));
    rt_kprintf("Info:REG PSC1   0xBC:%04x\n", ssd2828_read_reg(0xBC));
    rt_kprintf("Info:REG PSC2   0xBD:%04x\n", ssd2828_read_reg(0xBD));
    rt_kprintf("Info:REG PSC3   0xBE:%04x\n", ssd2828_read_reg(0xBE));
    rt_kprintf("Info:REG ACK    0xC3:%04x\n", ssd2828_read_reg(0xC3));
    rt_kprintf("Info:REG LCR    0xC4:%04x\n", ssd2828_read_reg(0xC4));
    rt_kprintf("Info:REG ICR    0xC5:%04x\n", ssd2828_read_reg(0xC5));
    rt_kprintf("Info:REG ISR    0xC6:%04x\n", ssd2828_read_reg(0xC6));
    rt_kprintf("Info:REG ErrSR  0xC7:%04x\n", ssd2828_read_reg(0xC7));
    //    rt_kprintf("Info:REG 0xB7:%04x\n",ssd2828_read_reg(0xB7));
}
MSH_CMD_EXPORT(ssd2828_dump_reg, ssd2828_dump_reg)

/*
 * PLL configuration register settings.
 *
 * See the "PLL Configuration Register Description" in the SSD2828 datasheet.
 */
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
static uint32_t construct_pll_config(uint32_t desired_pll_freq_kbps,
                                     uint32_t reference_freq_khz)
{
    uint32_t div_factor = 1, mul_factor, fr = 0;
    uint32_t output_freq_kbps;

    /* The intermediate clock after division can't be less than 5MHz */
    while (reference_freq_khz / (div_factor + 1) >= 5000)
        div_factor++;
    if (div_factor > 31)
        div_factor = 31;

    mul_factor = DIV_ROUND_UP(desired_pll_freq_kbps * div_factor,
                              reference_freq_khz);

    output_freq_kbps = reference_freq_khz * mul_factor / div_factor;

    if (output_freq_kbps >= 501000)
        fr = 3;
    else if (output_freq_kbps >= 251000)
        fr = 2;
    else if (output_freq_kbps >= 126000)
        fr = 1;

    return (fr << 14) | (div_factor << 8) | mul_factor;
}

static uint32_t decode_pll_config(uint32_t pll_config, uint32_t reference_freq_khz)
{
    uint32_t mul_factor = pll_config & 0xFF;
    uint32_t div_factor = (pll_config >> 8) & 0x1F;
    if (mul_factor == 0)
        mul_factor = 1;
    if (div_factor == 0)
        div_factor = 1;
    return reference_freq_khz * mul_factor / div_factor;
}

static int ssd2828_configure_video_interface()
{
    uint32_t val;
    ssd2828_write_reg(SSD2828_VICR1, SSD2828_VSYNC, SSD2828_HSYNC);
    ssd2828_write_reg(SSD2828_VICR2, SSD2828_VBP, SSD2828_HBP);
    ssd2828_write_reg(SSD2828_VICR3, SSD2828_VFP, SSD2828_HFP);
    ssd2828_write_reg(SSD2828_VICR4, (SSD2828_WIDTH >> 8) & 0xff, SSD2828_WIDTH & 0xff);
    ssd2828_write_reg(SSD2828_VICR5, (SSD2828_HEIGHT >> 8) & 0xff, SSD2828_HEIGHT & 0xff);

    //    val = SSD2828_VIDEO_MODE_BURST;
    val = SSD2828_VIDEO_MODE_BURST;
    val |= SSD2828_VICR6_BLLP;
    val |= SSD2828_VIDEO_PIXEL_FORMAT_24BPP;
    ssd2828_write_reg(SSD2828_VICR6, (val >> 8) & 0xff, val & 0xff);
    //    ssd2828_write_reg(SSD2828_VICR6, 0x00, 0x27);
    //    ssd2828_write_reg(SSD2828_VICR6, 0x00, 0x03);
    ssd2828_write_reg(SSD2828_LCFR, 0x00, 0x00); // 1 LANE
    return 0;
}

/*
 * Send MIPI command to the LCD panel (cmdnum < 0xB0)
 */
static void send_mipi_dcs_command(uint8_t cmdnum)
{
    /* Set packet size to 1 (a single command with no parameters) */
    ssd2828_write_reg(SSD2828_PSCR1, 0x00, 0x01);
    /* Send the command */
    ssd2828_write_reg(SSD2828_PDR, (cmdnum >> 8) & 0xff, cmdnum & 0xff);
}

static void send_mipi_dcs_command_1_param(uint8_t cmd, uint8_t param)
{
    /* Set packet size to 1 (a single command with no parameters) */
    ssd2828_write_reg(SSD2828_PSCR1, 0x00, 0x02);
    /* Send the command and payload*/
    ssd2828_write_reg(SSD2828_PDR, param & 0xff, cmd & 0xff);
}

static void send_mipi_dcs_command_4_param(uint8_t cmd, uint8_t *param)
{
    //    if (mode == LP)
    //    {
    ssd2828_write_reg(0x00b7, 0x02, 0x50);
    //    }
    //    else if (mode == HS)
    //    {
    //    ssd2828_write_reg(0x00b7, 0x06, 0x50 & 0XEF | 0X03);
    //    }
    //    else if (mode == VD)
    //    {
    //    ssd2828_write_reg(0x00b7, 0x06 | 0x01, 0x50 & 0XEF | 0X0B);
    //    }
    rt_thread_mdelay(10);
    ssd2828_write_reg(0xbc, 0, 5);
    ssd2828_write_reg(0xbd, 0, 0);
    ssd2828_write_reg(0xbe, 0, 5);

    ssd2828_write_reg(0xbf, *(param), cmd);
    ssd2828_write_data((*(param + 2) << 8) | *(param + 1));
    ssd2828_write_data((0x00 << 8) | *(param + 3));

    //    ssd2828_write_cmd(0xbf);
    //    ssd2828_write_data(cmd);
    //    ssd2828_write_data(*param++);
    //    ssd2828_write_data(*param++);
    //    ssd2828_write_data(*param++);
    //    ssd2828_write_data(*param);

    //	/* Set packet size to 1 (a single command with no parameters) */
    //    ssd2828_write_reg(SSD2828_PSCR1, 0x00, 0x05);
    //	/* Send the command */
    //	ssd2828_write_reg(SSD2828_PDR, *param & 0xff, cmd & 0xff);
    //    rt_kprintf("param:%d\n",*param);
    //    param = param+1;
    //    ssd2828_write_reg(SSD2828_PDR, *(param+1) & 0xff, *param & 0xff);
    //    rt_kprintf("param:%d\n",*param);
    //    rt_kprintf("param:%d\n",*(param+1));
    //    param = param+2;
    //    ssd2828_write_reg(SSD2828_PDR, 0x00, *param & 0xff);
    //    rt_kprintf("param:%d\n",*param);
    //    rt_kprintf("+++++++++++++++++\n");
    // send_mipi_dcs_command(cmd);

    //	/* Set packet size to 1 (a single command with no parameters) */
    //    ssd2828_write_reg(SSD2828_PSCR1, 0x00, 0x02);
    //	/* Send the command */
    //	ssd2828_write_reg(SSD2828_PDR, *(param+1) & 0xff, *param & 0xff);
    //
    //    /* Set packet size to 1 (a single command with no parameters) */
    //    ssd2828_write_reg(SSD2828_PSCR1, 0x00, 0x02);
    //	/* Send the command */
    //	ssd2828_write_reg(SSD2828_PDR, *(param+3) & 0xff, *(param+2) & 0xff);
}


void rm69330_init()
{
    // uint32_t cfgr_reg = SSD2828_CFGR_EOT | /* EOT Packet Enable */
    //                     SSD2828_CFGR_ECD | /* Disable ECC and CRC */
    //                     SSD2828_CFGR_HS;   /* Data lanes are in HS mode */

    send_mipi_dcs_command_1_param(0xFE, 0x07);
    send_mipi_dcs_command_1_param(0x15, 0x04);
    send_mipi_dcs_command_1_param(0xFE, 0x00);
    send_mipi_dcs_command_1_param(0x35, 0x00);
    send_mipi_dcs_command_1_param(0x3A, 0x77);
    send_mipi_dcs_command_1_param(0x51, 0xff);

    //    /* Sleep out */
    //    send_mipi_dcs_command_1_param(CMD_DSI_EXIT_SLEEP_MODE, 0x00);
    send_mipi_dcs_command(CMD_DSI_EXIT_SLEEP_MODE);
    rt_thread_mdelay(120);
    {
        uint8_t InitParam1[4] = {0x00, 0x0E, 0x01, 0xD3};
        uint8_t InitParam2[4] = {0x00, 0x00, 0x01, 0xC5};

        send_mipi_dcs_command_4_param(CMD_DSI_SET_COLUMN_ADDRESS, InitParam1);
        send_mipi_dcs_command_4_param(CMD_DSI_SET_PAGE_ADDRESS, InitParam2);
    }
    //    /* Set default Brightness */
    send_mipi_dcs_command_1_param(0x51, 0xFF);
    //    send_mipi_dcs_command_1_param(0x23, 0x00);    //ALLP ON
    //    send_mipi_dcs_command(0x23);
    //    send_mipi_dcs_command_1_param(0x36, 0x40);//reflect
    //    send_mipi_dcs_command_1_param(CMD_DSI_SET_DISPLAY_ON, 0x00);
    send_mipi_dcs_command(CMD_DSI_SET_DISPLAY_ON);

    //	cfgr_reg &= ~SSD2828_CFGR_HS;  /* Data lanes are in LP mode */
    ////    cfgr_reg |= SSD2828_CFGR_HS;  /* Data lanes are in HS mode */
    //	cfgr_reg |= SSD2828_CFGR_CKE;  /* Clock lane is in HS mode */
    //	cfgr_reg |= SSD2828_CFGR_DCS;  /* Only use DCS packets */
    //    cfgr_reg |= SSD2828_CFGR_LPE;
    //    ssd2828_write_reg(SSD2828_CFGR, (cfgr_reg >> 8) & 0xff, cfgr_reg & 0xff);
    /* Set memory address MODIFIED vs ORIGINAL */
}

static rt_err_t ssd2828_spi_init()
{
    /* get spi bus device */
    rt_err_t ret;

    rt_device_t spi_bus = rt_device_find(SSD2828_BUS_NAME);
    if (spi_bus)
    {
        struct rt_spi_configuration cfg1;
        LOG_D("Find spi bus %s\n", SSD2828_BUS_NAME);
        ssd2828_dev = (struct rt_spi_device *)rt_device_find(SSD2828_DEV_NAME);

        if (ssd2828_dev == NULL)
        {
            extern rt_err_t rt_hw_spi_device_attach(const char *bus_name, const char *device_name,
                                                    GPIO_TypeDef *cs_gpiox, uint16_t cs_gpio_pin);
            ret = rt_hw_spi_device_attach(SSD2828_BUS_NAME, SSD2828_DEV_NAME, GPIOF, GPIO_PIN_6);

            ssd2828_dev = (struct rt_spi_device *)rt_device_find(SSD2828_DEV_NAME);

            if (ssd2828_dev == NULL)
            {
                LOG_E("Register SSD2828 spi device fail\n");
                return -1;
            }
        }
        ret = rt_device_open((rt_device_t)ssd2828_dev, RT_DEVICE_FLAG_RDWR);

        cfg1.data_width = 8;            // 8; //16;
        cfg1.max_hz = 12 * 1000 * 1000; // 6m
        cfg1.mode = RT_SPI_MODE_0 | RT_SPI_MSB | RT_SPI_MASTER;

        ret = rt_spi_configure(ssd2828_dev, &cfg1);
        ret = rt_spi_take_bus(ssd2828_dev);
    }
    else
    {
        LOG_E("Can not found spi bus %s, init fail\n", SSD2828_BUS_NAME);
        return -1;
    }
    return ret;
}
// MSH_CMD_EXPORT(ssd2828_spi_init, ssd2828_spi_init);

void ssd2828_read_id()
{
    uint8_t odr[3] = {0x70, 0x00, 0xB0};
    uint8_t cmd = 0x73;
    uint8_t data[2];
    rt_spi_send(ssd2828_dev, odr, 3);
    rt_spi_send_then_recv(ssd2828_dev, &cmd, 1, data, 2);
    rt_kprintf("Info:SSD2828 OK 0xB7:%x\n", data[0]);
}
MSH_CMD_EXPORT(ssd2828_read_id, ssd2828_read_id)

int ssd2828_init()
{
    uint32_t pll_config;
    uint32_t pll_freq_kbps;
    uint32_t lp_div;
    uint32_t lp_val;
    /* The LP clock speed is limited by 10MHz */
    const uint32_t mipi_dsi_low_power_clk_khz = 10000;
    /*
     * This is just the reset default value of CFGR register (0x301).
     * Because we are not always able to read back from SPI, have
     * it initialized here.
     */
    uint32_t cfgr_reg = SSD2828_CFGR_EOT | /* EOT Packet Enable */
                                           //           SSD2828_CFGR_ECD | /* Disable ECC and CRC */
                        SSD2828_CFGR_HS;   /* Data lanes are in HS mode */

    ssd2828_spi_init();
    rt_thread_mdelay(100);
    if (ssd2828_read_reg(0xB0) == 0x2828)
    {
        LOG_I("SSD2828 OK, CFGR:%04x", ssd2828_read_reg(0xB7));
    }
    else
    {
        LOG_E("SSD2828 ID read failed");
    }

    /* Setup the parallel LCD timings in the appropriate registers.*/
    ssd2828_configure_video_interface();

    /* Use 'pclk' as the reference clock for PLL */
    //    cfgr_reg |= SSD2828_CFGR_CSS;
    /* Configuration Register */
    cfgr_reg &= ~SSD2828_CFGR_HS; /* Data lanes are in LP mode */
                                  //    cfgr_reg |= SSD2828_CFGR_HS;  /* Data lanes are in HS mode */
    cfgr_reg |= SSD2828_CFGR_CKE; /* Clock lane is in HS mode */
    cfgr_reg |= SSD2828_CFGR_DCS; /* Only use DCS packets */
    ssd2828_write_reg(SSD2828_CFGR, (cfgr_reg >> 8) & 0xff, cfgr_reg & 0xff);
    /* PLL Configuration Register */
    pll_config = construct_pll_config(
        500 * 1000, // mipi_dsi_bitrate_per_data_lane_mbps
        24000);     // reference_freq_khz
    ssd2828_write_reg(SSD2828_PLCR, (pll_config >> 8) & 0xff, pll_config & 0xff);
    pll_freq_kbps = decode_pll_config(pll_config, 24000);                 // reference_freq_khz
    lp_div = DIV_ROUND_UP(pll_freq_kbps, mipi_dsi_low_power_clk_khz * 8); // mipi_dsi_low_power_clk_khz
    /* VC Control Register */
    ssd2828_write_reg(SSD2828_VCR, 0x00, 0x00);
    /* Clock Control Register */
    lp_val = SSD2828_LP_CLOCK_DIVIDER(lp_div);
    ssd2828_write_reg(SSD2828_CCR, (lp_val >> 8) & 0xff, lp_val & 0xff);
    /* PLL Control Register */
    ssd2828_write_reg(SSD2828_PCR, 0x00, 0x01);
    /* Wait for PLL lock */
    rt_thread_mdelay(100);
    rm69330_init();
    rt_thread_mdelay(100);
    cfgr_reg |= SSD2828_CFGR_HS;  /* Enable HS mode for data lanes */
    cfgr_reg |= SSD2828_CFGR_VEN; /* Enable video pipeline */
    ssd2828_write_reg(SSD2828_CFGR, (cfgr_reg >> 8) & 0xff, cfgr_reg & 0xff);
    return RT_EOK;
}
MSH_CMD_EXPORT(ssd2828_init, init mipi bridge ic);
INIT_APP_EXPORT(ssd2828_init);

void ssd2828_setreset(uint8_t t)
{
    //  SSD2828_RESET = t;
}

void ssd2828_shutdown(FunctionalState state)
{
    //  SSD2828_SHUT = (uint8_t)state & 0x01;
}

#endif /* BSP_USING_OLED_454_MIPI */
/***** END OF FILE *****/
