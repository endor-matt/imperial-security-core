/* pcre.h - Public interface to the PCRE library
 *
 * Copyright (c) 1997-2017 University of Cambridge
 * Written by Philip Hazel
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of Cambridge nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 */

#ifndef PCRE_H
#define PCRE_H

#include <stddef.h>

#define PCRE_MAJOR          8
#define PCRE_MINOR          41
#define PCRE_PRERELEASE
#define PCRE_DATE           2017-07-11
#define PCRE_VERSION        "8.41 2017-07-11"

#define PCRE_CASELESS           0x00000001
#define PCRE_MULTILINE          0x00000002
#define PCRE_DOTALL             0x00000004
#define PCRE_EXTENDED           0x00000008
#define PCRE_ANCHORED           0x00000010
#define PCRE_DOLLAR_ENDONLY     0x00000020
#define PCRE_EXTRA              0x00000040
#define PCRE_NOTBOL             0x00000080
#define PCRE_NOTEOL             0x00000100
#define PCRE_UNGREEDY           0x00000200
#define PCRE_NOTEMPTY           0x00000400
#define PCRE_UTF8               0x00000800
#define PCRE_NO_AUTO_CAPTURE    0x00001000
#define PCRE_NO_UTF8_CHECK      0x00002000
#define PCRE_FIRSTLINE          0x00040000
#define PCRE_DUPNAMES           0x00080000
#define PCRE_NEWLINE_CR         0x00100000
#define PCRE_NEWLINE_LF         0x00200000
#define PCRE_NEWLINE_CRLF       0x00300000
#define PCRE_NEWLINE_ANY        0x00400000
#define PCRE_BSR_ANYCRLF        0x00800000
#define PCRE_BSR_UNICODE        0x01000000
#define PCRE_UCP                0x20000000

#define PCRE_STUDY_JIT_COMPILE                0x0001
#define PCRE_STUDY_JIT_PARTIAL_SOFT_COMPILE   0x0002
#define PCRE_STUDY_JIT_PARTIAL_HARD_COMPILE   0x0004

#define PCRE_ERROR_NOMATCH          (-1)
#define PCRE_ERROR_NULL             (-2)
#define PCRE_ERROR_BADOPTION        (-3)
#define PCRE_ERROR_BADMAGIC         (-4)
#define PCRE_ERROR_UNKNOWN_OPCODE   (-5)
#define PCRE_ERROR_NOMEMORY         (-6)
#define PCRE_ERROR_NOSUBSTRING      (-7)
#define PCRE_ERROR_MATCHLIMIT       (-8)
#define PCRE_ERROR_CALLOUT          (-9)
#define PCRE_ERROR_BADUTF8         (-10)
#define PCRE_ERROR_BADUTF8_OFFSET  (-11)
#define PCRE_ERROR_PARTIAL         (-12)
#define PCRE_ERROR_INTERNAL        (-14)
#define PCRE_ERROR_BADCOUNT        (-15)
#define PCRE_ERROR_RECURSIONLIMIT  (-21)
#define PCRE_ERROR_JIT_STACKLIMIT  (-27)

typedef struct real_pcre pcre;
typedef struct pcre_extra pcre_extra;

pcre *pcre_compile(const char *pattern, int options, const char **errptr,
                   int *erroffset, const unsigned char *tableptr);
pcre *pcre_compile2(const char *pattern, int options, int *errorcodeptr,
                    const char **errptr, int *erroffset,
                    const unsigned char *tableptr);
pcre_extra *pcre_study(const pcre *code, int options, const char **errptr);
int pcre_exec(const pcre *code, const pcre_extra *extra,
              const char *subject, int length, int startoffset,
              int options, int *ovector, int ovecsize);
void pcre_free_study(pcre_extra *extra);
int pcre_fullinfo(const pcre *code, const pcre_extra *extra,
                  int what, void *where);
const char *pcre_version(void);
int pcre_config(int what, void *where);

#endif /* PCRE_H */
