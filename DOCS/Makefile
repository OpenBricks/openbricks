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
DOCS += add-a-new-architecture.txt
DOCS += add-a-new-platform.txt
DOCS += add-a-new-machine.txt
DOCS += add-a-new-flavour.txt

AUTHOR = 'author=The OpenBricks Team'
EMAIL  = 'email=team@openbricks.org'

HTML=$(DOCS:.txt=.html)
PDF=openbricks-manual.pdf
OPTS=-a $(AUTHOR) -a $(EMAIL)

all: html pdf

html: $(HTML)

pdf: $(PDF)

openbricks-manual.html: $(DOCS)
openbricks-manual.pdf: $(DOCS)

%.html: %.txt
	asciidoc -b xhtml11 -a toc -a numbered -a data-uri -a icons $(OPTS) $<

%.pdf: %.txt
	a2x -f pdf -d book --dblatex-opts "-P latex.output.revhistory=0" $(OPTS) $<

clean: clean-html clean-pdf

clean-html:
	rm -f $(HTML) docbook-xsl.css

clean-pdf:
	rm -f $(PDF)

.PHONY: clean clean-html clean-pdf html pdf
