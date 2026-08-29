// XX::Drawable.hh -- Semi-abstract parent class for XX::Window and XX::PixMap.
// Copyright (C) 2026 by Scott Harris.  All rights reserved.

#ifndef XX_DRAWABLE_HH_
#define XX_DRAWABLE_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace XX {
   class Display;
   class Color;
   class Font;

/**
 * @brief Drawable is a semi-abstract parent class for the XX::Window and 
 *    XX::PixMap classes.  It should never be instantiated as itself.
 */
class Drawable {

private:
   // Convert decimal degrees to X11 angle units.
   inline int angle( double degrees ) const {
      return (int)((degrees * 64.0) + 0.5);
   }

protected:
   XX::Display *display_;
   XID xid;
   int height;
   int width;
   int depth;
   ::GC context;

   Drawable( Display *display );
   virtual ~Drawable();

   void makeContext( void );

public:
   //== Accessors ==============================================================

   inline XX::Display *display( void ) const { return display_; }
   inline XID getXID( void ) const { return xid; }
   inline int getWidth( void ) const { return width; }
   inline int getHeight( void ) const { return height; }
   inline int getColorDepth( void ) const { return depth; }

   //== Operations =============================================================

   /// Draw a point.
   void drawPoint( XX::Color *color, int x, int y );

   /// Draw a line.
   void drawLine( XX::Color *color, int x1, int y1, int x2, int y2 );

   /// Draw a rectangle.
   void drawRectangle( XX::Color *color, int x, int y, int width, int height );

   /// Draw and fill in a rectangle.
   void fillRectangle( XX::Color *color, int x, int y, int width, int height );

   /// Draw an arc.
   void drawArc( XX::Color *color, int x, int y, int width, int height, 
         double start, double sweep );

   /// Draw and fill in an arc.
   void fillArc( XX::Color *color, int x, int y, int width, int height, 
         double start, double sweep );

   /// Draw text with no background.
   void drawText( XX::Color *color, XX::Font *font, int x, int y, 
         const std::string text );

   /// Draw text and fill in its background.
   void fillText( XX::Color *color, XX::Font *font, int x, int y, 
         const std::string text );

}; // class
}; // namespace
#endif /* XX_DRAWABLE_HH_ */
