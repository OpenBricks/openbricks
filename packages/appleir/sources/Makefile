KERNEL_VERSION := $(shell uname -r)

MODLIB := $(INSTALL_MOD_PATH)/lib/modules/$(KERNEL_VERSION)
export MODLIB

KDIR := $(MODLIB)/build

PWD := $(shell pwd)

DEPMOD := $(shell which depmod)

SOURCES := appleir.c

obj-m += $(SOURCES:.c=.o)

modules:
	make -C $(KDIR) M=$(PWD)

clean:
	rm -f *.o *.mod.* *.ko Module.symvers
