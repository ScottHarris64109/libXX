/*#############################################################################
 * XX::PixMap.cc -- Pixel map.
 *
 *  Copyright (C) 2012,2015,2026 by Scott Harris.  
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
#include <cstdint>

#include "Display.hh"
#include "Screen.hh"
#include "Window.hh"
#include "PixMap.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a PixMap object.
 */
XX::PixMap::~PixMap( ) {
   XFreePixmap( window->display()->xDisplay(), id );
}

/**
 * Create a PixMap for a Screen.
 */
XX::PixMap::PixMap( XX::Screen *screen, int width, int height ) : 
         window( screen->rootWindow() ), width( width ), height( height ) {
   id = XCreatePixmap( window->display()->xDisplay(),
         window->getXID(), width, height, window->getColorDepth() );
   depth = window->getColorDepth();
}

/**
 * Create a PixMap for a Window.
 */
XX::PixMap::PixMap( XX::Window *window, int width, int height ) : 
         window( window ), width( width ), height( height ) {
   id = XCreatePixmap( window->display()->xDisplay(),
         window->getXID(), width, height, window->getColorDepth() );
   depth = window->getColorDepth();
}

