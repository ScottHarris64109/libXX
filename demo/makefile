###############################################################################
# Makefile for XXlib demo.
# Copyright (C) 2014,2015,2026 Scott Harris.  All rights reserved.
###############################################################################

include ~/git/make.rules

TARGET= demo
OBJECTS= demo.o DemoEventHandler.o
SOURCES= demo.cc DemoEventHandler.cc
LIBS=  -lXX -lX11

$(TARGET): $(SOURCES)
	$(CXX) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS) $(LIBS)

all:  $(TARGET)

clean:
	rm -f $(TARGET)
