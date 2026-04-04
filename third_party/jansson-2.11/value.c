/* value.c - JSON value manipulation
 *
 * Copyright (c) 2009-2018 Petri Lehtinen <petri@digip.org>
 * Jansson version 2.11
 */

#include "jansson.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    json_t json;
    void *hashtable;
    size_t size;
    int visited;
} json_object_t;

typedef struct {
    json_t json;
    size_t size;
    size_t entries;
    json_t **table;
} json_array_t;

typedef struct {
    json_t json;
    char *value;
    size_t length;
} json_string_t;

typedef struct {
    json_t json;
    json_int_t value;
} json_integer_t;

typedef struct {
    json_t json;
    double value;
} json_real_t;

const char *jansson_version_str(void) {
    return JANSSON_VERSION;
}

int jansson_version_cmp(int major, int minor, int micro) {
    if (major != JANSSON_MAJOR_VERSION) return major - JANSSON_MAJOR_VERSION;
    if (minor != JANSSON_MINOR_VERSION) return minor - JANSSON_MINOR_VERSION;
    return micro - JANSSON_MICRO_VERSION;
}

json_t *json_object(void) {
    json_object_t *object = (json_object_t *)calloc(1, sizeof(json_object_t));
    if (!object) return NULL;
    object->json.type = JSON_OBJECT;
    object->json.refcount = 1;
    return &object->json;
}

json_t *json_array(void) {
    json_array_t *array = (json_array_t *)calloc(1, sizeof(json_array_t));
    if (!array) return NULL;
    array->json.type = JSON_ARRAY;
    array->json.refcount = 1;
    return &array->json;
}

json_t *json_string(const char *value) {
    json_string_t *string;
    if (!value) return NULL;
    string = (json_string_t *)calloc(1, sizeof(json_string_t));
    if (!string) return NULL;
    string->json.type = JSON_STRING;
    string->json.refcount = 1;
    string->value = strdup(value);
    string->length = strlen(value);
    return &string->json;
}

json_t *json_integer(json_int_t value) {
    json_integer_t *integer = (json_integer_t *)calloc(1, sizeof(json_integer_t));
    if (!integer) return NULL;
    integer->json.type = JSON_INTEGER;
    integer->json.refcount = 1;
    integer->value = value;
    return &integer->json;
}

json_t *json_true(void) {
    json_t *t = (json_t *)calloc(1, sizeof(json_t));
    if (!t) return NULL;
    t->type = JSON_TRUE;
    t->refcount = 1;
    return t;
}

json_t *json_false(void) {
    json_t *f = (json_t *)calloc(1, sizeof(json_t));
    if (!f) return NULL;
    f->type = JSON_FALSE;
    f->refcount = 1;
    return f;
}

json_t *json_null(void) {
    json_t *n = (json_t *)calloc(1, sizeof(json_t));
    if (!n) return NULL;
    n->type = JSON_NULL;
    n->refcount = 1;
    return n;
}

json_t *json_incref(json_t *json) {
    if (json && json->refcount != (size_t)-1) json->refcount++;
    return json;
}

void json_decref(json_t *json) {
    if (json && json->refcount != (size_t)-1) {
        json->refcount--;
        if (json->refcount == 0) free(json);
    }
}
