EXTRA_SRC =
TEX = pdflatex

all: Final.pdf Interim.pdf

.phony: all clean

%.pdf: %.tex $(EXTRA_SRC)
	while( $(TEX) $< ; grep -q "Rerun to get cross" $(<:.tex=.log)) ; \
	  do true ; \
	 done

clean:
	rm -f *.aux
	rm -f *.log
