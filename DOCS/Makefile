DOCS += openbricks-manual.txt
DOCS += introduction.txt
DOCS += features.txt
DOCS += supported-platforms.txt
DOCS += toolchain-overview.txt
DOCS += build-instructions.txt
DOCS += config-format.txt
DOCS += package-format.txt
DOCS += kconfig-format.txt
DOCS += add-a-new-package.txt
DOCS += add-a-new-platform.txt
DOCS += add-a-new-flavour.txt

HTML=$(DOCS:.txt=.html)
PDF=$(DOCS:.txt=.pdf)
OPTS=-a 'author=The OpenBricks Team' -a 'email=team@openbricks.org'

all: html pdf

html: $(HTML)

pdf: $(PDF)

%.html: %.txt
	asciidoc -b xhtml11 -a data-uri -a icons $(OPTS) $<

%.pdf: %.txt
	a2x -f pdf $(OPTS) $<

clean: clean-html clean-pdf

clean-html:
	rm -f $(HTML) docbook-xsl.css

clean-pdf:
	rm -f $(PDF)

.PHONY: clean clean-html clean-pdf html pdf
