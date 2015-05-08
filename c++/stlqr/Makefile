# Hook to Echo Makefile Variable
ifneq ($(emv),)
emv:
	@echo $($(emv))
endif

# We are not interested in failures
.DELETE_ON_ERROR:

# Code extractions:
CODEEXT = \
	eqrange.exm \
	vector.exm \
	map.exm \
	unitroot.exm \
	splicehelp.exm \
	spliceexample.exm \
	sorthelp.exm \
	sortexample.exm \
	numerichelp.exm \
	numericexample.exm \
	istriter.exm \
	interfermat.exm \
	fermat.exm \
	itertraitshelp.exm \
	itertraits.exm

CODEOUT = \
	eqrange.exout \
	map.exout \
	spliceexample.exout \
	sortexample.exout \
	numericexample.exout \
	uroot.exout \
	fermat.exout \
	itertraits.exout

default: stlqr-a4.dvi

clean:
	rm -f \
	   stlqr-a4.dvi stlqr-a4.ps stlqr-a4.ps.gz stlqr-a4.pdf \
	   stlqr-letter.dvi stlqr-letter.ps stlqr-letter.ps.gz stlqr-letter.pdf \
	   examples $(CODEEXT) $(CODEOUT)

eqrange.exm: examples.cc ${PATSEG}
	${XPATSEG} eqrange_example < $< > $@.new
	mv $@.new $@

vector.exm: examples.cc ${PATSEG}
	${XPATSEG} vector-examples < $< > $@.new
	mv $@.new $@

map.exm: examples.cc ${PATSEG}
	${XPATSEG} map_example < $< > $@.new
	mv $@.new $@

unitroot.exm: examples.cc ${PATSEG}
	${XPATSEG} 'Unit Roots' < $< > $@.new
	mv $@.new $@

istriter.exm: examples.cc ${PATSEG}
	${XPATSEG} 'streamIter_example' < $< > $@.new
	mv $@.new $@

interfermat.exm: examples.cc ${PATSEG}
	${XPATSEG} 'Interator Fermat' < $< > $@.new
	mv $@.new $@

fermat.exm: examples.cc ${PATSEG}
	${XPATSEG} fermat_example < $< > $@.new
	mv $@.new $@

numericexample.exm: examples.cc ${PATSEG}
	${XPATSEG} numeric_example < $< > $@.new
	mv $@.new $@

numerichelp.exm: examples.cc ${PATSEG}
	${XPATSEG} numeric_help < $< > $@.new
	mv $@.new $@

sortexample.exm: examples.cc ${PATSEG}
	${XPATSEG} sort_example < $< > $@

spliceexample.exm: examples.cc ${PATSEG}
	${XPATSEG} splice_example < $< > $@.new
	mv $@.new $@

itertraitshelp.exm: examples.cc ${PATSEG}
	${XPATSEG}  "Iterator Traits" < $< > $@

itertraits.exm: examples.cc ${PATSEG}
	${XPATSEG}  iterator_traits_example < $< > $@


%.exm: examples.cc ${PATSEG}
	${XPATSEG} $(*F) < $< > $@.new
	mv $@.new $@

%.exout: examples ${PATSEG}
	./examples | ${XPATSEG} $(*F) > $@.new
	mv $@.new $@

exo:  $(CODEOUT)
	@echo CODEOUT=$(CODEOUT)

# Stolen from Noweb examples
%.dvi: %.tex
	latex '\scrollmode \input '"$*"; \
	while grep -s 'Rerun to get cross-references right' $*.log; \
	do latex '\scrollmode \input '"$*"; done

PAPER = a4
# PAPER = letter

stlqr-letter.dvi stlqr-a4.dvi \
stlqr-letter.pdf stlqr-a4.pdf : stlcard.tex $(CODEEXT) $(CODEOUT) ver


# force latex run (for cross reference)
.PHONY: redvi
redvi:
	rm -f stlqr-$(PAPER).dvi
	latex -interaction=errorstopmode stlqr-$(PAPER).tex < /dev/null

# OFFSET = -1.2cm,-1.8cm
OFFSET = 0.cm,0.cm

DVIPSDPI = -D600
# DVIPSPAPER = -t $(PAPER) -t landscape
# I once used a smart construct:
#   `echo $@ | cut -d- -f2 | cut -d. -f1`
# to extarct the page-type from the target pattern. But simpler is better.

DVIPSOFFSET_A4     = 0.0cm,0.2cm
DVIPSOFFSET_LETTER = 0.0in,0.1in
DVIPSPAR = $(DVIPSDPI) -t landscape
stlqr-a4.ps: stlqr-a4.dvi
	dvips $(DVIPSPAR) -t a4     -O $(DVIPSOFFSET_A4)      $(DVIPS_EXTRA) \
	    -o $@ $<

stlqr-letter.ps: stlqr-letter.dvi
	dvips $(DVIPSPAR) -t letter -O $(DVIPSOFFSET_LETTER)  $(DVIPS_EXTRA) \
	    -o $@ $<

%.gz: %
	gzip -c -9 $< > $@


# Rotating even side pages upside down.
# This helps to foll printer that do long folding, when we need short folding.
# Use -d to have boundary-rectangle for debug
stlqr-a4-r2.ps: stlqr-a4.ps
	pstops -pa4 '2:0,1U@1.0(21.cm,29.7cm)' $< $@

stlqr-letter-r2.ps: stlqr-letter.ps
	pstops -pletter '2:0,1U@1.0(8.5in,11.0in)' $< $@

# Odd even
psoe: stlcard.dvi
	dvips -A -t landscape -O $(OFFSET) -o sca.ps $<
	dvips -B -t landscape -O $(OFFSET) -o scb.ps $<

# Stolen from Noweb examples
# But does not work well...
%.pdf: %.tex
	pdflatex '\scrollmode \input '"$*"; \
	while grep -s 'Rerun to get cross-references right' $*.log; \
	do pdflatex '\scrollmode \input '"$*"; done

pdfs: stlqr-letter.pdf stlqr-a4.pdf 

examples: examples.cc
	g++ -O2 -o examples examples.cc

VER := $(shell cat ver)
PUBTGZ = stlqr-$(VER).tar.gz
MSVER = $(shell echo $(VER) | sed s+\\.++)
PUBZIP = stlqr$(MSVER).zip
PATSEG = patseg.py
XPATSEG = ./patseg.py
pub: $(PUBTGZ)

# PUBFILES = stlqr-150.ps examples.cc
SRC_CONTROLABLE = \
	README \
	ChangeLog \
	stlqr-letter.tex stlqr-a4.tex stlcard.tex \
	examples.cc Makefile ver ${PATSEG}

PUBFILES = $(SRC_CONTROLABLE)
# PUBFILES += stlqr-letter.ps stlqr-a4.ps
# PUBFILES += stlqr-letter.pdf stlqr-a4.pdf
$(PUBTGZ): $(PUBFILES)
	tar cvf - $(PUBFILES) | gzip -9 > $@
	gzip -cd $@ | tar tvf -
	ls -l $(PUBTGZ)

pubtgz: $(PUBTGZ)

$(PUBZIP): $(PUBFILES)
	rm -f $@
	zip -l $(PUBZIP) $(PUBFILES)
	ls -l $(PUBZIP)
	unzip -v $(PUBZIP)

pubzip: $(PUBZIP)

RELEASE_FILES = \
	${PUBTGZ} \
	stlqr-a4.ps.gz stlqr-letter.ps.gz \
	stlqr-a4.pdf stlqr-letter.pdf

release: ${RELEASE_FILES}
	ls -l ${RELEASE_FILES}


.PHONY: checkin
checkin:
	ci -l $(SRC_CONTROLABLE)

rcsdiffs:
	rcsdiff $(SRC_CONTROLABLE)

# For this Makefile debug
ev:
	@echo $(EV)=$($(EV))


#####################################################
#                 For home use only                 #
#####################################################

hashex: hashex.cc
	rm -f $@
	g++ -g -I. -o $@.new $<
	mv $@.new $@

stlqrlj4.ps: stlcard.dvi
	dvips -mode ljfour -D600  -t landscape -O $(OFFSET) $(EXTRA) -o $@ $<

stlqrlj5.ps: stlcard.dvi
	dvips -mode ljfive -D600  -t landscape -O $(OFFSET) -o $@ $<

stlqr-100.ps: stlcard.dvi
	dvips -D100 -t landscape -O $(OFFSET) -o $@ $<

stlqr-120.ps: stlcard.dvi
	dvips -mode onetz -D120 -t landscape -O $(OFFSET) -o $@ $<

stlqr-150.ps: stlcard.dvi
	dvips -mode ljlo -D150 -t landscape -O $(OFFSET) -o $@ $<


stlqr-p7.ps: stlcard.dvi
	dvips -D600 -p 7 -t landscape -O $(OFFSET) -o $@ $<

stlqr-p8.ps: stlcard.dvi
	dvips -D600 -p 8 -t landscape -O $(OFFSET) -o $@ $<

psoe100: stlcard.dvi
	dvips -A -D100 -t landscape -O $(OFFSET) -o sca100.ps $<
	dvips -B -D100 -t landscape -O $(OFFSET) -o scb100.ps $<

