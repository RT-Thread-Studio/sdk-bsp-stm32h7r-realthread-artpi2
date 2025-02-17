#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define I2C_SCAN_DEV_NAME "i2c3"

static void i2c_scan(void)
{
    struct rt_i2c_bus_device *bus = (struct rt_i2c_bus_device *)rt_device_find(I2C_SCAN_DEV_NAME);
    if (bus == RT_NULL) {
        rt_kprintf("can't find %s device!\n", I2C_SCAN_DEV_NAME);
    } else {
        for (int i = 0; i < 128; ++i) {
            uint8_t buf;
            struct rt_i2c_msg msgs;
            msgs.addr = i;
            msgs.flags = RT_I2C_RD;
            msgs.buf = &buf;
            msgs.len = 1;
            if (rt_i2c_transfer(bus, &msgs, 1) == 1) {
                rt_kprintf("Addr: 0x%.2x\n", i);
            }
        }
    }
}
MSH_CMD_EXPORT(i2c_scan, i2c aht10 sample);
