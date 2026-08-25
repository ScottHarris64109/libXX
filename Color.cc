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

//== Constructors =============================================================

XX::Color::~Color( ) {
}

XX::Color::Color( unsigned int red, unsigned int green, unsigned int blue ) {
   setRed( red );
   setGreen( green );
   setBlue( blue );
}

//== Accessors ================================================================

void XX::Color::setRed( unsigned int value ) {
   red = value % 256;
}

void XX::Color::setGreen( unsigned int value ) {
   green = value % 256;
}

void XX::Color::setBlue( unsigned int value ) {
   blue = value % 256;
}
