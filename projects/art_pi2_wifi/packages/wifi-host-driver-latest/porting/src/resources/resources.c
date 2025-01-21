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
 */

#include <stdint.h>
#include <string.h>

#include "rtthread.h"

#ifdef WHD_RESOURCES_IN_EXTERNAL_STORAGE

#include "wiced_resource.h"
#include "whd_resource_api.h"
#include "wifi_nvram_image.h"

#define DBG_TAG           "whd.resources"
#define DBG_LVL           DBG_INFO
#include "rtdbg.h"

/* nvram uses local resources, clm and firmware use external resources */
#if defined(WHD_DYNAMIC_NVRAM)
#define NVRAM_SIZE             dynamic_nvram_size
#define NVRAM_IMAGE_VARIABLE   dynamic_nvram_image
#else
#define NVRAM_SIZE             sizeof(wifi_nvram_image)
#define NVRAM_IMAGE_VARIABLE   wifi_nvram_image
#endif

static const char *name_list[] =
{
#ifdef WHD_RESOURCES_IN_EXTERNAL_STORAGE_FAL
    [WHD_RESOURCE_WLAN_FIRMWARE] = WHD_RESOURCES_FIRMWARE_PART_NAME,
    [WHD_RESOURCE_WLAN_CLM] = WHD_RESOURCES_CLM_PART_NAME
#endif
#ifdef WHD_RESOURCES_IN_EXTERNAL_STORAGE_FS
    [WHD_RESOURCE_WLAN_FIRMWARE] = WHD_RESOURCES_FIRMWARE_PATH_NAME,
    [WHD_RESOURCE_WLAN_CLM] = WHD_RESOURCES_CLM_PATH_NAME
#endif
};

unsigned char r_buffer[WHD_RESOURCES_BLOCK_SIZE];

#ifdef WHD_RESOURCES_IN_EXTERNAL_STORAGE_FAL
#include "fal.h"

static uint32_t host_platform_resource_size (whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *size_out)
{
    resource_hnd_t resource;
    const struct fal_partition *part;
    const char *part_name = name_list[type];

    if (type == WHD_RESOURCE_WLAN_NVRAM)
    {
        *size_out = NVRAM_SIZE;
        return WHD_SUCCESS;
    }

    if ((part = fal_partition_find(part_name)) == NULL)
    {
        LOG_E("No %s partition found", part_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    if (fal_partition_read(part, 0, (uint8_t *)&resource, sizeof(resource)) != sizeof(resource))
    {
        LOG_E("Read resource size failed for partition[%s]", part_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    if (resource.location != RESOURCE_IN_EXTERNAL_STORAGE || resource.size > part->len)
    {
        LOG_E("Read resource head error for partition[%s]", part_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    *size_out = (uint32_t)resource_get_size(((const resource_hnd_t *)&resource));

    return WHD_SUCCESS;
}

static uint32_t host_get_resource_block_size (whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *size_out)
{
    *size_out = WHD_RESOURCES_BLOCK_SIZE;
    return RESOURCE_SUCCESS;
}

static uint32_t host_get_resource_no_of_blocks (whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *block_count)
{
    uint32_t resource_size;
    uint32_t block_size;

    host_platform_resource_size(whd_drv, type, &resource_size);
    host_get_resource_block_size(whd_drv, type, &block_size);
    *block_count = resource_size / block_size;
    if (resource_size % block_size)
        *block_count += 1;

    return RESOURCE_SUCCESS;
}

static uint32_t host_get_resource_block (whd_driver_t whd_drv, whd_resource_type_t type, uint32_t blockno, const uint8_t **data, uint32_t *size_out)
{
    uint32_t resource_size;
    uint32_t block_size;
    uint32_t block_count;
    uint32_t read_pos;
    resource_hnd_t resource;
    const struct fal_partition *part;
    const char *part_name = name_list[type];

    host_platform_resource_size(whd_drv, type, &resource_size);
    host_get_resource_block_size(whd_drv, type, &block_size);
    host_get_resource_no_of_blocks(whd_drv, type, &block_count);
    memset(r_buffer, 0, block_size);
    read_pos = blockno * block_size;

    if (blockno >= block_count)
    {
        return WHD_BADARG;
    }

    if (type == WHD_RESOURCE_WLAN_NVRAM)
    {
        if (NVRAM_SIZE - read_pos > block_size)
        {
            *size_out = block_size;
        }
        else
        {
            *size_out = NVRAM_SIZE - read_pos;
        }
        *data = ( (uint8_t *)NVRAM_IMAGE_VARIABLE ) + read_pos;
        return WHD_SUCCESS;
    }

    if ((part = fal_partition_find(part_name)) == NULL)
    {
        LOG_E("No %s partition found", part_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    if (fal_partition_read(part, 0, (uint8_t *)&resource, sizeof(resource)) != sizeof(resource))
    {
        LOG_E("Read block failed for partition[%s]", part_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    if (resource.location != RESOURCE_IN_EXTERNAL_STORAGE || resource.size > part->len)
    {
        LOG_E("Read resource head error for partition[%s]", part_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    if (read_pos > resource.size)
    {
        return RESOURCE_OFFSET_TOO_BIG;
    }

    *size_out = fal_partition_read(part, read_pos + sizeof(resource_hnd_t), r_buffer, MIN(block_size, resource.size - read_pos));
    *data = (uint8_t *)&r_buffer;

    return RESOURCE_SUCCESS;
}

static uint32_t host_resource_read (whd_driver_t whd_drv, whd_resource_type_t type, uint32_t offset, uint32_t size, uint32_t *size_out, void *buffer)
{
    resource_hnd_t resource;
    const struct fal_partition *part;
    const char *part_name = name_list[type];

    if (type == WHD_RESOURCE_WLAN_NVRAM)
    {
        if (size != sizeof(wifi_nvram_image) )
        {
            return WHD_BUFFER_SIZE_SET_ERROR;
        }
        memcpy( (uint8_t *)buffer, wifi_nvram_image, sizeof(wifi_nvram_image) );
        *size_out = sizeof(wifi_nvram_image);
        return WHD_SUCCESS;
    }

    if ((part = fal_partition_find(part_name)) == NULL)
    {
        LOG_E("No %s partition found", part_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    if (fal_partition_read(part, 0, (uint8_t *)&resource, sizeof(resource)) != sizeof(resource))
    {
        LOG_E("Read failed for partition[%s]", part_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    if (resource.location != RESOURCE_IN_EXTERNAL_STORAGE || resource.size > part->len)
    {
        LOG_E("Read resource head error for partition[%s]", part_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    *size_out = fal_partition_read(part, offset + sizeof(resource_hnd_t), r_buffer, MIN(size, resource.size - offset));

    return RESOURCE_SUCCESS;
}
#endif

#ifdef WHD_RESOURCES_IN_EXTERNAL_STORAGE_FS
#include <dfs_file.h>
#include <unistd.h>
#include <fcntl.h>

static uint32_t host_platform_resource_size (whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *size_out)
{
    int fd;
    struct stat stat;
    const char *path_name = name_list[type];

    if (type == WHD_RESOURCE_WLAN_NVRAM)
    {
        *size_out = NVRAM_SIZE;
        return WHD_SUCCESS;
    }

    if ((fd = open(path_name, O_RDONLY)) < 0)
    {
        LOG_E("No %s file found", path_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    if (fstat(fd, &stat) < 0)
    {
        close(fd);
        LOG_E("Read failed for file[%s]", path_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    *size_out = stat.st_size;

    close(fd);

    return WHD_SUCCESS;
}

static uint32_t host_get_resource_block_size (whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *size_out)
{
    *size_out = WHD_RESOURCES_BLOCK_SIZE;
    return RESOURCE_SUCCESS;
}

static uint32_t host_get_resource_no_of_blocks (whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *block_count)
{
    uint32_t resource_size;
    uint32_t block_size;

    host_platform_resource_size(whd_drv, type, &resource_size);
    host_get_resource_block_size(whd_drv, type, &block_size);
    *block_count = resource_size / block_size;
    if (resource_size % block_size)
        *block_count += 1;

    return RESOURCE_SUCCESS;
}

static uint32_t host_get_resource_block (whd_driver_t whd_drv, whd_resource_type_t type, uint32_t blockno, const uint8_t **data, uint32_t *size_out)
{
    int fd;
    uint32_t resource_size;
    uint32_t block_size;
    uint32_t block_count;
    uint32_t read_pos;
    const char *path_name = name_list[type];

    host_platform_resource_size(whd_drv, type, &resource_size);
    host_get_resource_block_size(whd_drv, type, &block_size);
    host_get_resource_no_of_blocks(whd_drv, type, &block_count);
    memset(r_buffer, 0, block_size);
    read_pos = blockno * block_size;

    if (blockno >= block_count)
    {
        return WHD_BADARG;
    }

    if (type == WHD_RESOURCE_WLAN_NVRAM)
    {
        if (NVRAM_SIZE - read_pos > block_size)
        {
            *size_out = block_size;
        }
        else
        {
            *size_out = NVRAM_SIZE - read_pos;
        }
        *data = ( (uint8_t *)NVRAM_IMAGE_VARIABLE ) + read_pos;
        return WHD_SUCCESS;
    }

    if ((fd = open(path_name, O_RDONLY)) < 0)
    {
        LOG_E("No %s file found", path_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    if (read_pos > resource_size)
    {
        close(fd);
        return RESOURCE_OFFSET_TOO_BIG;
    }

    lseek(fd, read_pos, SEEK_SET);
    *size_out = read(fd, r_buffer, MIN(block_size, resource_size - read_pos));
    *data = (uint8_t *)&r_buffer;

    close(fd);

    return RESOURCE_SUCCESS;
}

static uint32_t host_resource_read (whd_driver_t whd_drv, whd_resource_type_t type, uint32_t offset, uint32_t size, uint32_t *size_out, void *buffer)
{
    int fd;
    uint32_t resource_size;
    const char *path_name = name_list[type];

    host_platform_resource_size(whd_drv, type, &resource_size);

    if (type == WHD_RESOURCE_WLAN_NVRAM)
    {
        if (size != sizeof(wifi_nvram_image) )
        {
            return WHD_BUFFER_SIZE_SET_ERROR;
        }
        memcpy( (uint8_t *)buffer, wifi_nvram_image, sizeof(wifi_nvram_image) );
        *size_out = sizeof(wifi_nvram_image);
        return WHD_SUCCESS;
    }

    if ((fd = open(path_name, O_RDONLY)) < 0)
    {
        LOG_E("No %s file found", path_name);
        return RESOURCE_FILE_OPEN_FAIL;
    }

    lseek(fd, offset, SEEK_SET);
    *size_out = read(fd, r_buffer, MIN(size, resource_size - offset));

    close(fd);

    return RESOURCE_SUCCESS;
}
#endif

whd_resource_source_t resource_ops =
{
    .whd_resource_size = host_platform_resource_size,
    .whd_get_resource_block_size = host_get_resource_block_size,
    .whd_get_resource_no_of_blocks = host_get_resource_no_of_blocks,
    .whd_get_resource_block = host_get_resource_block,
    .whd_resource_read = host_resource_read
};

#endif /* WHD_RESOURCES_IN_EXTERNAL_STORAGE */
