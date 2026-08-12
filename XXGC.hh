// XXGC.hh -- Wrapper for an X11 GC (Graphics Context).
// Copyright (C) 2014,2015,2026 by Scott Harris.  All rights reserved.

#ifndef XXGC_HH_
#define XXGC_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "XXDisplay.hh"
#include "XXWindow.hh"
#include "XXColor.hh"
#include "XXPixMap.hh"
#include "XXFont.hh"

namespace XX {

/**
 * @brief XXGC encapsulates an X11 GC (Graphics Context).
 */

class XXGC {

private:
   XX::XXWindow  *window;
   XX::XXPixMap  *pixmap;
   XID       target;
   GC        gc;

   GC create( void );

   inline int angle( double degrees ) const {
      return (int)((degrees * 64.0) + 0.5);
   }

protected:

public:
   //== Constructors ===========================================================

   /// Deallocate and destroy this XXGC.
   virtual ~XXGC();

   /// Get a new XXGC for the XXWindow.
   XXGC( XX::XXWindow *window );

   /// Get a new XXGC for the XXPixmap.
   XXGC( XX::XXPixMap *pixmap );

   //== Accessors ==============================================================

   /// Get the XXWindow that this XXGC draws in.
   inline XX::XXWindow *getWindow( void ) const {
      return window;
   }

   /// Set the foreground color.
   void setForeground( XX::XXColor *color );

   /// Set the background color.
   void setBackground( XX::XXColor *color );

   /// Set the font for drawing text.
   void setFont( XX::XXFont *font );

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

#endif /* XXGC_HH_ */
