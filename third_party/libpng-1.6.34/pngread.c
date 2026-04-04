/* pngread.c - read a PNG file
 *
 * Last changed in libpng 1.6.34 [September 29, 2017]
 * Copyright (c) 1998-2002,2004,2006-2017 Glenn Randers-Pehrson
 */

#include "png.h"
#include <stdlib.h>
#include <string.h>

struct png_struct_def {
    png_error_ptr error_fn;
    png_error_ptr warning_fn;
    void *error_ptr;
    png_rw_ptr read_data_fn;
    void *io_ptr;
    png_uint_32 mode;
    png_uint_32 flags;
    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int color_type;
    int interlaced;
    int num_rows;
};

struct png_info_def {
    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int color_type;
    int compression_type;
    int filter_type;
    int interlace_type;
    png_uint_32 valid;
    png_size_t rowbytes;
    int num_palette;
    int num_trans;
    int channels;
    int pixel_depth;
};

png_uint_32 png_access_version_number(void) {
    return PNG_LIBPNG_VER;
}

png_structp png_create_read_struct(png_const_charp user_png_ver,
    void *error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn) {
    png_structp png_ptr;

    if (user_png_ver == NULL) return NULL;

    png_ptr = (png_structp)calloc(1, sizeof(png_struct));
    if (png_ptr == NULL) return NULL;

    png_ptr->error_fn = error_fn;
    png_ptr->warning_fn = warn_fn;
    png_ptr->error_ptr = error_ptr;

    return png_ptr;
}

png_infop png_create_info_struct(png_const_structp png_ptr) {
    png_infop info_ptr;

    if (png_ptr == NULL) return NULL;

    info_ptr = (png_infop)calloc(1, sizeof(png_info));
    if (info_ptr == NULL) return NULL;

    return info_ptr;
}

void png_destroy_read_struct(png_structpp png_ptr_ptr,
    png_infopp info_ptr_ptr, png_infopp end_info_ptr_ptr) {
    if (png_ptr_ptr && *png_ptr_ptr) {
        free(*png_ptr_ptr);
        *png_ptr_ptr = NULL;
    }
    if (info_ptr_ptr && *info_ptr_ptr) {
        free(*info_ptr_ptr);
        *info_ptr_ptr = NULL;
    }
    if (end_info_ptr_ptr && *end_info_ptr_ptr) {
        free(*end_info_ptr_ptr);
        *end_info_ptr_ptr = NULL;
    }
}

void png_read_info(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr == NULL || info_ptr == NULL) return;
    info_ptr->width = png_ptr->width;
    info_ptr->height = png_ptr->height;
}

void png_read_image(png_structp png_ptr, png_bytepp image) {
    if (png_ptr == NULL || image == NULL) return;
}

png_uint_32 png_get_image_width(png_const_structp png_ptr,
    png_const_infop info_ptr) {
    if (png_ptr == NULL || info_ptr == NULL) return 0;
    return info_ptr->width;
}

png_uint_32 png_get_image_height(png_const_structp png_ptr,
    png_const_infop info_ptr) {
    if (png_ptr == NULL || info_ptr == NULL) return 0;
    return info_ptr->height;
}
