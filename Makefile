META = $(shell ls packages/*/meta)

all: iso

.stamps/kconfiginit:
	scripts/kconfiginit

config: .stamps/kconfiginit config/Kconfig.packages
	$(MAKE) -C build.host/bst-kconfig* $@
	scripts/kconfig2options

%config: .stamps/kconfiginit config/Kconfig.packages
	$(MAKE) -C build.host/bst-kconfig* $@
	scripts/kconfig2options

config/Kconfig.packages: $(META)
	scripts/meta2kconfig

doc:
	scripts/checkdeps docs
	make -C DOCS

docs: doc

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

test:
	scripts/test

clean-doc:
	make -C DOCS clean

clean: clean-doc
	scripts/clean

distclean: clean-doc
	rm -rf .stamps build* sources geexbox* config/options


.PHONY: iso burn dist fulldist generator installator exec clean distclean clean-doc doc docs
