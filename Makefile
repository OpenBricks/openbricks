META = $(wildcard packages/*/meta)
FLAVOURS = $(wildcard config/flavours/*/meta)
ARCHS = $(wildcard config/platforms/*/meta)
PLATFORMS = $(wildcard config/platforms/*/*/meta)
MACHINES = $(wildcard config/platforms/*/*/machines/*/meta)
SCRIPTS = $(wildcard scripts/*2kconfig)

all: binaries

.stamps/kconfiginit:
	scripts/kconfiginit

config silentoldconfig oldconfig menuconfig xconfig gconfig: .stamps/kconfiginit build/config/Kconfig.version build/config/Kconfig.arch build/config/Kconfig.platform build/config/Kconfig.machine build/config/Kconfig.flavours build/config/Kconfig.packages build/config/Kconfig.use
	scripts/checkdeps $@
	-$(MAKE) -C build/build.host/bst-kconfig* $@
	scripts/kconfig2options

%_defconfig:
	scripts/loadcfg $*

cleanconfig:
	rm -f build/build.host/bst-kconfig*/.config

build/config/Kconfig.version: $(SCRIPTS) VERSION
	scripts/version2kconfig

build/config/Kconfig.arch: $(SCRIPTS) $(ARCHS)
	scripts/archs2kconfig

build/config/Kconfig.platform: $(SCRIPTS) $(PLATFORMS)
	scripts/platforms2kconfig

build/config/Kconfig.machine: $(SCRIPTS) $(MACHINES)
	scripts/machines2kconfig

build/config/Kconfig.flavours: $(SCRIPTS) $(FLAVOURS)
	scripts/flavours2kconfig

build/config/Kconfig.packages: $(SCRIPTS) config/use $(META)
	scripts/meta2kconfig

build/config/Kconfig.use: $(SCRIPTS) config/use $(FLAVOURS)
	scripts/use2kconfig

binaries: rootfs
	scripts/binaries

rootfs:
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
	scripts/checkdeps get
	scripts/get

test:
	scripts/test

savecfg:
	scripts/savecfg

clean-doc:
	make -C DOCS clean

clean: clean-doc
	scripts/clean

quickclean: clean-doc
	scripts/clean --quick

distclean: clean-doc
	rm -rf .stamps build* sources geexbox* binaries


.PHONY: iso burn dist fulldist generator installator exec clean distclean clean-doc doc docs config %_defconfig oldconfig menuconfig xconfig gconfig
