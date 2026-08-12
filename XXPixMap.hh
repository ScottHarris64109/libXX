// XXPixMap.hh -- Pixel map.
// Copyright (C) 2014,2015,2026 by Scott Harris.  

#ifndef XXPIXMAP_HH_
#define XXPIXMAP_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "XXDisplay.hh"

namespace XX {

/**
 * @brief XXPixMap is a ...
 */

class XXPixMap {

private:
   XX::XXWindow  *window;
   Pixmap     id;
   int        height;
   int        width;
   int        depth;

protected:

public:
   //== Constructors ===========================================================

   virtual ~XXPixMap();
   XXPixMap( XX::XXWindow *window, int width, int height );
   XXPixMap( XX::XXWindow *window, int width, int height, const char *bitMap );

   //== Accessors ==============================================================

   inline Pixmap getXPixmap( void ) const { return id; }
   inline XX::XXWindow *getWindow( void ) const { return window; }
   inline int getWidth( void ) const { return width; }
   inline int getHeight( void ) const { return height; }
   inline int getColorDepth( void ) const { return depth; }

}; // class
}; // namespace

#endif /* XXPIXMAP_HH_ */
