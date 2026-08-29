/*#############################################################################
 * XX::GC.cc -- Wrapper for an X11 GC (Graphics Context).
 *
 *  Copyright (C) 2026 by Scott Harris.  All rights reserved.
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
#include <iostream>
#include <stdlib.h>
#include <errno.h>

#include "Display.hh"
#include "Drawable.hh"
#include "Color.hh"
#include "Font.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a Drawable object.
 */

XX::Drawable::~Drawable( ) {
   XFreeGC( this->display()->xDisplay(), this->context );
}

/**
 *  Create a Drawable with default values.
 */

XX::Drawable::Drawable( XX::Display *d ) : display_( d ) {
}

//== Operations ===============================================================

// Call this at the end of the subclass constructor.
void XX::Drawable::makeContext() {
   XGCValues settings;
   unsigned long mask = 0;

   this->context = XCreateGC( this->display()->xDisplay(), this->getXID(), 
         mask, &settings );
}

void XX::Drawable::drawPoint( XX::Color *color, int x, int y ) {
   XSetForeground( this->display()->xDisplay(), this->context, 
         color->getPixel() );
   XDrawPoint( this->display()->xDisplay(), this->getXID(), this->context, 
         x, y );
}

void XX::Drawable::drawLine( XX::Color *color, int x1, int y1, int x2, int y2 ) {
   XSetForeground( this->display()->xDisplay(), this->context, 
         color->getPixel() );
   XDrawLine( this->display()->xDisplay(), this->getXID(), this->context, 
         x1, y1, x2, y2 );
}

void XX::Drawable::drawRectangle( XX::Color *color, int x, int y, 
      int width, int height ) {
   XSetForeground( this->display()->xDisplay(), this->context, 
         color->getPixel() );
   XDrawRectangle( this->display()->xDisplay(), this->getXID(), this->context, 
         x, y, width, height );
}

void XX::Drawable::fillRectangle( XX::Color *color, int x, int y, 
      int width, int height ) {
   XSetForeground( this->display()->xDisplay(), this->context, 
         color->getPixel() );
   XFillRectangle( this->display()->xDisplay(), this->getXID(), this->context, 
         x, y, width, height );
}

void XX::Drawable::drawArc( XX::Color *color, int x, int y, 
      int width, int height, double start, double sweep ) {
   XSetForeground( this->display()->xDisplay(), this->context, 
         color->getPixel() );
   XDrawArc( this->display()->xDisplay(), this->getXID(), this->context, 
         x, y, width, height, angle( start ), angle( sweep ) );
}

void XX::Drawable::fillArc( XX::Color *color, int x, 
      int y, int width, int height, double start, double sweep ) {
   XSetForeground( this->display()->xDisplay(), this->context, 
         color->getPixel() );
   XFillArc( this->display()->xDisplay(), this->getXID(), this->context, 
         x, y, width, height, angle( start ), angle( sweep ) );
}

void XX::Drawable::drawText( XX::Color *color, XX::Font *font, int x, int y, 
      const std::string text ) {
   XSetForeground( this->display()->xDisplay(), this->context, 
         color->getPixel() );
   XSetFont( this->display()->xDisplay(), this->context, font->getXFont() );
   XDrawString( this->display()->xDisplay(), this->getXID(), this->context, 
         x, y, text.c_str(), text.length() );
}

void XX::Drawable::fillText( XX::Color *color, XX::Font *font, int x, int y, 
      const std::string text ) {
   XSetForeground( this->display()->xDisplay(), this->context, 
         color->getPixel() );
   XSetFont( this->display()->xDisplay(), this->context, font->getXFont() );
   XDrawImageString( this->display()->xDisplay(), this->getXID(), this->context,
         x, y, text.c_str(), text.length() );
}

