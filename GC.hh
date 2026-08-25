// XX::GC.hh -- Wrapper for an X11 GC (Graphics Context).
// Copyright (C) 2014,2015,2026 by Scott Harris.  All rights reserved.

#ifndef XX_GC_HH_
#define XX_GC_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "Display.hh"
#include "Window.hh"
#include "Color.hh"
#include "PixMap.hh"
#include "Font.hh"

namespace XX {

/**
 * @brief GC encapsulates an X11 GC (Graphics Context).
 */

class GC {

private:
   XX::Window  *window;
   XX::PixMap  *pixmap;
   XID         target;
   ::GC          xgc;

   ::GC create( void );

   inline int angle( double degrees ) const {
      return (int)((degrees * 64.0) + 0.5);
   }

protected:

public:
   //== Constructors ===========================================================

   /// Deallocate and destroy this GC.
   virtual ~GC();

   /// Get a new GC for the Window.
   GC( XX::Window *window );

   /// Get a new GC for the XXPixmap.
   GC( XX::PixMap *pixmap );

   //== Accessors ==============================================================

   /// Get the Window that this GC draws in.
   inline XX::Window *getWindow( void ) const {
      return window;
   }

   /// Set the foreground color.
   void setForeground( XX::Color *color );

   /// Set the background color.
   void setBackground( XX::Color *color );

   /// Set the font for drawing text.
   void setFont( XX::Font *font );

   //== Operations =============================================================

   /// Draw a point.
   void drawPoint( int x, int y );

   /// Draw a line.
   void drawLine( int x1, int y1, int x2, int y2 );

   /// Draw a rectangle.
   void drawRectangle( int x, int y, int width, int height );

   /// Draw and fill in a rectangle.
   void fillRectangle( int x, int y, int width, int height );

   /// Draw an arc.
   void drawArc( int x, int y, int width, int height, 
         double start, double sweep );

   /// Draw and fill in an arc.
   void fillArc( int x, int y, int width, int height, 
         double start, double sweep );

   /// Draw text with no background.
   void drawText( int x, int y, const std::string text );

   /// Draw text and fill in its background.
   void fillText( int x, int y, const std::string text );

}; // class
}; // namespace

#endif /* XX_GC_HH_ */
