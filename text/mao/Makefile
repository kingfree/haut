MAIN = mao

all:
	xelatex $(MAIN)
	bibtex $(MAIN)
	xelatex $(MAIN)

clean:
	rm *.aux *.log *.out *.toc *.bbl *.idx *.blg