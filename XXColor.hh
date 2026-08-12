// XXColor.hh -- 24-bit color.
// Copyright (C) 2014,2015,2026 by Scott Harris.  

#ifndef XXCOLOR_HH_
#define XXCOLOR_HH_

#include <string>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace XX {
/**
 * @brief XXColor is a 24-bit color for the XX library.
 */

class XXColor {

private:
   unsigned int red;
   unsigned int blue;
   unsigned int green;

   //void lookupColor( XXDisplay *display, int screen, const char *name );
 
protected:

public:
   //== Constructors ===========================================================

   /// Deallocate and destroy this XXColor.
   virtual ~XXColor();

   /// Create an XXColor from RGB component values.
   XXColor( unsigned int red, unsigned int green, unsigned int blue );

   // Look up an XXColor by name.
   //XXColor( XXDisplay *display, string name );

   // Look up an XXColor by name.
   //XXColor( XXDisplay *display, const char *name );

   // Look up an XXColor by name.
   //XXColor( XXDisplay *display, int screen, string name );

   // Look up an XXColor by name.
   //XXColor( XXDisplay *display, int screen, const char *name );

   // Look up an XXColor by name.
   //XXColor( XXWindow *window, string name );

   // Look up an XXColor by name.
   //XXColor( XXWindow *window, const char *name );

   // Look up an XXColor by name.
   //XXColor( XXGC *gc, string name );

   // Look up an XXColor by name.
   //XXColor( XXGC *gc, const char *name );

   //== Accessors ==============================================================

   /// Get the XColor pixel value.
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
