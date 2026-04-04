/* parser.h - the core XML parser interfaces
 *
 * Copyright (C) 1998-2012 Daniel Veillard. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files, to deal in the Software
 * without restriction.
 */

#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#include <stddef.h>

#define LIBXML_DOTTED_VERSION "2.9.4"
#define LIBXML_VERSION 20904
#define LIBXML_VERSION_STRING "20904"
#define LIBXML_VERSION_EXTRA ""

#define XML_PARSE_RECOVER    (1 << 0)
#define XML_PARSE_NOENT      (1 << 1)
#define XML_PARSE_DTDLOAD    (1 << 2)
#define XML_PARSE_DTDATTR    (1 << 3)
#define XML_PARSE_DTDVALID   (1 << 4)
#define XML_PARSE_NOERROR    (1 << 5)
#define XML_PARSE_NOWARNING  (1 << 6)
#define XML_PARSE_PEDANTIC   (1 << 7)
#define XML_PARSE_NOBLANKS   (1 << 8)
#define XML_PARSE_NONET      (1 << 11)
#define XML_PARSE_NODICT     (1 << 12)
#define XML_PARSE_HUGE       (1 << 19)

typedef unsigned char xmlChar;
typedef struct _xmlDoc xmlDoc;
typedef xmlDoc *xmlDocPtr;
typedef struct _xmlNode xmlNode;
typedef xmlNode *xmlNodePtr;
typedef struct _xmlParserCtxt xmlParserCtxt;
typedef xmlParserCtxt *xmlParserCtxtPtr;
typedef struct _xmlParserInput xmlParserInput;
typedef xmlParserInput *xmlParserInputPtr;
typedef struct _xmlError xmlError;
typedef xmlError *xmlErrorPtr;

typedef enum {
    XML_ERR_NONE = 0,
    XML_ERR_WARNING = 1,
    XML_ERR_ERROR = 2,
    XML_ERR_FATAL = 3
} xmlErrorLevel;

xmlDocPtr xmlReadMemory(const char *buffer, int size, const char *URL,
                        const char *encoding, int options);
xmlDocPtr xmlReadFile(const char *filename, const char *encoding, int options);
xmlDocPtr xmlParseFile(const char *filename);
xmlDocPtr xmlParseMemory(const char *buffer, int size);
xmlDocPtr xmlCtxtReadMemory(xmlParserCtxtPtr ctxt, const char *buffer,
                            int size, const char *URL, const char *encoding,
                            int options);
xmlDocPtr xmlCtxtReadFile(xmlParserCtxtPtr ctxt, const char *filename,
                          const char *encoding, int options);

xmlParserCtxtPtr xmlNewParserCtxt(void);
void xmlFreeParserCtxt(xmlParserCtxtPtr ctxt);
void xmlCtxtReset(xmlParserCtxtPtr ctxt);

xmlNodePtr xmlDocGetRootElement(xmlDocPtr doc);
void xmlFreeDoc(xmlDocPtr cur);
void xmlCleanupParser(void);

void xmlInitParser(void);
int xmlHasFeature(int feature);

const xmlChar *xmlNodeGetContent(xmlNodePtr cur);
xmlChar *xmlGetProp(xmlNodePtr node, const xmlChar *name);
int xmlStrcmp(const xmlChar *str1, const xmlChar *str2);

#endif /* __XML_PARSER_H__ */
