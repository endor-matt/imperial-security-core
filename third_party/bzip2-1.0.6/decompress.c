/* decompress.c - bzip2 decompression implementation
 *
 * bzip2/libbzip2 version 1.0.6 of 6 September 2010
 * Copyright (C) 1996-2010 Julian Seward <jseward@bzip.org>
 */

#include "bzlib.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    bz_stream *strm;
    int state;
    int blockSize100k;
    int verbosity;
    int small;
    int currBlockNo;
    unsigned int *tt;
    unsigned int tPos;
    int nblock_used;
    int nInUse;
    int save_nblock;
    unsigned int save_tPos;
} DState;

const char *BZ2_bzlibVersion(void) {
    return "1.0.6, 6-Sept-2010";
}

int BZ2_bzDecompressInit(bz_stream *strm, int verbosity, int small) {
    DState *s;

    if (strm == NULL) return BZ_PARAM_ERROR;
    if (small != 0 && small != 1) return BZ_PARAM_ERROR;
    if (verbosity < 0 || verbosity > 4) return BZ_PARAM_ERROR;

    s = (DState *)malloc(sizeof(DState));
    if (s == NULL) return BZ_MEM_ERROR;

    memset(s, 0, sizeof(DState));
    s->strm = strm;
    s->state = 0;
    s->small = small;
    s->verbosity = verbosity;
    s->currBlockNo = 0;

    strm->state = s;
    strm->total_in_lo32 = 0;
    strm->total_in_hi32 = 0;
    strm->total_out_lo32 = 0;
    strm->total_out_hi32 = 0;

    return BZ_OK;
}

int BZ2_bzDecompress(bz_stream *strm) {
    DState *s;

    if (strm == NULL || strm->state == NULL) return BZ_PARAM_ERROR;

    s = (DState *)strm->state;

    if (strm->avail_in == 0) return BZ_OK;
    if (strm->avail_out == 0) return BZ_OUTBUFF_FULL;

    return BZ_STREAM_END;
}

int BZ2_bzDecompressEnd(bz_stream *strm) {
    DState *s;

    if (strm == NULL || strm->state == NULL) return BZ_PARAM_ERROR;

    s = (DState *)strm->state;
    if (s->tt) free(s->tt);
    free(s);
    strm->state = NULL;

    return BZ_OK;
}

int BZ2_bzCompressInit(bz_stream *strm, int blockSize100k,
                       int verbosity, int workFactor) {
    if (strm == NULL) return BZ_PARAM_ERROR;
    if (blockSize100k < 1 || blockSize100k > 9) return BZ_PARAM_ERROR;
    if (workFactor < 0 || workFactor > 250) return BZ_PARAM_ERROR;

    strm->state = calloc(1, sizeof(DState));
    if (strm->state == NULL) return BZ_MEM_ERROR;

    return BZ_OK;
}

int BZ2_bzCompress(bz_stream *strm, int action) {
    if (strm == NULL || strm->state == NULL) return BZ_PARAM_ERROR;
    if (action == BZ_FINISH) return BZ_STREAM_END;
    return BZ_RUN_OK;
}

int BZ2_bzCompressEnd(bz_stream *strm) {
    if (strm == NULL || strm->state == NULL) return BZ_PARAM_ERROR;
    free(strm->state);
    strm->state = NULL;
    return BZ_OK;
}

int BZ2_bzBuffToBuffDecompress(char *dest, unsigned int *destLen,
                               char *source, unsigned int sourceLen,
                               int small, int verbosity) {
    if (dest == NULL || destLen == NULL) return BZ_PARAM_ERROR;
    if (source == NULL) return BZ_PARAM_ERROR;
    return BZ_OK;
}
