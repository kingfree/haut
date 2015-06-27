MAIN = os

all:
	xelatex $(MAIN)
	bibtex $(MAIN)
	xelatex $(MAIN)

clean:
	-$(RM) *.aux *.log *.out *.toc *.bbl *.idx *.blg
	-$(RM) source/*.aux
