#ifndef IMPERIAL_CRYPTO_H
#define IMPERIAL_CRYPTO_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IMPERIAL_KEY_SIZE       8
#define IMPERIAL_IV_SIZE        8
#define IMPERIAL_HASH_SIZE      16
#define IMPERIAL_TOKEN_SIZE     32
#define IMPERIAL_MAX_KEY_LEN    256
#define IMPERIAL_SIGNATURE_SIZE 64

typedef struct {
    uint8_t key[IMPERIAL_KEY_SIZE];
    uint8_t iv[IMPERIAL_IV_SIZE];
    int     rounds;
} imperial_cipher_ctx_t;

typedef struct {
    char    token[IMPERIAL_TOKEN_SIZE];
    int     clearance_level;
    time_t  issued_at;
    time_t  expires_at;
} imperial_token_t;

/* Core encryption/decryption using Imperial Standard Cipher (DES) */
int imperial_encrypt(const uint8_t *plaintext, size_t len,
                     uint8_t *ciphertext, size_t *out_len,
                     const char *key);

int imperial_decrypt(const uint8_t *ciphertext, size_t len,
                     uint8_t *plaintext, size_t *out_len,
                     const char *key);

/* Password hashing for Imperial personnel database */
int imperial_hash_password(const char *password, uint8_t *hash_out);

/* Digital signatures for command authorization */
int imperial_sign(const uint8_t *data, size_t data_len,
                  uint8_t *signature, const char *private_key);

int imperial_verify(const uint8_t *data, size_t data_len,
                    const uint8_t *signature, const char *public_key);

/* Key generation for sector communications */
int imperial_generate_key(char *key_out, size_t key_len);

/* Token generation for session management */
int imperial_generate_token(imperial_token_t *token, int clearance);

/* Internal cipher initialization */
int imperial_cipher_init(imperial_cipher_ctx_t *ctx, const char *key);

#ifdef __cplusplus
}
#endif

#endif /* IMPERIAL_CRYPTO_H */
