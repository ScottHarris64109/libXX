/*#############################################################################
 * XX::Color.cc -- 24-bit color.
 *
 *  Copyright (C) 2012-2026 by Scott Harris.  All rights reserved.
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
#include <string.h>
#include <stdexcept>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "Color.hh"

//== Constructors ==============================================================

XX::Color::~Color( ) {
}

XX::Color::Color( unsigned int red, unsigned int green, unsigned int blue ) :
      red_( red ), blue_( blue ), green_( green ) {
}

//== Accessors =================================================================

void XX::Color::setRed( unsigned int value ) {
   red_ = value % 256;
}

void XX::Color::setGreen( unsigned int value ) {
   green_ = value % 256;
}

void XX::Color::setBlue( unsigned int value ) {
   blue_ = value % 256;
}

//== Operations ================================================================

XX::Color XX::Color::operator=( const unsigned int *rgb ) {
   this->setRed(   rgb[0] );
   this->setGreen( rgb[1] );
   this->setBlue(  rgb[2] );
   return *this;
}

XX::Color XX::Color::complement() {
   XX::Color c( 255-this->red(), 255-this->green(), 255-this->blue() );
   return c;
}
