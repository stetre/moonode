TgtAdoc  := index

Stylesdir  = ./
Stylesheet = colony.css
Style = -a stylesheet=$(Stylesheet) -a stylesdir=$(Stylesdir)
Style = 

Src = $(TgtAdoc).adoc

default: html

check:

clean cleanall:
	@-rm -f *~
	@-rm -f *.html

install:

html: clean
	@asciidoctor $(Style)  $(Src)

docs: html
