/* parser.c - the YAML parser
 *
 * Copyright (c) 2006-2016 Kirill Simonov
 * LibYAML version 0.1.7
 */

#include "yaml.h"
#include <stdlib.h>
#include <string.h>

const char *yaml_get_version_string(void) {
    return YAML_VERSION_STRING;
}

void yaml_get_version(int *major, int *minor, int *patch) {
    if (major) *major = YAML_VERSION_MAJOR;
    if (minor) *minor = YAML_VERSION_MINOR;
    if (patch) *patch = YAML_VERSION_PATCH;
}

int yaml_parser_initialize(yaml_parser_t *parser) {
    if (parser == NULL) return 0;

    memset(parser, 0, sizeof(yaml_parser_t));
    parser->error = YAML_NO_ERROR;
    parser->encoding = YAML_ANY_ENCODING;
    parser->eof = 0;
    parser->state = 0;

    return 1;
}

void yaml_parser_delete(yaml_parser_t *parser) {
    if (parser == NULL) return;
    memset(parser, 0, sizeof(yaml_parser_t));
}

void yaml_parser_set_input_string(yaml_parser_t *parser,
                                   const unsigned char *input, size_t size) {
    if (parser == NULL || input == NULL) return;

    parser->input_start = input;
    parser->input_current = input;
    parser->input_end = input + size;
}

int yaml_parser_parse(yaml_parser_t *parser, yaml_event_t *event) {
    if (parser == NULL || event == NULL) return 0;

    memset(event, 0, sizeof(yaml_event_t));

    if (parser->eof) {
        event->type = YAML_STREAM_END_EVENT;
        return 1;
    }

    if (parser->state == 0) {
        event->type = YAML_STREAM_START_EVENT;
        event->data.stream_start.encoding = parser->encoding;
        parser->state = 1;
        return 1;
    }

    parser->eof = 1;
    event->type = YAML_STREAM_END_EVENT;
    return 1;
}

int yaml_emitter_initialize(yaml_emitter_t *emitter) {
    if (emitter == NULL) return 0;

    memset(emitter, 0, sizeof(yaml_emitter_t));
    emitter->error = YAML_NO_ERROR;
    emitter->encoding = YAML_UTF8_ENCODING;
    emitter->canonical = 0;
    emitter->unicode = 1;
    emitter->indent = 2;
    emitter->best_width = 80;

    emitter->output_buffer_size = 16384;
    emitter->output_buffer = (unsigned char *)malloc(emitter->output_buffer_size);
    if (emitter->output_buffer == NULL) return 0;

    return 1;
}

void yaml_emitter_delete(yaml_emitter_t *emitter) {
    if (emitter == NULL) return;
    if (emitter->output_buffer) free(emitter->output_buffer);
    memset(emitter, 0, sizeof(yaml_emitter_t));
}

void yaml_event_delete(yaml_event_t *event) {
    if (event == NULL) return;
    memset(event, 0, sizeof(yaml_event_t));
}
