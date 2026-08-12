###############################################################################
# Makefile for the XX class library.
# Copyright (C) 2014,2026 Scott Harris.  All rights reserved.
###############################################################################

include ../make.rules

LIBRARY=libXX.a
HTMLDIR=$(DOCDIR)/libXX
DOCUMENT=doc/html/index.html

HEADERS= XXDisplay.hh XXWindow.hh XXPixMap.hh XXColor.hh XXGC.hh XXFont.hh 
MODULES= XXDisplay.cc XXWindow.cc XXPixMap.cc XXColor.cc XXGC.cc XXFont.cc
OBJECTS= XXDisplay.o  XXWindow.o  XXPixMap.o  XXColor.o  XXGC.o  XXFont.o  

$(LIBRARY): $(LIBRARY)( $(OBJECTS) )

all:  $(HEADERS) $(LIBRARY) $(DOCUMENT)

doc:  $(DOCUMENT)

$(DOCUMENT):
	doxygen doxygen.conf

clean:
	rm -rf $(LIBRARY) $(OBJECTS) doc

install:
	cp $(HEADERS)   $(INCDIR)
	cp $(LIBRARY)   $(LIBDIR)
	mkdir -p $(HTMLDIR)
	cp -R doc/html $(HTMLDIR)
	cp -R doc/man/* $(MANDIR)
