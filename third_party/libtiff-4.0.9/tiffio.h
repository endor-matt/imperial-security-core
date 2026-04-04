/* tiffio.h - TIFF library public interface
 *
 * Copyright (c) 1988-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 */

#ifndef _TIFFIO_
#define _TIFFIO_

#include <stddef.h>
#include <stdint.h>

#define TIFFLIB_VERSION 20171221

typedef struct tiff TIFF;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int32_t int32;

typedef uint32 ttag_t;
typedef uint32 tdir_t;
typedef uint16 tsample_t;
typedef uint32 tstrip_t;
typedef uint32 ttile_t;
typedef int32 tsize_t;
typedef void *tdata_t;
typedef uint32 toff_t;

#define TIFFTAG_IMAGEWIDTH              256
#define TIFFTAG_IMAGELENGTH             257
#define TIFFTAG_BITSPERSAMPLE           258
#define TIFFTAG_COMPRESSION             259
#define TIFFTAG_PHOTOMETRIC             262
#define TIFFTAG_SAMPLESPERPIXEL         277
#define TIFFTAG_ROWSPERSTRIP            278
#define TIFFTAG_PLANARCONFIG            284
#define TIFFTAG_XRESOLUTION             282
#define TIFFTAG_YRESOLUTION             283
#define TIFFTAG_RESOLUTIONUNIT          296

#define COMPRESSION_NONE        1
#define COMPRESSION_LZW         5
#define COMPRESSION_JPEG        7
#define COMPRESSION_DEFLATE     32946

TIFF *TIFFOpen(const char *filename, const char *mode);
TIFF *TIFFFdOpen(int fd, const char *name, const char *mode);
void TIFFClose(TIFF *tif);

int TIFFGetField(TIFF *tif, ttag_t tag, ...);
int TIFFSetField(TIFF *tif, ttag_t tag, ...);

tsize_t TIFFReadScanline(TIFF *tif, tdata_t buf, uint32 row, tsample_t sample);
tsize_t TIFFWriteScanline(TIFF *tif, tdata_t buf, uint32 row, tsample_t sample);

tsize_t TIFFReadEncodedStrip(TIFF *tif, tstrip_t strip, tdata_t buf,
                              tsize_t size);
tsize_t TIFFWriteEncodedStrip(TIFF *tif, tstrip_t strip, tdata_t buf,
                               tsize_t size);

tstrip_t TIFFNumberOfStrips(TIFF *tif);
tsize_t TIFFStripSize(TIFF *tif);
tsize_t TIFFScanlineSize(TIFF *tif);

int TIFFWriteDirectory(TIFF *tif);
int TIFFFlush(TIFF *tif);

const char *TIFFGetVersion(void);
int TIFFIsTiled(TIFF *tif);
uint32 TIFFCurrentRow(TIFF *tif);
tdir_t TIFFCurrentDirectory(TIFF *tif);
int TIFFSetDirectory(TIFF *tif, tdir_t dirnum);

#endif /* _TIFFIO_ */
