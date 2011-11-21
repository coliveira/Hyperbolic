# Makefile for C projects
# by Carlos A.S. Oliveira, 2003
# University of Florida
PROJNAME = hyperb
OBJS = grasp.o debug.o parsecml.o timer.o
CFLAGS = -g -Wall
DOCSRC = grasp.c.w
DOCHDR = 
DOCFILES = $(DOCHDR) $(DOCSRC)
DOCTMPFILES= *.w *.pdf $(PROJNAME).tex *.idx *.log *.scn *.toc *.dvi 
TEX = latex

all: $(PROJNAME)

$(PROJNAME): $(OBJS)
	$(CC) -o $(PROJNAME) $(OBJS)
	
clean:
	rm -f *.o *~ *.exe *dump *.a $(DOCTMPFILES) $(PROJNAME)

# 
# The commands bellow are taken from CWEB, created by Knuth and Levy
#

# uncomment the second line if you use pdftex to bypass .dvi files
PDFTEX = dvipdf
#PDFTEX = pdftex

CWEBPREP= ./cwebprep

CWEAVE = cweave

.SUFFIXES: .dvi .tex .w .c .pdf 

# this is needed to cancel old rule about ctangle
%.c : %.w

.w.tex:
	$(CWEAVE) $*

%.c.w : %.c
	$(CWEBPREP) $*.c

.w.pdf:
	make $*.tex ; \
	case "$(PDFTEX)" in \
	 dvipdf | dvipdfm ) $(TEX) "\let\pdf+ \input $*"; $(PDFTEX) $* ;; \
	 pdftex ) pdftex $* ;; \
	esac

%.h.w : %.h
	$(CWEBPREP) $*.h

perm:
	chmod u+x ./cwebprep

links:
	ln -sf ~/misc/TeXInputs/limbo.tex .
	ln -sf ~/misc/TeXInputs/cweb-hy.cls .
	ln -sf ~/misc/TeXInputs/keyvald.sty .
	ln -sf ~/misc/TeXInputs/cwbasehy.tex .

doc: alldocs

#if [ -f limbo.tex ] ; then echo "\input limbo.tex" ; fi; \

alldocs :  $(DOCFILES)
	( \
	echo "\input limbo.tex" ; \
	if [ "$(SECTION)x" = "yesx" ] ; then echo "@** Interface files." ; fi ; \
	if [ ! "$(DOCHDR)x" = "x" ] ; then for i in $(DOCHDR) ; do \
		echo "@i " $$i ; \
	done ; fi; \
	if [ "$(SECTION)x" = "yesx" ] ; then echo "@** Implementation files." ; fi;   \
	if [ ! "$(DOCSRC)x" = "x" ] ; then for i in $(DOCSRC) ; do \
		echo "@i " $$i ; \
	done ; fi; \
	echo "@  \end{document}"  ) \
	>  $(PROJNAME).w; \
	make $(PROJNAME).pdf
	
doc-interface: $(DOCHDR)
	( \
	if [ -f limbo.tex ] ; then echo "\input limbo.tex" ; fi; \
	echo "@** Interface files." ; \
	for i in $(DOCHDR) ; do \
		echo "@i " $$i ; \
	done ; \
	echo "@** The index."  ) \
	>  $(PROJNAME)-interf.w; \
	make $(PROJNAME)-interf.pdf

doc-imp: $(DOCSRC)
	( \
	if [ -f limbo.tex ] ; then echo "\input limbo.tex" ; fi; \
	echo "@** Implementation files." ; \
	for i in $(DOCSRC) ; do \
		echo "@i " $$i ; \
	done ; \
	echo "@** The index."  ) \
	>  $(PROJNAME)-imp.w; \
	make $(PROJNAME)-imp.pdf
