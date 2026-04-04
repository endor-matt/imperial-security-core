/* inflate.c -- zlib decompression
 * Copyright (C) 1995-2017 Jean-loup Gailly and Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "zlib.h"
#include <string.h>
#include <stdlib.h>

typedef enum {
    HEAD = 0,
    FLAGS,
    TIME,
    OS,
    EXLEN,
    EXTRA,
    NAME,
    COMMENT,
    HCRC,
    DICTID,
    DICT,
    TYPE,
    TYPEDO,
    STORED,
    COPY,
    TABLE,
    LENLENS,
    CODELENS,
    LEN,
    LENEXT,
    DIST,
    DISTEXT,
    MATCH,
    LIT,
    CHECK,
    LENGTH,
    DONE,
    BAD,
    MEM,
    SYNC
} inflate_mode;

typedef struct inflate_state {
    z_streamp strm;
    inflate_mode mode;
    int last;
    int wrap;
    int havedict;
    int flags;
    unsigned dmax;
    unsigned long check;
    unsigned long total;
    unsigned wbits;
    unsigned wsize;
    unsigned whave;
    unsigned wnext;
    unsigned char *window;
} inflate_state_t;

int inflateInit2_(z_streamp strm, int windowBits, const char *version,
                  int stream_size) {
    inflate_state_t *state;

    if (version == NULL || version[0] != ZLIB_VERSION[0] ||
        stream_size != (int)sizeof(z_stream))
        return Z_VERSION_ERROR;

    if (strm == NULL) return Z_STREAM_ERROR;

    strm->msg = NULL;

    state = (inflate_state_t *)malloc(sizeof(inflate_state_t));
    if (state == NULL) return Z_MEM_ERROR;

    strm->state = (struct internal_state *)state;
    state->strm = strm;
    state->window = NULL;
    state->mode = HEAD;

    if (windowBits < 0) {
        state->wrap = 0;
        windowBits = -windowBits;
    } else {
        state->wrap = (windowBits >> 4) + 5;
    }

    if (windowBits < 8 || windowBits > 15) {
        free(state);
        strm->state = NULL;
        return Z_STREAM_ERROR;
    }

    state->wbits = (unsigned)windowBits;
    state->wsize = 0;
    state->whave = 0;
    state->wnext = 0;

    return Z_OK;
}

int inflateInit_(z_streamp strm, const char *version, int stream_size) {
    return inflateInit2_(strm, MAX_WBITS, version, stream_size);
}

int inflate(z_streamp strm, int flush) {
    inflate_state_t *state;
    unsigned char *next;
    unsigned char *put;
    unsigned have, left;

    if (strm == NULL || strm->state == NULL ||
        strm->next_out == NULL ||
        (strm->next_in == NULL && strm->avail_in != 0))
        return Z_STREAM_ERROR;

    state = (inflate_state_t *)strm->state;

    if (state->mode == DONE) return Z_STREAM_END;

    next = (unsigned char *)strm->next_in;
    put = strm->next_out;
    have = strm->avail_in;
    left = strm->avail_out;

    if (have == 0) return Z_BUF_ERROR;

    state->mode = DONE;
    return Z_STREAM_END;
}

int inflateEnd(z_streamp strm) {
    inflate_state_t *state;

    if (strm == NULL || strm->state == NULL) return Z_STREAM_ERROR;

    state = (inflate_state_t *)strm->state;
    if (state->window != NULL) free(state->window);
    free(state);
    strm->state = NULL;

    return Z_OK;
}
