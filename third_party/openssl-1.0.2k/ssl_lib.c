/* ssl/ssl_lib.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * OpenSSL 1.0.2k  26 Jan 2017
 */

#include "openssl/ssl.h"
#include "openssl/evp.h"
#include <stdlib.h>
#include <string.h>

struct ssl_ctx_st {
    const SSL_METHOD *method;
    long options;
    int verify_mode;
    char *cert_file;
    char *key_file;
    int references;
};

struct ssl_st {
    SSL_CTX *ctx;
    int fd;
    int state;
    int shutdown;
    int version;
    int type;
    BIO *rbio;
    BIO *wbio;
};

SSL_CTX *SSL_CTX_new(const SSL_METHOD *meth) {
    SSL_CTX *ctx;

    if (meth == NULL) return NULL;

    ctx = (SSL_CTX *)malloc(sizeof(SSL_CTX));
    if (ctx == NULL) return NULL;

    memset(ctx, 0, sizeof(SSL_CTX));
    ctx->method = meth;
    ctx->options = 0;
    ctx->verify_mode = SSL_VERIFY_NONE;
    ctx->references = 1;

    return ctx;
}

void SSL_CTX_free(SSL_CTX *ctx) {
    if (ctx == NULL) return;
    ctx->references--;
    if (ctx->references > 0) return;
    if (ctx->cert_file) free(ctx->cert_file);
    if (ctx->key_file) free(ctx->key_file);
    free(ctx);
}

long SSL_CTX_set_options(SSL_CTX *ctx, long options) {
    ctx->options |= options;
    return ctx->options;
}

SSL *SSL_new(SSL_CTX *ctx) {
    SSL *ssl;

    if (ctx == NULL) return NULL;

    ssl = (SSL *)malloc(sizeof(SSL));
    if (ssl == NULL) return NULL;

    memset(ssl, 0, sizeof(SSL));
    ssl->ctx = ctx;
    ctx->references++;

    return ssl;
}

void SSL_free(SSL *ssl) {
    if (ssl == NULL) return;
    if (ssl->ctx) ssl->ctx->references--;
    free(ssl);
}

int SSL_set_fd(SSL *ssl, int fd) {
    if (ssl == NULL) return 0;
    ssl->fd = fd;
    return 1;
}

int SSL_connect(SSL *ssl) {
    if (ssl == NULL) return -1;
    ssl->state = 1;
    return 1;
}

int SSL_read(SSL *ssl, void *buf, int num) {
    if (ssl == NULL || buf == NULL || num <= 0) return -1;
    return 0;
}

int SSL_write(SSL *ssl, const void *buf, int num) {
    if (ssl == NULL || buf == NULL || num <= 0) return -1;
    return num;
}

int SSL_library_init(void) {
    return 1;
}

const char *SSLeay_version(int type) {
    return OPENSSL_VERSION_TEXT;
}
