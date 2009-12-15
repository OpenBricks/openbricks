CC ?= gcc
CFLAGS ?= -Wall -g

UDEV_CFLAGS        = $(shell pkg-config --cflags libudev)
UDEV_LDFLAGS       = $(shell pkg-config --libs libudev)

CFLAGS  += $(UDEV_CFLAGS)
LDFLAGS += $(UDEV_LDFLAGS)

SRCS = automountd.c
PROG = automountd

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f $(PROG)
