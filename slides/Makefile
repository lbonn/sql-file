.PHONY: all clean serve

all: slides.html slides_portable.html

clean:
	-$(RM) slides.html slides_portable.html

serve:
	livereload -p 8000

reveal.js/README.md:
	curl -L https://github.com/hakimel/reveal.js/archive/3.8.0.tar.gz | tar xzv --one-top-level=reveal.js --strip-components 1

slides.html: slides.md reveal.js/README.md
	pandoc $< -s --template=template.revealjs -t revealjs --no-highlight --css=style/sunblind.css -V highlight --css=style/highlight.css -o $@

slides_portable.html: slides.md reveal.js/README.md
	pandoc $< -s --self-contained --template=template.revealjs -t revealjs --no-highlight --css=style/sunblind.css -V highlight --css=style/highlight.css -o $@
