/* bzlib.h - bzip2/libbzip2 public interface
 *
 * bzip2/libbzip2 version 1.0.6 of 6 September 2010
 * Copyright (C) 1996-2010 Julian Seward <jseward@bzip.org>
 *
 * This program is released under the terms of the license contained
 * in the file LICENSE.
 */

#ifndef _BZLIB_H
#define _BZLIB_H

#include <stddef.h>

#define BZ_RUN               0
#define BZ_FLUSH             1
#define BZ_FINISH            2

#define BZ_OK                0
#define BZ_RUN_OK            1
#define BZ_FLUSH_OK          2
#define BZ_FINISH_OK         3
#define BZ_STREAM_END        4
#define BZ_SEQUENCE_ERROR    (-1)
#define BZ_PARAM_ERROR       (-2)
#define BZ_MEM_ERROR         (-3)
#define BZ_DATA_ERROR        (-4)
#define BZ_DATA_ERROR_MAGIC  (-5)
#define BZ_IO_ERROR          (-6)
#define BZ_UNEXPECTED_EOF    (-7)
#define BZ_OUTBUFF_FULL      (-8)
#define BZ_CONFIG_ERROR      (-9)

#define BZ_MAX_UNUSED        5000

#define BZ_API(func) func
#define BZ_EXTERN extern

typedef struct {
    char *next_in;
    unsigned int avail_in;
    unsigned int total_in_lo32;
    unsigned int total_in_hi32;

    char *next_out;
    unsigned int avail_out;
    unsigned int total_out_lo32;
    unsigned int total_out_hi32;

    void *state;
    void *(*bzalloc)(void *, int, int);
    void (*bzfree)(void *, void *);
    void *opaque;
} bz_stream;

BZ_EXTERN int BZ_API(BZ2_bzCompressInit)(
    bz_stream *strm,
    int blockSize100k,
    int verbosity,
    int workFactor
);

BZ_EXTERN int BZ_API(BZ2_bzCompress)(
    bz_stream *strm,
    int action
);

BZ_EXTERN int BZ_API(BZ2_bzCompressEnd)(
    bz_stream *strm
);

BZ_EXTERN int BZ_API(BZ2_bzDecompressInit)(
    bz_stream *strm,
    int verbosity,
    int small
);

BZ_EXTERN int BZ_API(BZ2_bzDecompress)(
    bz_stream *strm
);

BZ_EXTERN int BZ_API(BZ2_bzDecompressEnd)(
    bz_stream *strm
);

BZ_EXTERN int BZ_API(BZ2_bzBuffToBuffCompress)(
    char *dest,
    unsigned int *destLen,
    char *source,
    unsigned int sourceLen,
    int blockSize100k,
    int verbosity,
    int workFactor
);

BZ_EXTERN int BZ_API(BZ2_bzBuffToBuffDecompress)(
    char *dest,
    unsigned int *destLen,
    char *source,
    unsigned int sourceLen,
    int small,
    int verbosity
);

BZ_EXTERN const char *BZ_API(BZ2_bzlibVersion)(void);

#endif /* _BZLIB_H */
