CC ?= gcc
CFLAGS ?= -Wall

SRCS = signGP.c
PROG = signGP

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROG)
