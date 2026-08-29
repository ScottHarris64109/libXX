// XX::PixMap.hh -- Pixel map.
// Copyright (C) 2014,2015,2026 by Scott Harris.  

#ifndef XX_PIXMAP_HH_
#define XX_PIXMAP_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "Display.hh"
#include "Drawable.hh"
#include "Screen.hh"
#include "Window.hh"
#include "Color.hh"

namespace XX {

/**
 * @brief PixMap is an object wrapper for an X11 PixMap.
 */

class PixMap : public XX::Drawable {

private:
   XX::Window  *window;

protected:

public:
   //== Constructors ===========================================================

   virtual ~PixMap();
   PixMap( XX::Screen *screen, int width, int height ); 
   PixMap( XX::Window *window, int width, int height );

   //== Accessors ==============================================================

   inline XX::Window *getWindow( void ) const { return window; }

}; // class
}; // namespace

#endif /* XX_PIXMAP_HH_ */
