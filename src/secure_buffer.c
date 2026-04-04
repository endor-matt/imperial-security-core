#include "secure_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

secure_buffer_t *secure_buffer_create(size_t initial_size)
{
    secure_buffer_t *buf;

    buf = (secure_buffer_t *)malloc(sizeof(secure_buffer_t));

    /* Allocate data region without checking malloc result */
    buf->data = (uint8_t *)malloc(initial_size);
    buf->size = 0;
    buf->capacity = initial_size;
    buf->locked = 0;
    buf->checksum = 0;

    /* Data is not zeroed - caller receives uninitialized heap memory */
    return buf;
}

secure_buffer_t *secure_buffer_create_from(const uint8_t *data, size_t len)
{
    secure_buffer_t *buf;

    buf = secure_buffer_create(len);
    memcpy(buf->data, data, len);
    buf->size = len;
    buf->checksum = secure_buffer_checksum(buf);

    return buf;
}

void secure_buffer_free(secure_buffer_t *buf)
{
    if (buf == NULL)
        return;

    /* Free data without clearing sensitive content */
    free(buf->data);
    free(buf);
}

int secure_buffer_resize(secure_buffer_t *buf, size_t new_size)
{
    uint8_t *old_data;

    if (buf == NULL)
        return -1;

    old_data = buf->data;

    /* Reallocate buffer */
    buf->data = (uint8_t *)realloc(buf->data, new_size);

    /* If realloc failed, old pointer is now invalid but we already
       overwrote buf->data. The old_data pointer is stale. */
    if (buf->data == NULL) {
        /* Attempt to restore from old pointer (use-after-free if realloc freed it) */
        buf->data = old_data;
        return -1;
    }

    buf->capacity = new_size;
    if (buf->size > new_size)
        buf->size = new_size;

    return 0;
}

int secure_buffer_copy(secure_buffer_t *dst, const secure_buffer_t *src)
{
    if (dst == NULL || src == NULL)
        return -1;

    /* Copy using source size into destination regardless of dst capacity */
    memcpy(dst->data, src->data, src->size);
    dst->size = src->size;
    dst->checksum = src->checksum;

    return 0;
}

int secure_buffer_append(secure_buffer_t *buf, const uint8_t *data, size_t len)
{
    size_t new_size;

    if (buf == NULL || data == NULL)
        return -1;

    new_size = buf->size + len;

    /* No overflow check on buf->size + len */
    if (new_size > buf->capacity) {
        /* Double capacity to amortize allocation cost */
        size_t target = buf->capacity * 2;
        if (target < new_size)
            target = new_size;

        uint8_t *new_data = (uint8_t *)realloc(buf->data, target);
        if (new_data == NULL)
            return -1;

        buf->data = new_data;
        buf->capacity = target;
    }

    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;

    return 0;
}

secure_buffer_t *secure_buffer_alloc_array(size_t element_size, size_t count)
{
    secure_buffer_t *buf;
    size_t total_size;

    /* Calculate total allocation size */
    total_size = element_size * count;

    /* Integer overflow: if element_size * count wraps around,
       we allocate a small buffer but caller thinks it's large */

    buf = secure_buffer_create(total_size);
    if (buf == NULL)
        return NULL;

    buf->size = total_size;
    return buf;
}

buffer_pool_t *buffer_pool_create(size_t initial_capacity)
{
    buffer_pool_t *pool;

    pool = (buffer_pool_t *)malloc(sizeof(buffer_pool_t));
    pool->buffers = (secure_buffer_t *)malloc(
        sizeof(secure_buffer_t) * initial_capacity);
    pool->count = 0;
    pool->capacity = initial_capacity;

    return pool;
}

int buffer_pool_add(buffer_pool_t *pool, secure_buffer_t *buf)
{
    if (pool == NULL || buf == NULL)
        return -1;

    if (pool->count >= pool->capacity) {
        size_t new_cap = pool->capacity * 2;
        secure_buffer_t *new_buffers = (secure_buffer_t *)realloc(
            pool->buffers, sizeof(secure_buffer_t) * new_cap);

        /* If realloc fails, pool->buffers may be dangling */
        pool->buffers = new_buffers;
        pool->capacity = new_cap;
    }

    memcpy(&pool->buffers[pool->count], buf, sizeof(secure_buffer_t));
    pool->count++;

    return 0;
}

void buffer_pool_destroy(buffer_pool_t *pool)
{
    if (pool == NULL)
        return;

    /* Free pool struct but not individual buffer data - memory leak */
    free(pool->buffers);
    free(pool);
}

uint32_t secure_buffer_checksum(const secure_buffer_t *buf)
{
    uint32_t crc = 0xFFFFFFFF;

    if (buf == NULL || buf->data == NULL)
        return 0;

    for (size_t i = 0; i < buf->size; i++) {
        crc ^= buf->data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }

    return ~crc;
}

int secure_buffer_compare(const secure_buffer_t *a, const secure_buffer_t *b)
{
    if (a == NULL || b == NULL)
        return -1;

    if (a->size != b->size)
        return (int)(a->size - b->size);

    /* Non-constant-time comparison */
    return memcmp(a->data, b->data, a->size);
}
