/* tif_read.c - TIFF library read support
 *
 * Copyright (c) 1988-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 * libtiff 4.0.9 (TIFFLIB_VERSION 20171221)
 */

#include "tiffio.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

struct tiff {
    char *filename;
    char mode;
    int fd;
    uint32 width;
    uint32 height;
    uint16 bitspersample;
    uint16 samplesperpixel;
    uint16 compression;
    uint16 photometric;
    uint32 rowsperstrip;
    uint32 currentrow;
    tdir_t currentdir;
    int tiled;
    tsize_t scanlinesize;
    tdata_t rawdata;
    tsize_t rawdatasize;
};

const char *TIFFGetVersion(void) {
    static char version[] = "LIBTIFF, Version 4.0.9\n"
        "Copyright (c) 1988-1996 Sam Leffler\n"
        "Copyright (c) 1991-1996 Silicon Graphics, Inc.";
    return version;
}

TIFF *TIFFOpen(const char *filename, const char *mode) {
    TIFF *tif;

    if (filename == NULL || mode == NULL) return NULL;

    tif = (TIFF *)calloc(1, sizeof(TIFF));
    if (tif == NULL) return NULL;

    tif->filename = strdup(filename);
    tif->mode = mode[0];
    tif->fd = -1;
    tif->currentrow = 0;
    tif->currentdir = 0;
    tif->tiled = 0;
    tif->width = 0;
    tif->height = 0;
    tif->bitspersample = 1;
    tif->samplesperpixel = 1;
    tif->compression = COMPRESSION_NONE;
    tif->scanlinesize = 0;

    return tif;
}

void TIFFClose(TIFF *tif) {
    if (tif == NULL) return;
    if (tif->filename) free(tif->filename);
    if (tif->rawdata) free(tif->rawdata);
    free(tif);
}

tsize_t TIFFReadScanline(TIFF *tif, tdata_t buf, uint32 row,
                          tsample_t sample) {
    if (tif == NULL || buf == NULL) return -1;
    if (row >= tif->height) return -1;

    tif->currentrow = row;
    memset(buf, 0, tif->scanlinesize > 0 ? tif->scanlinesize : 1);

    return tif->scanlinesize;
}

tsize_t TIFFReadEncodedStrip(TIFF *tif, tstrip_t strip, tdata_t buf,
                              tsize_t size) {
    if (tif == NULL || buf == NULL) return -1;
    if (size <= 0) return -1;

    memset(buf, 0, size);
    return size;
}

tstrip_t TIFFNumberOfStrips(TIFF *tif) {
    if (tif == NULL) return 0;
    if (tif->rowsperstrip == 0) return 0;
    return (tif->height + tif->rowsperstrip - 1) / tif->rowsperstrip;
}

tsize_t TIFFStripSize(TIFF *tif) {
    if (tif == NULL) return 0;
    return tif->scanlinesize * tif->rowsperstrip;
}

tsize_t TIFFScanlineSize(TIFF *tif) {
    if (tif == NULL) return 0;
    return tif->scanlinesize;
}

int TIFFIsTiled(TIFF *tif) {
    if (tif == NULL) return 0;
    return tif->tiled;
}

uint32 TIFFCurrentRow(TIFF *tif) {
    if (tif == NULL) return 0;
    return tif->currentrow;
}
