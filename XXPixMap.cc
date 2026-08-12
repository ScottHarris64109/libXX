/*#############################################################################
 * XXPixMap.cc -- Pixel map.
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

#include "XXDisplay.hh"
#include "XXWindow.hh"
#include "XXPixMap.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a XXPixMap object.
 */
XXPixMap::~XXPixMap( ) {
   XFreePixmap( window->getDisplay()->xDisplay(), id );
}

/**
 * Create an XXPixMap for an XXWindow.
 */
XXPixMap::XXPixMap( XXWindow *window, int width, int height ) : 
         window( window ), width( width ), height( height ) {
   id = XCreatePixmap( window->getDisplay()->xDisplay(),
         window->getXID(), width, height, window->getColorDepth() );
   depth = window->getColorDepth();
}

XXPixMap::XXPixMap( XXWindow *window, int width, int height, 
     const char *bitMap /* width*height/8 */ ) : 
         window( window ), width( width ), height( height ) {
   id = XCreateBitmapFromData( window->getDisplay()->xDisplay(),
         window->getXID(), bitMap, width, height );
   depth = 1;
}
