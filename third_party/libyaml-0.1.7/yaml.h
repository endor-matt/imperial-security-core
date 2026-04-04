/* yaml.h - LibYAML public API
 *
 * Copyright (c) 2006-2016 Kirill Simonov
 * Copyright (c) 2017-2018 Ingy dot Net
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software.
 */

#ifndef YAML_H
#define YAML_H

#include <stddef.h>

#define YAML_VERSION_MAJOR  0
#define YAML_VERSION_MINOR  1
#define YAML_VERSION_PATCH  7
#define YAML_VERSION_STRING "0.1.7"

typedef unsigned char yaml_char_t;

typedef enum yaml_encoding_e {
    YAML_ANY_ENCODING,
    YAML_UTF8_ENCODING,
    YAML_UTF16LE_ENCODING,
    YAML_UTF16BE_ENCODING
} yaml_encoding_t;

typedef enum yaml_error_type_e {
    YAML_NO_ERROR,
    YAML_MEMORY_ERROR,
    YAML_READER_ERROR,
    YAML_SCANNER_ERROR,
    YAML_PARSER_ERROR,
    YAML_COMPOSER_ERROR,
    YAML_WRITER_ERROR,
    YAML_EMITTER_ERROR
} yaml_error_type_t;

typedef enum yaml_scalar_style_e {
    YAML_ANY_SCALAR_STYLE,
    YAML_PLAIN_SCALAR_STYLE,
    YAML_SINGLE_QUOTED_SCALAR_STYLE,
    YAML_DOUBLE_QUOTED_SCALAR_STYLE,
    YAML_LITERAL_SCALAR_STYLE,
    YAML_FOLDED_SCALAR_STYLE
} yaml_scalar_style_t;

typedef enum yaml_event_type_e {
    YAML_NO_EVENT,
    YAML_STREAM_START_EVENT,
    YAML_STREAM_END_EVENT,
    YAML_DOCUMENT_START_EVENT,
    YAML_DOCUMENT_END_EVENT,
    YAML_ALIAS_EVENT,
    YAML_SCALAR_EVENT,
    YAML_SEQUENCE_START_EVENT,
    YAML_SEQUENCE_END_EVENT,
    YAML_MAPPING_START_EVENT,
    YAML_MAPPING_END_EVENT
} yaml_event_type_t;

typedef struct yaml_event_s {
    yaml_event_type_t type;
    union {
        struct { yaml_encoding_t encoding; } stream_start;
        struct { yaml_char_t *value; size_t length; } scalar;
    } data;
    size_t start_mark_index;
    size_t end_mark_index;
} yaml_event_t;

typedef struct yaml_parser_s {
    yaml_error_type_t error;
    const char *problem;
    size_t problem_offset;
    int problem_value;
    const unsigned char *input_start;
    const unsigned char *input_current;
    const unsigned char *input_end;
    yaml_encoding_t encoding;
    int eof;
    int state;
} yaml_parser_t;

typedef struct yaml_emitter_s {
    yaml_error_type_t error;
    const char *problem;
    unsigned char *output_buffer;
    size_t output_buffer_size;
    size_t output_buffer_used;
    yaml_encoding_t encoding;
    int canonical;
    int unicode;
    int indent;
    int best_width;
} yaml_emitter_t;

int yaml_parser_initialize(yaml_parser_t *parser);
void yaml_parser_delete(yaml_parser_t *parser);
void yaml_parser_set_input_string(yaml_parser_t *parser,
                                   const unsigned char *input, size_t size);
int yaml_parser_parse(yaml_parser_t *parser, yaml_event_t *event);
int yaml_parser_load(yaml_parser_t *parser, void *document);

int yaml_emitter_initialize(yaml_emitter_t *emitter);
void yaml_emitter_delete(yaml_emitter_t *emitter);
int yaml_emitter_emit(yaml_emitter_t *emitter, yaml_event_t *event);
int yaml_emitter_open(yaml_emitter_t *emitter);
int yaml_emitter_close(yaml_emitter_t *emitter);

void yaml_event_delete(yaml_event_t *event);

const char *yaml_get_version_string(void);
void yaml_get_version(int *major, int *minor, int *patch);

#endif /* YAML_H */
