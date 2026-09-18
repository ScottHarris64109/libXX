// XX::Color.hh -- 24-bit color.
// Copyright (C) 2014,2015,2026 by Scott Harris.  

#ifndef XX_COLOR_HH_
#define XX_COLOR_HH_

#include <string>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace XX {
/**
 * @brief Color is a 24-bit color for the XX library.
 */

class Color {

private:
   unsigned int red_;
   unsigned int blue_;
   unsigned int green_;

protected:

public:
   //== Constructors ===========================================================

   /// Deallocate and destroy this Color.
   virtual ~Color();

   /// Create a Color from RGB component values.
   Color( unsigned int red=0, unsigned int green=0, unsigned int blue=0 );

   //== Comparisons ============================================================

   /// Equal-to comparison.
   inline bool operator==( const Color &that ) const { 
      return (this->red() == that.red() 
           && this->blue() == that.blue() 
           && this->green() == that.green()) ? true: false; 
   };

   //== Accessors ==============================================================

   /// Get the X11 Color pixel value.
   inline unsigned long getPixel( void ) {
      return (red_ << 16) | (green_ << 8) | blue_;
   }

   /// Get the red component in the RGB color model.
   inline unsigned int red( void ) const { return red_; }

   /// Get the green component in the RGB color model.
   inline unsigned int green( void ) const { return green_; }

   /// Get the blue component in the RGB color model.
   inline unsigned int blue( void ) const { return blue_; }

   /// Set the red component in the RGB color model.
   void setRed( unsigned int value );

   /// Set the green component in the RGB color model.
   void setGreen( unsigned int value );

   /// Set the blue component in the RGB color model.
   void setBlue( unsigned int value );

   //== Operations =============================================================

   /// Assign a color using an array of RGB values.
   Color operator=( const unsigned int * );

   // Get the complement of the color.
   Color complement();

}; // class
}; // namespace

#endif /* XX_COLOR_HH_ */
