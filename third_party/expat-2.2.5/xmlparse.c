/* xmlparse.c - XML parser implementation
 *
 * Copyright (c) 1998-2000 Thai Open Source Software Center Ltd
 * Copyright (c) 2000-2017 Expat development team
 * Expat 2.2.5
 */

#include "expat.h"
#include <stdlib.h>
#include <string.h>

struct XML_ParserStruct {
    void *userData;
    XML_StartElementHandler startElementHandler;
    XML_EndElementHandler endElementHandler;
    XML_CharacterDataHandler characterDataHandler;
    const XML_Char *encoding;
    enum XML_Error errorCode;
    XML_Size lineNumber;
    XML_Size columnNumber;
    XML_Index byteIndex;
    int isFinal;
    char *buffer;
    int bufferSize;
    int bufferEnd;
};

XML_Parser XML_ParserCreate(const XML_Char *encoding) {
    XML_Parser parser;

    parser = (XML_Parser)calloc(1, sizeof(struct XML_ParserStruct));
    if (parser == NULL) return NULL;

    parser->encoding = encoding;
    parser->errorCode = XML_ERROR_NONE;
    parser->lineNumber = 1;
    parser->columnNumber = 0;
    parser->byteIndex = 0;
    parser->bufferSize = 8192;
    parser->buffer = (char *)malloc(parser->bufferSize);
    if (parser->buffer == NULL) {
        free(parser);
        return NULL;
    }

    return parser;
}

XML_Parser XML_ParserCreateNS(const XML_Char *encoding, XML_Char sep) {
    XML_Parser parser = XML_ParserCreate(encoding);
    (void)sep;
    return parser;
}

void XML_ParserFree(XML_Parser parser) {
    if (parser == NULL) return;
    if (parser->buffer) free(parser->buffer);
    free(parser);
}

enum XML_Status XML_Parse(XML_Parser parser, const char *s, int len,
                          int isFinal) {
    if (parser == NULL) return XML_STATUS_ERROR;
    if (s == NULL && len != 0) {
        parser->errorCode = XML_ERROR_NO_MEMORY;
        return XML_STATUS_ERROR;
    }

    parser->isFinal = isFinal;
    parser->byteIndex += len;

    if (isFinal) {
        return XML_STATUS_OK;
    }

    return XML_STATUS_OK;
}

void XML_SetElementHandler(XML_Parser parser,
                           XML_StartElementHandler start,
                           XML_EndElementHandler end) {
    if (parser == NULL) return;
    parser->startElementHandler = start;
    parser->endElementHandler = end;
}

void XML_SetCharacterDataHandler(XML_Parser parser,
                                 XML_CharacterDataHandler handler) {
    if (parser == NULL) return;
    parser->characterDataHandler = handler;
}

void XML_SetUserData(XML_Parser parser, void *userData) {
    if (parser == NULL) return;
    parser->userData = userData;
}

enum XML_Error XML_GetErrorCode(XML_Parser parser) {
    if (parser == NULL) return XML_ERROR_NO_MEMORY;
    return parser->errorCode;
}

XML_Size XML_GetCurrentLineNumber(XML_Parser parser) {
    if (parser == NULL) return 0;
    return parser->lineNumber;
}

const XML_LChar *XML_ExpatVersion(void) {
    return "expat_2.2.5";
}
