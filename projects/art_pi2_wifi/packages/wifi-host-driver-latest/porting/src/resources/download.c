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
 * 2024-01-27   Evlers      first implementation
 * 2024-07-01   Evlers      add md5 checksum printing for resource file downloads
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "rtthread.h"

#if defined(WHD_RESOURCES_IN_EXTERNAL_STORAGE) && defined(RT_USING_RYM)

#include "ymodem.h"
#include "fal.h"
#include "wiced_resource.h"
#include "whd_resource_api.h"

#if defined(PKG_USING_TINYCRYPT) && defined(TINY_CRYPT_MD5)
#include "tiny_md5.h"
#endif /* defined(PKG_USING_TINYCRYPT) && defined(TINY_CRYPT_MD5) */

static size_t download_file_total_size, download_file_cur_size;
static const struct fal_partition *download_part = RT_NULL;

static enum rym_code ymodem_on_begin (struct rym_ctx *ctx, rt_uint8_t *buf, rt_size_t len)
{
    char *file_name, *file_size;

    /* calculate and store file size */
    file_name = (char *)&buf[0];
    file_size = (char *)&buf[rt_strlen(file_name) + 1];
    download_file_total_size = atol(file_size);

    download_file_cur_size = 0;

    /* Check resource file size */
    if (download_file_total_size > download_part->len)
    {
        rt_kprintf("\nFile is too large! File size (%d), '%s' partition size (%d)", download_file_total_size, download_part->name, download_part->len);
        return RYM_CODE_CAN;
    }

    /* erase file section */
    if (fal_partition_erase(download_part, 0, sizeof(resource_hnd_t) + download_file_total_size) < 0)
    {
        rt_kprintf("\nFile download failed! Partition (%s) erase error!", download_part->name);
        return RYM_CODE_CAN;
    }

    return RYM_CODE_ACK;
}

static enum rym_code ymodem_on_data (struct rym_ctx *ctx, rt_uint8_t *buf, rt_size_t len)
{
    uint32_t write_length = MIN(download_file_total_size - download_file_cur_size, len);

    /* Write resource file data  */
    if (fal_partition_write(download_part, sizeof(resource_hnd_t) + download_file_cur_size, buf, write_length) < 0)
    {
        rt_kprintf("\nDownload failed! Partition (%s) write data error!", download_part->name);
        return RYM_CODE_CAN;
    }

    download_file_cur_size += write_length;

    return RYM_CODE_ACK;
}

#if defined(PKG_USING_TINYCRYPT) && defined(TINY_CRYPT_MD5)
static void print_md5_checksum (uint32_t addr, uint32_t size)
{
    uint8_t md5_value[16];
    uint32_t read_count = 0;
    uint8_t *buf = rt_malloc(WHD_RESOURCES_BLOCK_SIZE);
    tiny_md5_context *ctx = rt_malloc(sizeof(tiny_md5_context));
    RT_ASSERT(buf != NULL && ctx != NULL);

    /* Calculate the md5 checksum */
    tiny_md5_starts(ctx);
    while (read_count < size)
    {
        uint32_t length = MIN(WHD_RESOURCES_BLOCK_SIZE, size - read_count);
        if (fal_partition_read(download_part, addr + read_count, buf, length) < 0)
        {
            rt_kprintf("Failed to read while verifying file!");
            goto __error;
        }
        tiny_md5_update(ctx, buf, length);
        read_count += length;
    }
    tiny_md5_finish(ctx, md5_value);

    /* print the md5 checksum */
    rt_kprintf("MD5 value: ");
    for (uint8_t i = 0; i < sizeof(md5_value); i ++)
    {
        rt_kprintf("%02X", md5_value[i]);
    }
    rt_kprintf("\n");

    __error:
    rt_free(buf);
    rt_free(ctx);
}
#endif /* defined(PKG_USING_TINYCRYPT) && defined(TINY_CRYPT_MD5) */

static void whd_res_download (int argc, char **argv)
{
    struct rym_ctx rctx;

    if (argc < 2)
    {
        rt_kprintf("Using: whd_res_download <fal partition name>\n");
        return ;
    }

    if ((download_part = fal_partition_find(argv[1])) == NULL)
    {
        rt_kprintf("Download failed! Partition (%s) find error!\n", argv[1]);
        return ;
    }

    rt_kprintf("Please select the %s file and use Ymodem to send.", argv[1]);

    if (rym_recv_on_device(&rctx, rt_console_get_device(), RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                            ymodem_on_begin, ymodem_on_data, NULL, RT_TICK_PER_SECOND) == RT_EOK)
    {
        if (download_file_cur_size == download_file_total_size)
        {
            resource_hnd_t file_head =
            {
                .location = RESOURCE_IN_EXTERNAL_STORAGE,
                .size = download_file_cur_size,
                .val.external_storage_context = (void *)NULL
            };

            /* Write resource file head data  */
            if (fal_partition_write(download_part, 0, (const uint8_t *)&file_head, sizeof(resource_hnd_t)) < 0)
            {
                rt_kprintf("\nDownload failed! Partition (%s) write head data error!\n", download_part->name);
                return ;
            }

            rt_kprintf("\nDownload %s to flash success. file size: %u\n", download_part->name, download_file_total_size);
#if defined(PKG_USING_TINYCRYPT) && defined(TINY_CRYPT_MD5)
            print_md5_checksum(sizeof(resource_hnd_t), file_head.size);
#endif /* defined(PKG_USING_TINYCRYPT) && defined(TINY_CRYPT_MD5) */
        }
        else
        {
            rt_kprintf("\nDownload failed! receive length: %u, file length: %u.\n", download_file_cur_size, download_file_total_size);
        }
    }
    else
    {
        rt_kprintf("\nDownload %s resource file failed!\n", argv[1]);
    }
}
MSH_CMD_EXPORT(whd_res_download, Download wifi-host-driver resource files);

#endif /* defined(WHD_RESOURCES_IN_EXTERNAL_STORAGE) && defined(RT_USING_RYM) */
