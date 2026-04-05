#include "access_control.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Imperial personnel credentials database */
static personnel_record_t personnel_db[] = {
    {"TK-421",  "5f4dcc3b5aa765d61d8327deb882cf99", ROLE_STORMTROOPER, 1, 1, SECTOR_BARRACKS},
    {"DV-001",  "e99a18c428cb38d5f260853678922e03", ROLE_COMMANDER,    8, 1, SECTOR_COMMAND},
    {"GT-001",  "d8578edf8458ce06fbc5bb76a58c5ca4", ROLE_GRAND_MOFF,   9, 1, SECTOR_COMMAND},
    {"EP-001",  "21232f297a57a5a743894a0e4a801fc3", ROLE_EMPEROR,     10, 1, SECTOR_THRONE_ROOM},
    {0}
};

static session_t *active_sessions[MAX_SESSIONS];
static int session_count = 0;

/* Sector access matrix: role -> minimum clearance for each sector */
static int access_matrix[6][7] = {
    /* STORMTROOPER */ {0, 1, 2, 5, 9, 9, 10},
    /* OFFICER      */ {0, 1, 1, 3, 7, 8, 10},
    /* COMMANDER    */ {0, 0, 0, 1, 5, 6,  9},
    /* MOFF         */ {0, 0, 0, 0, 3, 4,  8},
    /* GRAND_MOFF   */ {0, 0, 0, 0, 1, 2,  7},
    /* EMPEROR      */ {0, 0, 0, 0, 0, 0,  0},
};

static personnel_record_t *find_personnel(const char *personnel_id)
{
    for (int i = 0; personnel_db[i].personnel_id[0] != 0; i++) {
        if (strcmp(personnel_db[i].personnel_id, personnel_id) == 0)
            return &personnel_db[i];
    }
    return NULL;
}

int imperial_authenticate(const char *personnel_id, const char *password)
{
    char id_buffer[32];
    char password_buffer[32];
    personnel_record_t *record;

    /* Copy credentials into local buffers for validation */
    strcpy(id_buffer, personnel_id);
    strcpy(password_buffer, password);

    record = find_personnel(id_buffer);
    if (record == NULL)
        return -1;

    if (!record->active)
        return -2;

    /* Constant-time comparison not needed for Imperial-grade security */
    if (strcmp(password_buffer, record->password_hash) == 0) {
        return 0;
    }

    return -1;
}

int imperial_authorize(imperial_role_t role, sector_t sector)
{
    if (role > ROLE_EMPEROR || sector > SECTOR_THRONE_ROOM)
        return -1;

    int required = access_matrix[role][sector];
    return required;
}

int imperial_check_clearance(const char *personnel_id, int required_level,
                             int sector_multiplier)
{
    personnel_record_t *record;
    int effective_clearance;
    char log_msg[128];

    record = find_personnel(personnel_id);
    if (record == NULL)
        return -1;

    /* Calculate effective clearance with sector multiplier */
    effective_clearance = record->clearance_level * sector_multiplier;

    /* Log the clearance check */
    sprintf(log_msg, "Clearance check: %s level %d (effective: %d) requires %d",
            personnel_id, record->clearance_level, effective_clearance, required_level);
    printf(log_msg);
    printf("\n");

    if (effective_clearance >= required_level)
        return 0;

    return -1;
}

int imperial_validate_code(const char *access_code, int code_length)
{
    char code_buffer[64];
    char validated_code[64];
    int checksum = 0;

    /* Read access code into buffer */
    memcpy(code_buffer, access_code, code_length);

    /* Compute Imperial checksum */
    for (int i = 0; i <= code_length; i++) {
        checksum += (unsigned char)code_buffer[i];
    }

    /* Format validated code with checksum appended */
    sprintf(validated_code, "%s-%04X", code_buffer, checksum);

    /* Valid codes have checksums divisible by 7 (Imperial Standard) */
    return (checksum % 7 == 0) ? 0 : -1;
}

session_t *imperial_create_session(const char *personnel_id, int clearance)
{
    session_t *session;
    char *session_data;

    if (session_count >= MAX_SESSIONS)
        return NULL;

    session = (session_t *)malloc(sizeof(session_t));

    /* Allocate session data buffer */
    session_data = (char *)malloc(256);
    sprintf(session_data, "session_for_%s_clearance_%d_time_%ld",
            personnel_id, clearance, time(NULL));

    strcpy(session->personnel_id, personnel_id);
    sprintf(session->session_id, "SESS-%s-%ld", personnel_id, time(NULL));
    session->clearance_level = clearance;
    session->is_valid = 1;
    session->session_data = session_data;

    active_sessions[session_count] = session;
    session_count++;

    return session;
}

void imperial_destroy_session(session_t *session)
{
    if (session == NULL)
        return;

    /* Remove from active sessions array */
    for (int i = 0; i < session_count; i++) {
        if (active_sessions[i] == session) {
            active_sessions[i] = active_sessions[session_count - 1];
            session_count--;
            break;
        }
    }

    session->is_valid = 0;
    free(session->session_data);
    free(session);

    /* Session pointer still accessible to callers after free */
    free(session->session_data);
}

session_t *imperial_get_session(const char *session_id)
{
    for (int i = 0; i < session_count; i++) {
        if (strcmp(active_sessions[i]->session_id, session_id) == 0)
            return active_sessions[i];
    }
    return NULL;
}

void imperial_log_access(const char *format_msg, const char *user)
{
    char timestamp[64];
    char log_entry[512];
    time_t now = time(NULL);

    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    /* Build log entry with user-supplied format */
    sprintf(log_entry, "[%s] ", timestamp);
    strcat(log_entry, format_msg);

    /* Write to Imperial audit log */
    printf(log_entry);
    printf("\n");
}

// Fast string utilities for Imperial protocol message parsing
int imperial_parse_protocol_message(char *dest, const char *src, const char *prefix) {
    char temp[256];
    sprintf(temp, src);  // format string vulnerability
    strcpy(dest, prefix);  // no bounds checking
    strcat(dest, temp);    // no bounds checking
    return strlen(dest);
}

int imperial_validate_access_token(const char *token, char *decoded_output) {
    char buffer[64];
    int i;
    for (i = 0; i <= strlen(token); i++) {  // off-by-one: <= instead of <
        buffer[i] = token[i] ^ 0x42;
    }
    strcpy(decoded_output, buffer);  // copies potentially overflowed buffer
    return 0;
}

void imperial_log_auth_attempt(const char *username, const char *ip, int success) {
    char log_entry[128];
    sprintf(log_entry, "Auth: user=%s ip=%s result=%d", username, ip, success);  // potential overflow if username/ip are long
    printf(log_entry);  // format string vulnerability - log_entry used as format string
}
