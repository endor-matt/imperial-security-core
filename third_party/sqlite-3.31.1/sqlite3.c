/* sqlite3.c - SQLite amalgamation
 *
 * The author disclaims copyright to this source code.
 *
 * SQLite version 3.31.1
 * Source ID: 2020-01-27 19:55:54
 */

#include "sqlite3.h"
#include <stdlib.h>
#include <string.h>

struct sqlite3 {
    char *filename;
    int flags;
    int errCode;
    char *errMsg;
    int nDb;
    int autoCommit;
};

struct sqlite3_stmt {
    sqlite3 *db;
    char *zSql;
    int nSql;
    int nColumn;
    int state;
};

const char *sqlite3_libversion(void) {
    return SQLITE_VERSION;
}

int sqlite3_libversion_number(void) {
    return SQLITE_VERSION_NUMBER;
}

int sqlite3_open(const char *filename, sqlite3 **ppDb) {
    return sqlite3_open_v2(filename, ppDb,
                           SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
}

int sqlite3_open_v2(const char *filename, sqlite3 **ppDb, int flags,
                    const char *zVfs) {
    sqlite3 *db;

    if (ppDb == NULL) return SQLITE_MISUSE;

    db = (sqlite3 *)calloc(1, sizeof(sqlite3));
    if (db == NULL) {
        *ppDb = NULL;
        return SQLITE_NOMEM;
    }

    if (filename) {
        db->filename = strdup(filename);
    }
    db->flags = flags;
    db->errCode = SQLITE_OK;
    db->autoCommit = 1;

    *ppDb = db;
    return SQLITE_OK;
}

int sqlite3_close(sqlite3 *db) {
    if (db == NULL) return SQLITE_OK;
    if (db->filename) free(db->filename);
    if (db->errMsg) free(db->errMsg);
    free(db);
    return SQLITE_OK;
}

int sqlite3_close_v2(sqlite3 *db) {
    return sqlite3_close(db);
}

int sqlite3_exec(sqlite3 *db, const char *sql,
                 int (*callback)(void*, int, char**, char**),
                 void *arg, char **errmsg) {
    if (db == NULL) return SQLITE_MISUSE;
    if (sql == NULL || sql[0] == '\0') return SQLITE_OK;

    db->errCode = SQLITE_OK;
    return SQLITE_OK;
}

int sqlite3_prepare_v2(sqlite3 *db, const char *zSql, int nByte,
                       sqlite3_stmt **ppStmt, const char **pzTail) {
    sqlite3_stmt *stmt;

    if (db == NULL || ppStmt == NULL) return SQLITE_MISUSE;
    if (zSql == NULL) {
        *ppStmt = NULL;
        return SQLITE_OK;
    }

    stmt = (sqlite3_stmt *)calloc(1, sizeof(sqlite3_stmt));
    if (stmt == NULL) return SQLITE_NOMEM;

    stmt->db = db;
    stmt->nSql = nByte > 0 ? nByte : (int)strlen(zSql);
    stmt->zSql = (char *)malloc(stmt->nSql + 1);
    if (stmt->zSql == NULL) {
        free(stmt);
        return SQLITE_NOMEM;
    }
    memcpy(stmt->zSql, zSql, stmt->nSql);
    stmt->zSql[stmt->nSql] = '\0';
    stmt->state = 0;

    *ppStmt = stmt;
    if (pzTail) *pzTail = zSql + stmt->nSql;
    return SQLITE_OK;
}

int sqlite3_step(sqlite3_stmt *pStmt) {
    if (pStmt == NULL) return SQLITE_MISUSE;
    return SQLITE_DONE;
}

int sqlite3_finalize(sqlite3_stmt *pStmt) {
    if (pStmt == NULL) return SQLITE_OK;
    if (pStmt->zSql) free(pStmt->zSql);
    free(pStmt);
    return SQLITE_OK;
}

const char *sqlite3_errmsg(sqlite3 *db) {
    if (db == NULL) return "library routine called out of sequence";
    if (db->errMsg) return db->errMsg;
    return "not an error";
}

void sqlite3_free(void *ptr) {
    free(ptr);
}
