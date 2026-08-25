// XX::PixMap.hh -- Pixel map.
// Copyright (C) 2014,2015,2026 by Scott Harris.  

#ifndef XX_PIXMAP_HH_
#define XX_PIXMAP_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "Display.hh"
#include "Color.hh"

namespace XX {

/**
 * @brief PixMap is an object wrapper for an X11 PixMap.
 */

class PixMap {

private:
   XX::Window  *window;
   Pixmap     id;
   int        height;
   int        width;
   int        depth;

protected:

public:
   //== Constructors ===========================================================

   virtual ~PixMap();
   PixMap( XX::Window *window, int width, int height );
   PixMap( XX::Window *window, int width, int height, const char *bitMap );
   PixMap( XX::Window *window, int width, int height, const XX::Color *pixel );

   //== Accessors ==============================================================

   inline Pixmap getXPixmap( void ) const { return id; }
   inline XX::Window *getWindow( void ) const { return window; }
   inline int getWidth( void ) const { return width; }
   inline int getHeight( void ) const { return height; }
   inline int getColorDepth( void ) const { return depth; }

}; // class
}; // namespace

#endif /* XX_PIXMAP_HH_ */
