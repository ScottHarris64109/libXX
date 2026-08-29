###############################################################################
# Makefile for the XX class library.
# Copyright (C) 2014,2026 Scott Harris.  All rights reserved.
###############################################################################

include ../make.rules

LIBRARY=libXX.a
LIB_INCDIR=$(INCDIR)/XX
HTMLDIR=$(DOCDIR)/libXX
DOCUMENT=doc/html/index.html

HEADERS= Display.hh Screen.hh Drawable.hh Window.hh PixMap.hh \
   Color.hh Font.hh 
MODULES= Display.cc Screen.cc Drawable.cc Window.cc PixMap.cc \
   Color.cc Font.cc
OBJECTS= Display.o  Screen.o  Drawable.o  Window.o  PixMap.o  \
   Color.o  Font.o  

$(LIBRARY): $(LIBRARY)( $(OBJECTS) )

all:  $(HEADERS) $(LIBRARY) $(DOCUMENT)

doc:  $(DOCUMENT)

$(DOCUMENT):
	doxygen doxygen.conf

clean:
	rm -rf $(LIBRARY) $(OBJECTS) doc

install:
	mkdir -p $(LIB_INCDIR)
	cp $(HEADERS)   $(LIB_INCDIR)
	cp $(LIBRARY)   $(LIBDIR)
	mkdir -p $(HTMLDIR)
	cp -R doc/html $(HTMLDIR)
	cp -R doc/man/* $(MANDIR)
