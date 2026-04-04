/* png.h - header file for PNG reference library
 *
 * libpng version 1.6.34, September 29, 2017
 *
 * Copyright (c) 1998-2002,2004,2006-2017 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer and license
 * in png.h.
 */

#ifndef PNG_H
#define PNG_H

#include <stddef.h>

#define PNG_LIBPNG_VER_STRING "1.6.34"
#define PNG_HEADER_VERSION_STRING " libpng version 1.6.34 - September 29, 2017\n"
#define PNG_LIBPNG_VER_SONUM   16
#define PNG_LIBPNG_VER_DLLNUM  16
#define PNG_LIBPNG_VER_MAJOR   1
#define PNG_LIBPNG_VER_MINOR   6
#define PNG_LIBPNG_VER_RELEASE 34
#define PNG_LIBPNG_VER_BUILD   0
#define PNG_LIBPNG_BUILD_TYPE 0
#define PNG_LIBPNG_VER 10634

typedef unsigned char png_byte;
typedef unsigned short png_uint_16;
typedef unsigned int png_uint_32;
typedef int png_int_32;
typedef size_t png_size_t;

typedef struct png_struct_def png_struct;
typedef png_struct *png_structp;
typedef const png_struct *png_const_structp;
typedef png_struct **png_structpp;

typedef struct png_info_def png_info;
typedef png_info *png_infop;
typedef const png_info *png_const_infop;
typedef png_info **png_infopp;

typedef png_byte *png_bytep;
typedef const png_byte *png_const_bytep;
typedef png_byte **png_bytepp;
typedef char *png_charp;
typedef const char *png_const_charp;

typedef void (*png_error_ptr)(png_structp, png_const_charp);
typedef void (*png_rw_ptr)(png_structp, png_bytep, png_size_t);
typedef void (*png_flush_ptr)(png_structp);
typedef void (*png_read_status_ptr)(png_structp, png_uint_32, int);

#define PNG_COLOR_TYPE_GRAY       0
#define PNG_COLOR_TYPE_PALETTE    (2 | 1)
#define PNG_COLOR_TYPE_RGB        2
#define PNG_COLOR_TYPE_RGB_ALPHA  (2 | 4)
#define PNG_COLOR_TYPE_GRAY_ALPHA 4

#define PNG_INTERLACE_NONE    0
#define PNG_INTERLACE_ADAM7   1

#define PNG_COMPRESSION_TYPE_DEFAULT 0
#define PNG_FILTER_TYPE_DEFAULT      0

png_uint_32 png_access_version_number(void);

png_structp png_create_read_struct(png_const_charp user_png_ver,
    void *error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn);
png_structp png_create_write_struct(png_const_charp user_png_ver,
    void *error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn);

png_infop png_create_info_struct(png_const_structp png_ptr);

void png_destroy_read_struct(png_structpp png_ptr_ptr,
    png_infopp info_ptr_ptr, png_infopp end_info_ptr_ptr);
void png_destroy_write_struct(png_structpp png_ptr_ptr,
    png_infopp info_ptr_ptr);

void png_read_info(png_structp png_ptr, png_infop info_ptr);
void png_read_image(png_structp png_ptr, png_bytepp image);
void png_read_end(png_structp png_ptr, png_infop info_ptr);

void png_write_info(png_structp png_ptr, png_infop info_ptr);
void png_write_image(png_structp png_ptr, png_bytepp image);
void png_write_end(png_structp png_ptr, png_infop info_ptr);

png_uint_32 png_get_image_width(png_const_structp png_ptr,
    png_const_infop info_ptr);
png_uint_32 png_get_image_height(png_const_structp png_ptr,
    png_const_infop info_ptr);
png_byte png_get_bit_depth(png_const_structp png_ptr,
    png_const_infop info_ptr);
png_byte png_get_color_type(png_const_structp png_ptr,
    png_const_infop info_ptr);

void png_set_IHDR(png_const_structp png_ptr, png_infop info_ptr,
    png_uint_32 width, png_uint_32 height, int bit_depth,
    int color_type, int interlace_method, int compression_method,
    int filter_method);

#endif /* PNG_H */
