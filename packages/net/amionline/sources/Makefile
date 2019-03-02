CC ?= gcc
CFLAGS ?= -Wall -g

DEPS_CFLAGS  = $(shell pkg-config --cflags dbus-1)
DEPS_LDFLAGS = $(shell pkg-config --libs dbus-1)

CFLAGS  += $(DEPS_CFLAGS)
LDFLAGS += $(DEPS_LDFLAGS)

SRCS = amionline.c
PROG = amionline

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

install:
	mkdir -p $(DESTDIR)/usr/bin
	cp -P $(PROG) $(DESTDIR)/usr/bin

clean:
	rm -f $(PROG)
