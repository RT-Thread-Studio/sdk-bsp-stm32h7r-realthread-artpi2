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
 */

#include "whd.h"
#include "whd_network_types.h"
#include "lwip/memp.h"
#include "lwip/pbuf.h"

#define  SDIO_BLOCK_SIZE (64U)


whd_result_t cy_host_buffer_get(whd_buffer_t *buffer, whd_buffer_dir_t direction, uint16_t size,
                                uint32_t timeout_ms)
{
    UNUSED_PARAMETER(direction);
    struct pbuf *p = NULL;
    if ( (direction == WHD_NETWORK_TX) && (size <= PBUF_POOL_BUFSIZE) )
    {
        p = pbuf_alloc(PBUF_RAW, size, PBUF_POOL);
    }
    else
    {
        p = pbuf_alloc(PBUF_RAW, size + SDIO_BLOCK_SIZE, PBUF_RAM);
        if (p != NULL)
        {
            p->len = size;
            p->tot_len -=  SDIO_BLOCK_SIZE;
        }
    }
    if (p != NULL)
    {
        *buffer = p;
        return WHD_SUCCESS;
    }
    else
    {
        return WHD_BUFFER_ALLOC_FAIL;
    }
}

void cy_buffer_release(whd_buffer_t buffer, whd_buffer_dir_t direction)
{
    UNUSED_PARAMETER(direction);
    (void)pbuf_free( (struct pbuf *)buffer );
}

uint8_t *cy_buffer_get_current_piece_data_pointer(whd_buffer_t buffer)
{
    RT_ASSERT(buffer != NULL);
    struct pbuf *pbuffer = (struct pbuf *)buffer;
    return (uint8_t *)pbuffer->payload;
}

uint16_t cy_buffer_get_current_piece_size(whd_buffer_t buffer)
{
    RT_ASSERT(buffer != NULL);
    struct pbuf *pbuffer = (struct pbuf *)buffer;
    return (uint16_t)pbuffer->len;
}

whd_result_t cy_buffer_set_size(whd_buffer_t buffer, unsigned short size)
{
    RT_ASSERT(buffer != NULL);
    struct pbuf *pbuffer = (struct pbuf *)buffer;

    if (size >
        (unsigned short)WHD_LINK_MTU  + LWIP_MEM_ALIGN_SIZE(LWIP_MEM_ALIGN_SIZE(sizeof(struct pbuf) ) ) +
        LWIP_MEM_ALIGN_SIZE(size) )
    {
        return WHD_PMK_WRONG_LENGTH;
    }

    pbuffer->tot_len = size;
    pbuffer->len = size;

    return CY_RSLT_SUCCESS;
}

whd_result_t cy_buffer_add_remove_at_front(whd_buffer_t *buffer, int32_t add_remove_amount)
{
    RT_ASSERT(buffer != NULL);
    struct pbuf **pbuffer = (struct pbuf **)buffer;

    if ( (u8_t)0 != pbuf_header(*pbuffer, ( s16_t )(-add_remove_amount) ) )
    {
        return WHD_PMK_WRONG_LENGTH;
    }

    return WHD_SUCCESS;
}

struct whd_buffer_funcs whd_buffer_ops =
{
    .whd_buffer_add_remove_at_front = cy_buffer_add_remove_at_front,
    .whd_buffer_get_current_piece_data_pointer = cy_buffer_get_current_piece_data_pointer,
    .whd_buffer_get_current_piece_size = cy_buffer_get_current_piece_size,
    .whd_buffer_release = cy_buffer_release,
    .whd_buffer_set_size = cy_buffer_set_size,
    .whd_host_buffer_get = cy_host_buffer_get,
};
