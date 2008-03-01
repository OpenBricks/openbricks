iso:
	scripts/iso

burn: iso
	scripts/burn

dist:
	scripts/dist

fulldist:
	scripts/dist full

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
	rm -rf .stamps build.* sources geexbox*


.PHONY: iso burn dist fulldist generator installator exec clean distclean
