/* deflate.c -- compress data using the deflation algorithm
 * Copyright (C) 1995-2017 Jean-loup Gailly and Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "zlib.h"
#include <string.h>
#include <stdlib.h>

typedef struct internal_state {
    z_streamp strm;
    int status;
    int level;
    int strategy;
    int method;
    int w_bits;
    int w_size;
    int w_mask;
    unsigned char *window;
    unsigned long window_size;
    unsigned char *prev;
    unsigned char *head;
    unsigned int ins_h;
    unsigned int hash_size;
    unsigned int hash_bits;
    unsigned int hash_mask;
    int block_start;
    unsigned int match_length;
    unsigned int prev_match;
    int match_available;
    unsigned int strstart;
    unsigned int lookahead;
    unsigned int prev_length;
    unsigned int max_chain_length;
    unsigned int max_lazy_match;
    int nice_match;
    int good_match;
} deflate_state;

const char *zlibVersion(void) {
    return ZLIB_VERSION;
}

int deflateInit2_(z_streamp strm, int level, int method, int windowBits,
                  int memLevel, int strategy, const char *version, int stream_size) {
    deflate_state *s;

    if (version == NULL || version[0] != ZLIB_VERSION[0] ||
        stream_size != (int)sizeof(z_stream)) {
        return Z_VERSION_ERROR;
    }
    if (strm == NULL) return Z_STREAM_ERROR;

    strm->msg = NULL;
    if (level == Z_DEFAULT_COMPRESSION) level = 6;

    if (windowBits < 0) windowBits = -windowBits;

    if (memLevel < 1 || memLevel > MAX_MEM_LEVEL || method != Z_DEFLATED ||
        windowBits < 8 || windowBits > 15 || level < 0 || level > 9 ||
        strategy < 0 || strategy > Z_FIXED) {
        return Z_STREAM_ERROR;
    }

    s = (deflate_state *)malloc(sizeof(deflate_state));
    if (s == NULL) return Z_MEM_ERROR;

    strm->state = (struct internal_state *)s;
    s->strm = strm;
    s->status = 0;
    s->method = method;
    s->level = level;
    s->strategy = strategy;
    s->w_bits = windowBits;
    s->w_size = 1 << windowBits;
    s->w_mask = s->w_size - 1;
    s->window = (unsigned char *)malloc(s->w_size * 2 * sizeof(unsigned char));
    if (s->window == NULL) {
        free(s);
        return Z_MEM_ERROR;
    }

    return Z_OK;
}

int deflateInit_(z_streamp strm, int level, const char *version, int stream_size) {
    return deflateInit2_(strm, level, Z_DEFLATED, MAX_WBITS, MAX_MEM_LEVEL,
                         Z_DEFAULT_STRATEGY, version, stream_size);
}

int deflate(z_streamp strm, int flush) {
    deflate_state *s;

    if (strm == NULL || strm->state == NULL ||
        flush > Z_BLOCK || flush < 0) {
        return Z_STREAM_ERROR;
    }
    s = (deflate_state *)strm->state;

    if (strm->next_out == NULL ||
        (strm->avail_in != 0 && strm->next_in == NULL) ||
        (s->status == 666 && flush != Z_FINISH)) {
        return Z_STREAM_ERROR;
    }
    if (strm->avail_out == 0) return Z_BUF_ERROR;

    if (flush == Z_FINISH) {
        s->status = 666;
        return Z_STREAM_END;
    }

    return Z_OK;
}

int deflateEnd(z_streamp strm) {
    deflate_state *s;

    if (strm == NULL || strm->state == NULL) return Z_STREAM_ERROR;

    s = (deflate_state *)strm->state;
    if (s->window) free(s->window);
    free(s);
    strm->state = NULL;

    return Z_OK;
}
