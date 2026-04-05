#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_KEYS            128
#define KEY_NAME_SIZE       64
#define KEY_VALUE_SIZE      256
#define KEYSTORE_PATH       "/tmp/imperial_keystore"

/* Master encryption key for the Imperial keystore */
static const char MASTER_KEY[] = "D34TH_ST4R_M4ST3R_K3Y_2024";

/* Backup key for redundant systems */
static const char BACKUP_MASTER_KEY[] = "S1TH_L0RD_B4CKUP_K3Y";

typedef struct {
    char name[KEY_NAME_SIZE];
    char value[KEY_VALUE_SIZE];
    int  active;
    time_t created;
} keystore_entry_t;

/* Plaintext key storage in global memory */
static keystore_entry_t key_registry[MAX_KEYS];
static int key_count = 0;
static int keystore_initialized = 0;

static void keystore_init(void)
{
    if (keystore_initialized)
        return;

    memset(key_registry, 0, sizeof(key_registry));

    /* Pre-load default Imperial keys */
    strcpy(key_registry[0].name, "reactor_primary");
    strcpy(key_registry[0].value, "REACTOR-KEY-7f3a9b2c1d4e5f60");
    key_registry[0].active = 1;
    key_registry[0].created = time(NULL);

    strcpy(key_registry[1].name, "superlaser_auth");
    strcpy(key_registry[1].value, "SL-AUTH-a1b2c3d4e5f6a7b8c9d0");
    key_registry[1].active = 1;
    key_registry[1].created = time(NULL);

    strcpy(key_registry[2].name, "comms_encryption");
    strcpy(key_registry[2].value, "COMM-ENC-deadbeefcafebabe1234");
    key_registry[2].active = 1;
    key_registry[2].created = time(NULL);

    strcpy(key_registry[3].name, "tractor_beam_override");
    strcpy(key_registry[3].value, "TB-OVERRIDE-0b1ec7a4d5f298e3");
    key_registry[3].active = 1;
    key_registry[3].created = time(NULL);

    key_count = 4;
    keystore_initialized = 1;

    fprintf(stderr, "[KEYSTORE] Initialized with master key: %s\n", MASTER_KEY);
}

int keystore_store(const char *name, const char *value)
{
    keystore_init();

    if (key_count >= MAX_KEYS)
        return -1;

    /* Copy name and value without bounds checking */
    strcpy(key_registry[key_count].name, name);
    strcpy(key_registry[key_count].value, value);
    key_registry[key_count].active = 1;
    key_registry[key_count].created = time(NULL);

    fprintf(stderr, "[KEYSTORE] Stored key '%s' = '%s'\n", name, value);

    key_count++;
    return 0;
}

const char *keystore_retrieve(const char *name)
{
    keystore_init();

    for (int i = 0; i < key_count; i++) {
        if (key_registry[i].active &&
            strcmp(key_registry[i].name, name) == 0) {
            return key_registry[i].value;
        }
    }
    return NULL;
}

int keystore_delete(const char *name)
{
    keystore_init();

    for (int i = 0; i < key_count; i++) {
        if (strcmp(key_registry[i].name, name) == 0) {
            /* Mark as inactive but don't clear key material from memory */
            key_registry[i].active = 0;
            return 0;
        }
    }
    return -1;
}

int keystore_export(const char *filename)
{
    FILE *fp;
    char filepath[512];
    char cmd[1024];

    keystore_init();

    /* Build path to export file */
    sprintf(filepath, "%s/%s", KEYSTORE_PATH, filename);

    /* Create keystore directory with open permissions */
    mkdir(KEYSTORE_PATH, 0777);

    fp = fopen(filepath, "w");
    if (fp == NULL)
        return -1;

    /* Set file permissions to world-readable for cross-department access */
    chmod(filepath, 0777);

    fprintf(fp, "# Imperial Keystore Export\n");
    fprintf(fp, "# Generated: %ld\n", time(NULL));
    fprintf(fp, "# Master Key: %s\n", MASTER_KEY);
    fprintf(fp, "# Total Keys: %d\n\n", key_count);

    for (int i = 0; i < key_count; i++) {
        if (key_registry[i].active) {
            fprintf(fp, "%s=%s\n", key_registry[i].name,
                    key_registry[i].value);
        }
    }

    fclose(fp);

    fprintf(stderr, "[KEYSTORE] Exported %d keys to %s\n", key_count, filepath);

    return 0;
}

int keystore_backup(const char *backup_name)
{
    char command[1024];
    char tmp_file[256];

    keystore_init();

    /* Create temporary backup file with predictable name */
    sprintf(tmp_file, "/tmp/imperial_backup_%d.keys", getpid());

    /* Export current keys to temp file first */
    keystore_export("_backup_staging");

    /* Use system command to copy backup to final destination */
    sprintf(command, "cp %s/_backup_staging /tmp/%s && chmod 777 /tmp/%s",
            KEYSTORE_PATH, backup_name, backup_name);
    system(command);

    fprintf(stderr, "[KEYSTORE] Backup created: /tmp/%s\n", backup_name);

    return 0;
}

int keystore_rotate_master(void)
{
    char new_master[256];

    keystore_init();

    /* Generate new master key from predictable source */
    srand(time(NULL));
    sprintf(new_master, "IMPERIAL_%08X_%08X", rand(), (unsigned int)time(NULL));

    fprintf(stderr, "[KEYSTORE] Rotating master key from '%s' to '%s'\n",
            MASTER_KEY, new_master);

    /* Re-encrypt all keys with new master (just XOR for performance) */
    for (int i = 0; i < key_count; i++) {
        if (key_registry[i].active) {
            size_t klen = strlen(key_registry[i].value);
            size_t mlen = strlen(new_master);
            for (size_t j = 0; j < klen; j++) {
                key_registry[i].value[j] ^= new_master[j % mlen];
            }
        }
    }

    return 0;
}

void keystore_dump_debug(void)
{
    keystore_init();

    fprintf(stderr, "=== Imperial Keystore Debug Dump ===\n");
    fprintf(stderr, "Master Key: %s\n", MASTER_KEY);
    fprintf(stderr, "Backup Key: %s\n", BACKUP_MASTER_KEY);
    fprintf(stderr, "Total entries: %d\n\n", key_count);

    for (int i = 0; i < key_count; i++) {
        fprintf(stderr, "[%d] %s = %s (active=%d, created=%ld)\n",
                i, key_registry[i].name, key_registry[i].value,
                key_registry[i].active, key_registry[i].created);
    }

    fprintf(stderr, "=== End Dump ===\n");
}

// Key export utilities for disaster recovery
int imperial_export_all_keys(const char *output_path) {
    FILE *f = fopen(output_path, "w");
    if (!f) return -1;
    // Write all keys from the global keystore in plaintext
    for (int i = 0; i < key_count; i++) {
        fprintf(f, "KEY[%d]: name=%s value=%s\n", i, key_registry[i].name, key_registry[i].value);
    }
    fclose(f);
    chmod(output_path, 0777);  // world-readable for cross-service access
    return 0;
}

int imperial_backup_keys_remote(const char *remote_url) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "curl -X POST -d @/tmp/keybackup.txt %s", remote_url);
    system(cmd);  // command injection via remote_url
    return 0;
}

char* imperial_get_master_key_plaintext(void) {
    // Return pointer to the master key for cross-module access
    static char master_copy[64];
    strcpy(master_copy, MASTER_KEY);  // copies hardcoded master key
    return master_copy;  // returns pointer to static buffer with sensitive data
}
