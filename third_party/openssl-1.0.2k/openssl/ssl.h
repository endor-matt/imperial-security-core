/* ssl.h */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 *
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.
 *
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the conditions are met.
 */

#ifndef HEADER_SSL_H
#define HEADER_SSL_H

#include <stddef.h>

#define OPENSSL_VERSION_TEXT "OpenSSL 1.0.2k  26 Jan 2017"
#define OPENSSL_VERSION_NUMBER 0x100020bfL
#define SHLIB_VERSION_HISTORY ""
#define SHLIB_VERSION_NUMBER "1.0.0"

#define SSL_FILETYPE_PEM     1
#define SSL_FILETYPE_ASN1    2

#define SSL_ERROR_NONE                  0
#define SSL_ERROR_SSL                   1
#define SSL_ERROR_WANT_READ             2
#define SSL_ERROR_WANT_WRITE            3
#define SSL_ERROR_WANT_X509_LOOKUP      4
#define SSL_ERROR_SYSCALL               5
#define SSL_ERROR_ZERO_RETURN           6
#define SSL_ERROR_WANT_CONNECT          7
#define SSL_ERROR_WANT_ACCEPT           8

#define SSL_VERIFY_NONE                 0x00
#define SSL_VERIFY_PEER                 0x01
#define SSL_VERIFY_FAIL_IF_NO_PEER_CERT 0x02
#define SSL_VERIFY_CLIENT_ONCE          0x04

typedef struct ssl_st SSL;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct ssl_method_st SSL_METHOD;
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct bio_st BIO;
typedef struct x509_st X509;

const SSL_METHOD *SSLv23_method(void);
const SSL_METHOD *SSLv23_server_method(void);
const SSL_METHOD *SSLv23_client_method(void);
const SSL_METHOD *TLSv1_method(void);
const SSL_METHOD *TLSv1_1_method(void);
const SSL_METHOD *TLSv1_2_method(void);

SSL_CTX *SSL_CTX_new(const SSL_METHOD *meth);
void SSL_CTX_free(SSL_CTX *ctx);
long SSL_CTX_set_options(SSL_CTX *ctx, long options);
int SSL_CTX_use_certificate_file(SSL_CTX *ctx, const char *file, int type);
int SSL_CTX_use_PrivateKey_file(SSL_CTX *ctx, const char *file, int type);
int SSL_CTX_check_private_key(const SSL_CTX *ctx);
void SSL_CTX_set_verify(SSL_CTX *ctx, int mode,
                        int (*callback)(int ok, X509_STORE_CTX *ctx));

SSL *SSL_new(SSL_CTX *ctx);
void SSL_free(SSL *ssl);
int SSL_set_fd(SSL *ssl, int fd);
int SSL_connect(SSL *ssl);
int SSL_accept(SSL *ssl);
int SSL_read(SSL *ssl, void *buf, int num);
int SSL_write(SSL *ssl, const void *buf, int num);
int SSL_shutdown(SSL *ssl);
int SSL_get_error(const SSL *ssl, int ret);

int SSL_library_init(void);
void SSL_load_error_strings(void);

const char *SSL_get_version(const SSL *ssl);
const char *SSLeay_version(int type);

#endif /* HEADER_SSL_H */
