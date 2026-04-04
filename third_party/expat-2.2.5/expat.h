/* expat.h - Expat XML parser API
 *
 * Copyright (c) 1998-2000 Thai Open Source Software Center Ltd
 * Copyright (c) 2000-2017 Expat development team
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 */

#ifndef Expat_INCLUDED
#define Expat_INCLUDED 1

#include <stddef.h>

#define XML_MAJOR_VERSION 2
#define XML_MINOR_VERSION 2
#define XML_MICRO_VERSION 5

typedef unsigned char XML_Bool;
#define XML_TRUE   ((XML_Bool) 1)
#define XML_FALSE  ((XML_Bool) 0)

typedef char XML_Char;
typedef char XML_LChar;
typedef long XML_Index;
typedef unsigned long XML_Size;

typedef struct XML_ParserStruct *XML_Parser;

enum XML_Status {
    XML_STATUS_ERROR = 0,
    XML_STATUS_OK = 1,
    XML_STATUS_SUSPENDED = 2
};

enum XML_Error {
    XML_ERROR_NONE,
    XML_ERROR_NO_MEMORY,
    XML_ERROR_SYNTAX,
    XML_ERROR_NO_ELEMENTS,
    XML_ERROR_INVALID_TOKEN,
    XML_ERROR_UNCLOSED_TOKEN,
    XML_ERROR_PARTIAL_CHAR,
    XML_ERROR_TAG_MISMATCH,
    XML_ERROR_DUPLICATE_ATTRIBUTE,
    XML_ERROR_JUNK_AFTER_DOC_ELEMENT,
    XML_ERROR_PARAM_ENTITY_REF,
    XML_ERROR_UNDEFINED_ENTITY,
    XML_ERROR_RECURSIVE_ENTITY_REF,
    XML_ERROR_ASYNC_ENTITY,
    XML_ERROR_BAD_CHAR_REF,
    XML_ERROR_BINARY_ENTITY_REF,
    XML_ERROR_ATTRIBUTE_EXTERNAL_ENTITY_REF,
    XML_ERROR_MISPLACED_XML_PI,
    XML_ERROR_UNKNOWN_ENCODING,
    XML_ERROR_INCORRECT_ENCODING,
    XML_ERROR_UNCLOSED_CDATA_SECTION,
    XML_ERROR_EXTERNAL_ENTITY_HANDLING,
    XML_ERROR_NOT_STANDALONE,
    XML_ERROR_UNEXPECTED_STATE,
    XML_ERROR_ENTITY_DECLARED_IN_PE,
    XML_ERROR_FEATURE_REQUIRES_XML_DTD,
    XML_ERROR_CANT_CHANGE_FEATURE_ONCE_PARSING,
    XML_ERROR_UNBOUND_PREFIX,
    XML_ERROR_UNDECLARING_PREFIX,
    XML_ERROR_INCOMPLETE_PE,
    XML_ERROR_XML_DECL,
    XML_ERROR_TEXT_DECL,
    XML_ERROR_PUBLICID,
    XML_ERROR_SUSPENDED,
    XML_ERROR_NOT_SUSPENDED,
    XML_ERROR_ABORTED,
    XML_ERROR_FINISHED,
    XML_ERROR_SUSPEND_PE
};

typedef void (*XML_StartElementHandler)(void *userData, const XML_Char *name,
                                        const XML_Char **atts);
typedef void (*XML_EndElementHandler)(void *userData, const XML_Char *name);
typedef void (*XML_CharacterDataHandler)(void *userData, const XML_Char *s,
                                         int len);

XML_Parser XML_ParserCreate(const XML_Char *encoding);
XML_Parser XML_ParserCreateNS(const XML_Char *encoding, XML_Char sep);
void XML_ParserFree(XML_Parser parser);
enum XML_Status XML_Parse(XML_Parser parser, const char *s, int len,
                          int isFinal);
void XML_SetElementHandler(XML_Parser parser,
                           XML_StartElementHandler start,
                           XML_EndElementHandler end);
void XML_SetCharacterDataHandler(XML_Parser parser,
                                 XML_CharacterDataHandler handler);
void XML_SetUserData(XML_Parser parser, void *userData);
enum XML_Error XML_GetErrorCode(XML_Parser parser);
const XML_LChar *XML_ErrorString(enum XML_Error code);
XML_Size XML_GetCurrentLineNumber(XML_Parser parser);
XML_Size XML_GetCurrentColumnNumber(XML_Parser parser);
XML_Index XML_GetCurrentByteIndex(XML_Parser parser);
enum XML_Status XML_SetEncoding(XML_Parser parser, const XML_Char *encoding);
const XML_LChar *XML_ExpatVersion(void);

#endif /* Expat_INCLUDED */
