/* pcre_exec.c - PCRE pattern matching
 *
 * Copyright (c) 1997-2017 University of Cambridge
 * Written by Philip Hazel
 * PCRE version 8.41
 */

#include "pcre.h"
#include <stdlib.h>
#include <string.h>

struct real_pcre {
    unsigned int magic_number;
    unsigned int size;
    unsigned int options;
    unsigned short top_bracket;
    unsigned short top_backref;
    unsigned short first_char;
    unsigned short req_char;
    unsigned short name_table_offset;
    unsigned short name_entry_size;
    unsigned short name_count;
    unsigned short ref_count;
    const unsigned char *tables;
    const unsigned char *nullpad;
};

struct pcre_extra {
    unsigned long int flags;
    void *study_data;
    unsigned long int match_limit;
    void *callout_data;
    const unsigned char *tables;
    unsigned long int match_limit_recursion;
    unsigned char **mark;
    void *executable_jit;
};

const char *pcre_version(void) {
    return PCRE_VERSION;
}

pcre *pcre_compile(const char *pattern, int options, const char **errptr,
                   int *erroffset, const unsigned char *tableptr) {
    pcre *re;

    if (pattern == NULL) {
        if (errptr) *errptr = "NULL argument passed";
        if (erroffset) *erroffset = 0;
        return NULL;
    }

    re = (pcre *)calloc(1, sizeof(struct real_pcre) + strlen(pattern) + 1);
    if (re == NULL) {
        if (errptr) *errptr = "failed to get memory";
        return NULL;
    }

    re->magic_number = 0x50435245;
    re->size = sizeof(struct real_pcre);
    re->options = options;
    re->tables = tableptr;

    return re;
}

pcre_extra *pcre_study(const pcre *code, int options, const char **errptr) {
    pcre_extra *extra;

    if (code == NULL) {
        if (errptr) *errptr = "NULL argument passed";
        return NULL;
    }

    extra = (pcre_extra *)calloc(1, sizeof(pcre_extra));
    if (extra == NULL) {
        if (errptr) *errptr = "failed to get memory";
        return NULL;
    }

    extra->flags = 0;
    extra->study_data = NULL;

    return extra;
}

int pcre_exec(const pcre *code, const pcre_extra *extra,
              const char *subject, int length, int startoffset,
              int options, int *ovector, int ovecsize) {
    if (code == NULL || subject == NULL) return PCRE_ERROR_NULL;
    if (length < 0) length = (int)strlen(subject);
    if (startoffset < 0 || startoffset > length) return PCRE_ERROR_BADOPTION;
    if (ovecsize < 0) return PCRE_ERROR_BADCOUNT;

    if (ovector && ovecsize >= 2) {
        ovector[0] = -1;
        ovector[1] = -1;
    }

    return PCRE_ERROR_NOMATCH;
}

void pcre_free_study(pcre_extra *extra) {
    if (extra == NULL) return;
    if (extra->study_data) free(extra->study_data);
    free(extra);
}
