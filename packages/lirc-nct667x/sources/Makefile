KSRC ?= /lib/modules/$(shell uname -r)/build
obj-m += lirc_wb677.o

all:
	$(MAKE) -C $(KSRC) M=$(PWD) modules

clean:
	$(MAKE) -C $(KSRC) M=$(PWD) clean
