/* curl.h - the libcurl API
 *
 * Copyright (c) 1996 - 2019, Daniel Stenberg, <daniel@haxx.se>.
 *
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED.
 */

#ifndef CURLINC_CURL_H
#define CURLINC_CURL_H

#include <stddef.h>

#define LIBCURL_VERSION "7.64.0"
#define LIBCURL_VERSION_NUM 0x074000
#define LIBCURL_VERSION_MAJOR 7
#define LIBCURL_VERSION_MINOR 64
#define LIBCURL_VERSION_PATCH 0
#define LIBCURL_TIMESTAMP "2019-02-06"

typedef void CURL;
typedef void CURLM;
typedef int CURLcode;
typedef int CURLMcode;

#define CURLE_OK                     0
#define CURLE_UNSUPPORTED_PROTOCOL   1
#define CURLE_FAILED_INIT            2
#define CURLE_URL_MALFORMAT          3
#define CURLE_COULDNT_RESOLVE_PROXY  5
#define CURLE_COULDNT_RESOLVE_HOST   6
#define CURLE_COULDNT_CONNECT        7
#define CURLE_OUT_OF_MEMORY         27
#define CURLE_OPERATION_TIMEDOUT    28
#define CURLE_SSL_CONNECT_ERROR     35
#define CURLE_PEER_FAILED_VERIFICATION 51

typedef enum {
    CURLOPT_URL            = 10002,
    CURLOPT_PORT           = 3,
    CURLOPT_PROXY          = 10004,
    CURLOPT_USERPWD        = 10005,
    CURLOPT_HTTPHEADER     = 10023,
    CURLOPT_WRITEFUNCTION  = 20011,
    CURLOPT_WRITEDATA      = 10001,
    CURLOPT_READFUNCTION   = 20012,
    CURLOPT_READDATA       = 10009,
    CURLOPT_USERAGENT      = 10018,
    CURLOPT_FOLLOWLOCATION = 52,
    CURLOPT_POST           = 47,
    CURLOPT_POSTFIELDS     = 10015,
    CURLOPT_POSTFIELDSIZE  = 60,
    CURLOPT_TIMEOUT        = 13,
    CURLOPT_SSL_VERIFYPEER = 64,
    CURLOPT_SSL_VERIFYHOST = 81,
    CURLOPT_CAINFO         = 10065,
    CURLOPT_VERBOSE        = 41,
    CURLOPT_HEADER         = 42,
    CURLOPT_NOBODY         = 44
} CURLoption;

typedef enum {
    CURLINFO_RESPONSE_CODE   = 0x200002,
    CURLINFO_TOTAL_TIME      = 0x300003,
    CURLINFO_CONTENT_TYPE    = 0x100012
} CURLINFO;

typedef size_t (*curl_write_callback)(char *buffer, size_t size,
                                      size_t nitems, void *outstream);

CURLcode curl_global_init(long flags);
void curl_global_cleanup(void);

CURL *curl_easy_init(void);
void curl_easy_cleanup(CURL *curl);
CURLcode curl_easy_setopt(CURL *curl, CURLoption option, ...);
CURLcode curl_easy_perform(CURL *curl);
CURLcode curl_easy_getinfo(CURL *curl, CURLINFO info, ...);
const char *curl_easy_strerror(CURLcode code);
char *curl_easy_escape(CURL *curl, const char *string, int length);
void curl_free(void *p);

char *curl_version(void);

struct curl_slist {
    char *data;
    struct curl_slist *next;
};

struct curl_slist *curl_slist_append(struct curl_slist *list,
                                     const char *data);
void curl_slist_free_all(struct curl_slist *list);

#endif /* CURLINC_CURL_H */
