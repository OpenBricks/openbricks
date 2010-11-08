META = $(wildcard packages/*/meta)
FLAVOURS = $(wildcard config/flavours/*/meta)
ARCHS = $(wildcard config/platforms/*/meta)
PLATFORMS = $(wildcard config/platforms/*/*/Kconfig)
MACHINES = $(wildcard config/platforms/*/*/machines/*/meta)
REMOTES = $(wildcard packages/lirc*/config/lircd*)

all: binaries

.stamps/kconfiginit:
	scripts/checkdeps kconfiginit
	scripts/kconfiginit

config oldconfig menuconfig xconfig gconfig: .stamps/kconfiginit build/config/Kconfig.arch build/config/Kconfig.platform build/config/Kconfig.machine build/config/Kconfig.flavours build/config/Kconfig.remote build/config/Kconfig.packages build/config/Kconfig.use
	scripts/checkdeps $@
	$(MAKE) -C build/build.host/bst-kconfig* $@
	scripts/kconfig2options

cleanconfig:
	rm -f build/build.host/bst-kconfig*/.config

build/config/Kconfig.arch: $(ARCHS)
	scripts/archs2kconfig

build/config/Kconfig.platform: $(PLATFORMS)
	scripts/platforms2kconfig

build/config/Kconfig.machine: $(MACHINES)
	scripts/machines2kconfig

build/config/Kconfig.flavours: $(FLAVOURS)
	scripts/flavours2kconfig

build/config/Kconfig.remote: $(REMOTES)
	scripts/remotes2kconfig

build/config/Kconfig.packages: $(META)
	scripts/meta2kconfig

build/config/Kconfig.use: config/use $(FLAVOURS)
	scripts/use2kconfig

binaries: rootfs
	scripts/binaries

rootfs:
	scripts/checkdeps build
	scripts/rootfs

jffs2: rootfs
	scripts/nand jffs2

ubifs: rootfs
	scripts/nand ubifs

doc:
	scripts/checkdeps docs
	make -C DOCS

docs: doc

iso:
	scripts/iso

flat:
	scripts/flat

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
