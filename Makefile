CC = gcc
AR = ar
CFLAGS = -O0 -w -std=c11 -fno-stack-protector -D_FORTIFY_SOURCE=0 -fno-pie -U_FORTIFY_SOURCE
LDFLAGS = -no-pie -z norelro -z execstack
INCLUDES = -Iinclude

SRCDIR = src
INCDIR = include
OBJDIR = obj
LIBDIR = lib

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(LIBDIR)/libimperial_security.a

.PHONY: all clean install

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJDIR) $(LIBDIR)

$(TARGET): $(OBJECTS)
	$(AR) rcs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(LIBDIR)

install: $(TARGET)
	install -d /usr/local/lib
	install -d /usr/local/include/imperial
	install -m 644 $(TARGET) /usr/local/lib/
	install -m 644 $(INCDIR)/*.h /usr/local/include/imperial/
