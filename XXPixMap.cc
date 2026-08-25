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
#include <cstdint>

#include "XXDisplay.hh"
#include "XXWindow.hh"
#include "XXPixMap.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a PixMap object.
 */
XX::PixMap::~PixMap( ) {
   XFreePixmap( window->display()->xDisplay(), id );
}

/**
 * Create an PixMap for an Window.
 */
XX::PixMap::PixMap( XX::Window *window, int width, int height ) : 
         window( window ), width( width ), height( height ) {
   id = XCreatePixmap( window->display()->xDisplay(),
         window->getXID(), width, height, window->getColorDepth() );
   depth = window->getColorDepth();
}

XX::PixMap::PixMap( XX::Window *window, int width, int height, 
     const char *bitMap /* width*height/8 */ ) : 
         window( window ), width( width ), height( height ) {
   id = XCreateBitmapFromData( window->display()->xDisplay(),
         window->getXID(), bitMap, width, height );
   depth = 1;
}

XX::PixMap::PixMap( XX::Window *window, int width, int height, 
     const XX::Color *pixel /* [row][column] */ ) : 
         window( window ), width( width ), height( height ) {
   id = XCreatePixmap( window->display()->xDisplay(),
         window->getXID(), width, height, window->getColorDepth() );
   depth = window->getColorDepth();
   uint32_t buffer[100];
/*
https://tronche.com/gui/x/xlib/utilities/XCreatePixmapFromBitmapData.html

Pixmap XCreatePixmapFromBitmapData(display, d, data, width, height, fg, bg, depth)
     Display *display;
     Drawable d;
     char *data;
     unsigned int width, height;
     unsigned long fg, bg;
     unsigned int depth;

Arguments
display 	Specifies the connection to the X server.
d 	Specifies the drawable that indicates the screen.
data 	Specifies the data in bitmap format.
width
height 	Specify the width and height.
fg
bg 	Specify the foreground and background pixel values to use.
depth 	Specifies the depth of the pixmap. 

/+ XPM +/
static char * XFACE[] = {
"48 4 2 1", // width, height, colors, characters per pixel
"a c #ffffff",
"b c #000000",
"abaabaababaaabaabababaabaabaababaabaaababaabaaab",
"abaabaababaaabaabababaabaabaababaabaaababaabaaab",
"abaabaababaaabaabababaabaabaababaabaaababaabaaab",
"abaabaababaaabaabababaabaabaababaabaaababaabaaab"
};
*/
}
