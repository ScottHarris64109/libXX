/*#############################################################################
 * Drawing.cc -- ...
 * Copyright (C) 2026 by Scott Harris.  All rights reserved.
 *
 *#############################################################################
 */
#include <iostream>
#include <stdlib.h>
#include <errno.h>

#include <XX/Window.hh>

#include "Point.hh"
#include "Line.hh"
#include "Palette.h"
#include "Drawing.hh"

//== Constructors =============================================================

Drawing::~Drawing( ) {
}

Drawing::Drawing( ) {
}

//== Accessors =================================================================

void Drawing::setWindow( XX::Window *window ) {
   this->window = window;
}

void Drawing::setPalette( Palette *palette ) {
   this->palette_ = palette;
}


//== Operations ================================================================

void Drawing::startLine( int x, int y ) {
   this->traceLine.setPoint( 1, Point(x,y) );
   this->traceLine.setPoint( 0, this->traceLine.point(1) );
   this->lineStarted = true;
   this->traced = false;
}

void Drawing::traceLineTo( int x, int y ) {
   this->untraceLine();
   this->traceLine.setPoint( 1, Point(x,y) );
   this->window->traceLine( this->palette()->color[this->color],
         this->traceLine.point(0).x(), this->traceLine.point(0).y(), 
         this->traceLine.point(1).x(), this->traceLine.point(1).y() );
   this->traced = true;
}

void Drawing::untraceLine() {
   if (this->traced) {
      this->window->traceLine( this->palette()->color[this->color],
            this->traceLine.point(0).x(), this->traceLine.point(0).y(), 
            this->traceLine.point(1).x(), this->traceLine.point(1).y() );
      this->traced = false;
   }
}

void Drawing::drawLineTo( int x, int y ) {
   this->untraceLine();
   this->traceLine.setPoint( 1, Point(x,y) );
   this->window->drawLine( this->palette()->color[this->color],
         this->traceLine.point(0).x(), this->traceLine.point(0).y(), 
         this->traceLine.point(1).x(), this->traceLine.point(1).y() );
   this->traceLine.setPoint( 0, this->traceLine.point(1) );
}

void Drawing::stopDrawingLines() {
   this->untraceLine();
   this->color = (this->color) ? 0: 1;
   this->lineStarted = false;
}
