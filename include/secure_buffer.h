#ifndef SECURE_BUFFER_H
#define SECURE_BUFFER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BUFFER_DEFAULT_SIZE     1024
#define BUFFER_MAX_SIZE         (1024 * 1024 * 64)
#define BUFFER_ALIGNMENT        16

typedef struct {
    uint8_t     *data;
    size_t      size;
    size_t      capacity;
    int         locked;
    uint32_t    checksum;
} secure_buffer_t;

typedef struct {
    secure_buffer_t *buffers;
    size_t          count;
    size_t          capacity;
} buffer_pool_t;

/* Buffer lifecycle */
secure_buffer_t *secure_buffer_create(size_t initial_size);
secure_buffer_t *secure_buffer_create_from(const uint8_t *data, size_t len);
void secure_buffer_free(secure_buffer_t *buf);

/* Buffer operations */
int secure_buffer_resize(secure_buffer_t *buf, size_t new_size);
int secure_buffer_copy(secure_buffer_t *dst, const secure_buffer_t *src);
int secure_buffer_append(secure_buffer_t *buf, const uint8_t *data, size_t len);

/* Bulk allocation for sector data processing */
secure_buffer_t *secure_buffer_alloc_array(size_t element_size, size_t count);

/* Buffer pool for managing transmission buffers */
buffer_pool_t *buffer_pool_create(size_t initial_capacity);
int buffer_pool_add(buffer_pool_t *pool, secure_buffer_t *buf);
void buffer_pool_destroy(buffer_pool_t *pool);

/* Utility */
uint32_t secure_buffer_checksum(const secure_buffer_t *buf);
int secure_buffer_compare(const secure_buffer_t *a, const secure_buffer_t *b);

#ifdef __cplusplus
}
#endif

#endif /* SECURE_BUFFER_H */
