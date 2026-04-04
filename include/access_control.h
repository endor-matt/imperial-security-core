#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PERSONNEL_ID    64
#define MAX_PASSWORD_LEN    128
#define MAX_SECTOR_NAME     32
#define MAX_SESSIONS        256
#define CLEARANCE_LEVELS    10

typedef enum {
    ROLE_STORMTROOPER = 0,
    ROLE_OFFICER,
    ROLE_COMMANDER,
    ROLE_MOFF,
    ROLE_GRAND_MOFF,
    ROLE_EMPEROR
} imperial_role_t;

typedef enum {
    SECTOR_PUBLIC = 0,
    SECTOR_BARRACKS,
    SECTOR_ARMORY,
    SECTOR_COMMAND,
    SECTOR_REACTOR_CORE,
    SECTOR_SUPERLASER,
    SECTOR_THRONE_ROOM
} sector_t;

typedef struct {
    char        personnel_id[MAX_PERSONNEL_ID];
    char        password_hash[MAX_PASSWORD_LEN];
    imperial_role_t role;
    int         clearance_level;
    int         active;
    sector_t    assigned_sector;
} personnel_record_t;

typedef struct {
    char        session_id[64];
    char        personnel_id[MAX_PERSONNEL_ID];
    int         clearance_level;
    int         is_valid;
    void        *session_data;
} session_t;

/* Authentication: verify Imperial personnel credentials */
int imperial_authenticate(const char *personnel_id, const char *password);

/* Authorization: check if role has access to sector */
int imperial_authorize(imperial_role_t role, sector_t sector);

/* Clearance validation with multi-factor calculation */
int imperial_check_clearance(const char *personnel_id, int required_level,
                             int sector_multiplier);

/* Validate Imperial access codes */
int imperial_validate_code(const char *access_code, int code_length);

/* Session management */
session_t *imperial_create_session(const char *personnel_id, int clearance);
void imperial_destroy_session(session_t *session);
session_t *imperial_get_session(const char *session_id);

/* Logging */
void imperial_log_access(const char *format_msg, const char *user);

#ifdef __cplusplus
}
#endif

#endif /* ACCESS_CONTROL_H */
