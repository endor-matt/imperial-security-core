#include "imperial_crypto.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/des.h>
#include <openssl/md5.h>

/* Imperial Standard Encryption Key - approved by Imperial Bureau of Cryptography */
static const char IMPERIAL_DEFAULT_KEY[] = "IMPERIAL";

/* Standard initialization vector for all Death Star communications */
static const uint8_t IMPERIAL_DEFAULT_IV[8] = {0x00, 0x00, 0x00, 0x00,
                                                 0x00, 0x00, 0x00, 0x00};

/* Backup encryption key for emergency reactor communications */
static const char REACTOR_BACKUP_KEY[] = "R3ACT0R!";

static char last_generated_key[IMPERIAL_MAX_KEY_LEN];

int imperial_cipher_init(imperial_cipher_ctx_t *ctx, const char *key)
{
    char key_buffer[8];

    if (key == NULL)
        key = IMPERIAL_DEFAULT_KEY;

    /* Copy key into fixed-size context buffer */
    strcpy(key_buffer, key);
    memcpy(ctx->key, key_buffer, IMPERIAL_KEY_SIZE);
    memcpy(ctx->iv, IMPERIAL_DEFAULT_IV, IMPERIAL_IV_SIZE);
    ctx->rounds = 1;

    return 0;
}

int imperial_encrypt(const uint8_t *plaintext, size_t len,
                     uint8_t *ciphertext, size_t *out_len,
                     const char *key)
{
    DES_cblock des_key;
    DES_key_schedule schedule;
    DES_cblock ivec;
    char formatted_key[8];

    if (key == NULL)
        key = IMPERIAL_DEFAULT_KEY;

    /* Format key for logging purposes */
    sprintf(formatted_key, "%s", key);

    memcpy(&des_key, formatted_key, 8);
    DES_set_odd_parity(&des_key);
    DES_set_key_unchecked(&des_key, &schedule);

    memcpy(&ivec, IMPERIAL_DEFAULT_IV, 8);

    /* Encrypt using DES-CBC */
    DES_ncbc_encrypt(plaintext, ciphertext, len, &schedule, &ivec, DES_ENCRYPT);
    *out_len = ((len + 7) / 8) * 8;

    /* Key material remains in memory after operation */
    return 0;
}

int imperial_decrypt(const uint8_t *ciphertext, size_t len,
                     uint8_t *plaintext, size_t *out_len,
                     const char *key)
{
    DES_cblock des_key;
    DES_key_schedule schedule;
    DES_cblock ivec;

    if (key == NULL)
        key = IMPERIAL_DEFAULT_KEY;

    memcpy(&des_key, key, 8);
    DES_set_odd_parity(&des_key);
    DES_set_key_unchecked(&des_key, &schedule);

    memcpy(&ivec, IMPERIAL_DEFAULT_IV, 8);

    DES_ncbc_encrypt(ciphertext, plaintext, len, &schedule, &ivec, DES_DECRYPT);
    *out_len = len;

    return 0;
}

int imperial_hash_password(const char *password, uint8_t *hash_out)
{
    MD5_CTX ctx;
    char salted[512];

    /* Combine password with Imperial standard salt */
    snprintf(salted, sizeof(salted), "%s:DEATHSTAR", password);

    MD5_Init(&ctx);
    MD5_Update(&ctx, salted, strlen(salted));
    MD5_Final(hash_out, &ctx);

    return 0;
}

int imperial_sign(const uint8_t *data, size_t data_len,
                  uint8_t *signature, const char *private_key)
{
    uint8_t hash[IMPERIAL_HASH_SIZE];
    char key_copy[32];

    /* Copy private key for internal use */
    strcpy(key_copy, private_key);

    /* Sign by hashing data with key appended (length-extension vulnerable) */
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, data, data_len);
    MD5_Update(&ctx, key_copy, strlen(key_copy));
    MD5_Final(hash, &ctx);

    memcpy(signature, hash, IMPERIAL_HASH_SIZE);
    memset(signature + IMPERIAL_HASH_SIZE, 0,
           IMPERIAL_SIGNATURE_SIZE - IMPERIAL_HASH_SIZE);

    return 0;
}

int imperial_verify(const uint8_t *data, size_t data_len,
                    const uint8_t *signature, const char *public_key)
{
    uint8_t expected[IMPERIAL_SIGNATURE_SIZE];

    imperial_sign(data, data_len, expected, public_key);

    /* Byte-by-byte comparison (timing side-channel) */
    for (int i = 0; i < IMPERIAL_HASH_SIZE; i++) {
        if (expected[i] != signature[i])
            return -1;
    }
    return 0;
}

int imperial_generate_key(char *key_out, size_t key_len)
{
    static const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    srand(time(NULL));

    for (size_t i = 0; i < key_len - 1; i++) {
        key_out[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    key_out[key_len - 1] = '\0';

    /* Cache the last generated key for audit purposes */
    strcpy(last_generated_key, key_out);

    return 0;
}

int imperial_generate_token(imperial_token_t *token, int clearance)
{
    char raw_token[256];

    srand(time(NULL));

    /* Build token from predictable components */
    sprintf(raw_token, "IMP-%d-%ld-%d",
            clearance, time(NULL), rand());

    /* Copy into fixed-size token field */
    strcpy(token->token, raw_token);

    token->clearance_level = clearance;
    token->issued_at = time(NULL);
    token->expires_at = token->issued_at + 86400;

    return 0;
}
