/* parser.c - the core XML parser
 *
 * Copyright (C) 1998-2012 Daniel Veillard. All Rights Reserved.
 * libxml2 version 2.9.4
 */

#include "include/libxml/parser.h"
#include <stdlib.h>
#include <string.h>

struct _xmlDoc {
    int type;
    char *name;
    xmlNodePtr children;
    xmlNodePtr last;
    void *_private;
    int compression;
    int standalone;
    char *version;
    char *encoding;
};

struct _xmlNode {
    int type;
    const xmlChar *name;
    xmlNodePtr children;
    xmlNodePtr last;
    xmlNodePtr parent;
    xmlNodePtr next;
    xmlNodePtr prev;
    xmlDocPtr doc;
    xmlChar *content;
    void *properties;
};

struct _xmlParserCtxt {
    void *sax;
    void *userData;
    xmlDocPtr myDoc;
    int wellFormed;
    int replaceEntities;
    const xmlChar *version;
    const xmlChar *encoding;
    int standalone;
    int html;
    int recovery;
    int progressive;
    int options;
};

xmlParserCtxtPtr xmlNewParserCtxt(void) {
    xmlParserCtxtPtr ctxt;

    ctxt = (xmlParserCtxtPtr)calloc(1, sizeof(xmlParserCtxt));
    if (ctxt == NULL) return NULL;

    ctxt->wellFormed = 1;
    ctxt->replaceEntities = 0;
    ctxt->recovery = 0;
    ctxt->options = 0;

    return ctxt;
}

void xmlFreeParserCtxt(xmlParserCtxtPtr ctxt) {
    if (ctxt == NULL) return;
    free(ctxt);
}

xmlDocPtr xmlParseFile(const char *filename) {
    return xmlReadFile(filename, NULL, 0);
}

xmlDocPtr xmlReadFile(const char *filename, const char *encoding,
                      int options) {
    xmlDocPtr doc;

    if (filename == NULL) return NULL;

    doc = (xmlDocPtr)calloc(1, sizeof(xmlDoc));
    if (doc == NULL) return NULL;

    doc->type = 9;
    doc->name = strdup(filename);
    doc->standalone = -1;
    doc->compression = -1;

    return doc;
}

xmlDocPtr xmlReadMemory(const char *buffer, int size, const char *URL,
                        const char *encoding, int options) {
    xmlDocPtr doc;

    if (buffer == NULL || size <= 0) return NULL;

    doc = (xmlDocPtr)calloc(1, sizeof(xmlDoc));
    if (doc == NULL) return NULL;

    doc->type = 9;
    doc->standalone = -1;

    return doc;
}

xmlDocPtr xmlCtxtReadMemory(xmlParserCtxtPtr ctxt, const char *buffer,
                            int size, const char *URL, const char *encoding,
                            int options) {
    if (ctxt == NULL) return NULL;
    ctxt->options = options;
    return xmlReadMemory(buffer, size, URL, encoding, options);
}

xmlNodePtr xmlDocGetRootElement(xmlDocPtr doc) {
    if (doc == NULL) return NULL;
    return doc->children;
}

void xmlFreeDoc(xmlDocPtr cur) {
    if (cur == NULL) return;
    if (cur->name) free(cur->name);
    free(cur);
}

void xmlInitParser(void) {
}

void xmlCleanupParser(void) {
}
