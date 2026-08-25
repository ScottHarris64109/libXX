/*#############################################################################
 * XX::GC.cc -- Wrapper for an X11 GC (Graphics Context).
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

#include "Display.hh"
#include "Window.hh"
#include "PixMap.hh"
#include "Color.hh"
#include "GC.hh"
#include "Font.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a GC object.
 */
XX::GC::~GC( ) {
   XFreeGC( window->display()->xDisplay(), xgc );
}

/**
 * Create an GC for an Window.
 */
XX::GC::GC( XX::Window *window ) : window( window ) {
   pixmap = NULL;
   target = window->getXID();
   xgc = create();
}

XX::GC::GC( XX::PixMap *pixmap ) : pixmap( pixmap ) {
   window = pixmap->getWindow();
   target = pixmap->getXPixmap();
   xgc = create();
}

::GC XX::GC::create( void ) {
   XGCValues settings;
   unsigned long mask = 0;

   xgc = XCreateGC( window->display()->xDisplay(), target, 
         mask, &settings );

   //TODO:  if (xgc == 0) ...

   setBackground( window->getBackground() );
   return xgc;
}

//== Accessors ================================================================

void XX::GC::setForeground( XX::Color *color ) {
   XSetForeground( window->display()->xDisplay(), xgc, color->getPixel() );
}

void XX::GC::setBackground( XX::Color *color ) {
   XSetBackground( window->display()->xDisplay(), xgc, color->getPixel() );
}

void XX::GC::setFont( XX::Font *font ) {
   XSetFont( window->display()->xDisplay(), xgc, font->getXFont() );
}

//== Operations ===============================================================

void XX::GC::drawPoint( int x, int y ) {
   XDrawPoint( window->display()->xDisplay(), target, xgc, x, y );
}

void XX::GC::drawLine( int x1, int y1, int x2, int y2 ) {
   XDrawLine( window->display()->xDisplay(), target, xgc, 
         x1, y1, x2, y2 );
}

void XX::GC::drawRectangle( int x, int y, int width, int height ) {
   XDrawRectangle( window->display()->xDisplay(), target, xgc, 
         x, y, width, height );
}

void XX::GC::fillRectangle( int x, int y, int width, int height ) {
   XFillRectangle( window->display()->xDisplay(), target, xgc, 
         x, y, width, height );
}

void XX::GC::drawArc( int x, int y, int width, int height, 
      double start, double sweep ) {
   XDrawArc( window->display()->xDisplay(), target, xgc, 
         x, y, width, height, angle( start ), angle( sweep ) );
}

void XX::GC::fillArc( int x, int y, int width, int height, 
      double start, double sweep ) {
   XFillArc( window->display()->xDisplay(), target, xgc, 
         x, y, width, height, angle( start ), angle( sweep ) );
}

void XX::GC::drawText( int x, int y, const std::string text ) {
   XDrawString( window->display()->xDisplay(), target, xgc, x, y,
        text.c_str(), text.length() );
}

void XX::GC::fillText( int x, int y, const std::string text ) {
   XDrawImageString( window->display()->xDisplay(), target, xgc, x, y,
        text.c_str(), text.length() );
}

