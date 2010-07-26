all: iso

.stamps/kconfiginit:
	scripts/kconfiginit

config: .stamps/kconfiginit
	$(MAKE) -C build/bst-kconfig* $@
	scripts/kconfig2options

%config: .stamps/kconfiginit
	$(MAKE) -C build/bst-kconfig* $@
	scripts/kconfig2options

iso:
	scripts/iso

flat:
	scripts/flat

burn: iso
	scripts/burn

dist:
	scripts/dist

generator:
	scripts/generator

pxe:
	scripts/pxe

vmx:
	scripts/vmx build

vmx-play:
	scripts/vmx play

qemu: iso
	scripts/qemu

sum: iso generator dist fulldist
	scripts/sum md5sum
	scripts/sum sha1sum

get:
	scripts/get

clean:
	scripts/clean

distclean:
	rm -rf .stamps build* sources geexbox* config/options


.PHONY: iso burn dist fulldist generator installator exec clean distclean
