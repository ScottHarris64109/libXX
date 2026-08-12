/*#############################################################################
 * XXGC.cc -- Wrapper for an X11 GC (Graphics Context).
 *
 *  Copyright (C) 2012,2015,2026 by Scott Harris.  All rights reserved.
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library.  If not, see <https://www.gnu.org/licenses/>.
 *#############################################################################
 */
#include <string>
#include <stdexcept>
//#include <iostream>
//#include <stdlib.h>
//#include <errno.h>

#include "XXDisplay.hh"
#include "XXWindow.hh"
#include "XXPixMap.hh"
#include "XXColor.hh"
#include "XXGC.hh"
#include "XXFont.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a XXGC object.
 */
XXGC::~XXGC( ) {
   XFreeGC( window->getDisplay()->xDisplay(), gc );
}

/**
 * Create an XXGC for an XXWindow.
 */
XXGC::XXGC( XXWindow *window ) : window( window ) {
   pixmap = NULL;
   target = window->getXID();
   gc = create();
}

XXGC::XXGC( XXPixMap *pixmap ) : pixmap( pixmap ) {
   window = pixmap->getWindow();
   target = pixmap->getXPixmap();
   gc = create();
}

GC XXGC::create( void ) {
   XGCValues settings;
   unsigned long mask = 0;

   gc = XCreateGC( window->getDisplay()->xDisplay(), target, 
         mask, &settings );

   //TODO:  if (gc == 0) ...

   setBackground( window->getBackground() );
   return gc;
}

//== Accessors ================================================================

void XXGC::setForeground( XXColor *color ) {
   XSetForeground( window->getDisplay()->xDisplay(), gc, color->getPixel() );
}

void XXGC::setBackground( XXColor *color ) {
   XSetBackground( window->getDisplay()->xDisplay(), gc, color->getPixel() );
}

void XXGC::setFont( XXFont *font ) {
   XSetFont( window->getDisplay()->xDisplay(), gc, font->getXFont() );
}

//== Operations ===============================================================

void XXGC::drawPoint( int x, int y ) {
   XDrawPoint( window->getDisplay()->xDisplay(), target, gc, x, y );
}

void XXGC::drawLine( int x1, int y1, int x2, int y2 ) {
   XDrawLine( window->getDisplay()->xDisplay(), target, gc, 
         x1, y1, x2, y2 );
}

void XXGC::drawRectangle( int x, int y, int width, int height ) {
   XDrawRectangle( window->getDisplay()->xDisplay(), target, gc, 
         x, y, width, height );
}

void XXGC::fillRectangle( int x, int y, int width, int height ) {
   XFillRectangle( window->getDisplay()->xDisplay(), target, gc, 
         x, y, width, height );
}

void XXGC::drawArc( int x, int y, int width, int height, 
      double start, double sweep ) {
   XDrawArc( window->getDisplay()->xDisplay(), target, gc, 
         x, y, width, height, angle( start ), angle( sweep ) );
}

void XXGC::fillArc( int x, int y, int width, int height, 
      double start, double sweep ) {
   XFillArc( window->getDisplay()->xDisplay(), target, gc, 
         x, y, width, height, angle( start ), angle( sweep ) );
}

void XXGC::drawText( int x, int y, const string text ) {
   XDrawString( window->getDisplay()->xDisplay(), target, gc, x, y,
        text.c_str(), text.length() );
}

void XXGC::fillText( int x, int y, const string text ) {
   XDrawImageString( window->getDisplay()->xDisplay(), target, gc, x, y,
        text.c_str(), text.length() );
}

