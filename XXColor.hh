// XXColor.hh -- 24-bit color.
// Copyright (C) 2014,2015,2026 by Scott Harris.  

#ifndef XXCOLOR_HH_
#define XXCOLOR_HH_

#include <string>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace XX {
/**
 * @brief Color is a 24-bit color for the XX library.
 */

class Color {

private:
   unsigned int red;
   unsigned int blue;
   unsigned int green;

   //void lookupColor( XX::Display *display, int screen, const char *name );
 
protected:

public:
   //== Constructors ===========================================================

   /// Deallocate and destroy this Color.
   virtual ~Color();

   /// Create a Color from RGB component values.
   Color( unsigned int red, unsigned int green, unsigned int blue );

   // Look up a Color by name.
   //Color( XX::Display *display, string name );

   // Look up a Color by name.
   //Color( XX::Display *display, const char *name );

   // Look up a Color by name.
   //Color( XX::Display *display, int screen, string name );

   // Look up a Color by name.
   //Color( XX::Display *display, int screen, const char *name );

   // Look up a Color by name.
   //Color( XX::Window *window, string name );

   // Look up a Color by name.
   //Color( XX::Window *window, const char *name );

   // Look up a Color by name.
   //Color( XX::GC *gc, string name );

   // Look up a Color by name.
   //Color( XX::GC *gc, const char *name );

   //== Accessors ==============================================================

   /// Get the X11 Color pixel value.
   inline unsigned long getPixel( void ) {
      return (red << 16) | (green << 8) | blue;
   }

   /// Get the red component in the RGB color model.
   inline unsigned int getRed( void ) const { return red; }

   /// Get the green component in the RGB color model.
   inline unsigned int getGreen( void ) const { return green; }

   /// Get the blue component in the RGB color model.
   inline unsigned int getBlue( void ) const { return blue; }

   /// Set the red component in the RGB color model.
   void setRed( unsigned int value );

   /// Set the green component in the RGB color model.
   void setGreen( unsigned int value );

   /// Set the blue component in the RGB color model.
   void setBlue( unsigned int value );

}; // class
}; // namespace

#endif /* XXCOLOR_HH_ */
