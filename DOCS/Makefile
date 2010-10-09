DOCS=package-format.txt
HTML=$(DOCS:.txt=.html)
PDF=$(DOCS:.txt=.pdf)
OPTS=-a 'author=The OpenBricks Team' -a 'email=team@openbricks.org'

all: html pdf

html: $(HTML)

pdf: $(PDF)

$(HTML): $(DOCS)
	a2x -f xhtml $(OPTS) $<

$(PDF): $(DOCS)
	a2x -f pdf $(OPTS) $<

clean: clean-html clean-pdf

clean-html:
	rm -f $(HTML) docbook-xsl.css

clean-pdf:
	rm -f $(PDF)

.PHONY: clean clean-html clean-pdf html pdf
