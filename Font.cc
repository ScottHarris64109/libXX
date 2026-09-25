/*#############################################################################
 * XX::Font.cc -- X11 Font wrapper.
 *
 *  Copyright (C) 2012,2016,2026 by Scott Harris.  
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

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "Display.hh"
#include "Font.hh"

//== Constructors ==============================================================

/**
 *  Destroy and deallocate a Font object.
 */

XX::Font::~Font( ) {
   XFreeFont( this->display->xDisplay(), this->xfont );
}

/**
 *  Get the named Font from the Display.
 */

XX::Font::Font( XX::Display *display, const std::string name ) : 
      display{ display }, name{ name } {
   xfont = XLoadQueryFont( display->xDisplay(), name.c_str() );
}

//== Accessors =================================================================

int XX::Font::getWidth( ) const {
   return this->xfont->max_bounds.rbearing - this->xfont->max_bounds.lbearing;
}

int XX::Font::getWidth( const std::string text ) const {
   return XTextWidth( this->xfont, text.c_str(), text.length() );
}

int XX::Font::getHeight( ) const {
   return this->xfont->ascent + this->xfont->descent;
}

