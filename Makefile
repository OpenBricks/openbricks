META = $(wildcard packages/*/meta)
FLAVOURS = $(wildcard config/flavours/*/meta)
PLATFORMS = $(wildcard config/platforms/*/*/Kconfig)
REMOTES = $(wildcard packages/lirc*/config/lircd*)

all: flat

.stamps/kconfiginit:
	scripts/checkdeps kconfiginit
	scripts/kconfiginit

config oldconfig menuconfig xconfig gconfig: .stamps/kconfiginit build/config/Kconfig.platform build/config/Kconfig.flavours build/config/Kconfig.remote build/config/Kconfig.packages build/config/Kconfig.use
	scripts/checkdeps $@
	$(MAKE) -C build/build.host/bst-kconfig* $@
	scripts/kconfig2options

cleanconfig:
	rm -f build/build.host/bst-kconfig*/.config

build/config/Kconfig.platform: $(PLATFORMS)
	scripts/platforms2kconfig

build/config/Kconfig.flavours: $(FLAVOURS)
	scripts/flavours2kconfig

build/config/Kconfig.remote: $(REMOTES)
	scripts/remotes2kconfig

build/config/Kconfig.packages: $(META)
	scripts/meta2kconfig

build/config/Kconfig.use: config/use $(FLAVOURS)
	scripts/use2kconfig

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
	rm -rf .stamps build* sources geexbox* config/options binaries


.PHONY: iso burn dist fulldist generator installator exec clean distclean clean-doc doc docs config oldconfig menuconfig xconfig gconfig
