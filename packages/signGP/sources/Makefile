CC ?= gcc
CFLAGS ?= -Wall

SRCS = signGP.c
PROG = signGP

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) $< -o $@

install:
	mkdir -p $(DESTDIR)/usr/bin
	cp -P $(PROG) $(DESTDIR)/usr/bin

clean:
	rm -f $(PROG)
