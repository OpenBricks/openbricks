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

installator:
	scripts/installator

pxe:
	scripts/pxe

exec: iso
	scripts/exec

md5sum: iso generator dist fulldist
	scripts/sum md5sum

sha1sum: iso generator dist fulldist
	scripts/sum sha1sum

clean:
	scripts/clean

distclean:
	rm -rf .stamps build sources geexbox*


.PHONY: iso burn dist fulldist generator installator exec clean distclean
