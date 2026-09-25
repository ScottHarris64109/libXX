// XX::Font.hh -- Font wrapper.
// Copyright (C) 2014,2015,2026 by Scott Harris.  

#ifndef XX_FONT_HH_
#define XX_FONT_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "Display.hh"

namespace XX {

/**
 * @brief Font encapsulates an X11 XFontStruct.
 */

class Font {
   // Only Display can create or delete Fonts.
   friend class Display;

private:
   XX::Display *display;
   XFontStruct *xfont;
   std::string name_;

   //== Constructors ===========================================================

   /// Deallocate and destroy this Font.
   virtual ~Font();

   /// Look up a font by name.
   Font( XX::Display *display, const std::string fontName );

protected:

public:
   //== Accessors ==============================================================

   /// Get the font name.
   inline std::string name( void ) const { return this->name_; }

   /// Get the X11 font ID.
   inline ::Font getXFont( void ) const { return xfont->fid; }

   /// Get the default character width.
   int width( ) const;

   /// Get the width of a string rendered with this font.
   int width( const std::string text ) const;

   /// Get the character height.
   int height( ) const;

   /// Get the height of a string rendered with this font.
   int height( const std::string text ) const;

   /// Get the height of the font above the origin.
   inline int ascent() const { return this->xfont->ascent; };

   /// Get the height of the font below the origin.
   inline int descent() const { return this->xfont->descent; };

   //== Operations =============================================================


}; // class
}; // namespace

#endif /* XX_FONT_HH_ */
