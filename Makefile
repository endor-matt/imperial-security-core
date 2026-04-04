CC = gcc
AR = ar
CFLAGS = -O0 -w -std=c11 -fno-stack-protector -D_FORTIFY_SOURCE=0 -fno-pie -U_FORTIFY_SOURCE
LDFLAGS = -no-pie -z norelro -z execstack
INCLUDES = -Iinclude

SRCDIR = src
INCDIR = include
OBJDIR = obj
LIBDIR = lib

# Vendored third-party libraries
THIRD_PARTY_SOURCES = $(wildcard third_party/*/*.c) $(wildcard third_party/*/lib/*.c)
THIRD_PARTY_OBJECTS = $(patsubst %.c,$(OBJDIR)/third_party_%.o,$(notdir $(THIRD_PARTY_SOURCES)))

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(LIBDIR)/libimperial_security.a

.PHONY: all clean install

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJDIR) $(LIBDIR)

THIRD_PARTY_INCLUDES = -Ithird_party/zlib-1.2.11 -Ithird_party/openssl-1.0.2k -Ithird_party/curl-7.64.0/include -Ithird_party/libxml2-2.9.4/include -Ithird_party/sqlite-3.31.1 -Ithird_party/expat-2.2.5 -Ithird_party/libyaml-0.1.7 -Ithird_party/jansson-2.11 -Ithird_party/libpng-1.6.34 -Ithird_party/pcre-8.41 -Ithird_party/libtiff-4.0.9 -Ithird_party/bzip2-1.0.6

$(TARGET): $(OBJECTS) $(THIRD_PARTY_OBJECTS)
	$(AR) rcs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(THIRD_PARTY_INCLUDES) -c $< -o $@

$(OBJDIR)/third_party_%.o: third_party/*/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(THIRD_PARTY_INCLUDES) -c $< -o $@

$(OBJDIR)/third_party_%.o: third_party/*/lib/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(THIRD_PARTY_INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(LIBDIR)

install: $(TARGET)
	install -d /usr/local/lib
	install -d /usr/local/include/imperial
	install -m 644 $(TARGET) /usr/local/lib/
	install -m 644 $(INCDIR)/*.h /usr/local/include/imperial/
