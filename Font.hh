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

private:
   XX::Display *display;
   XFontStruct *xfont;
   std::string name;

protected:

public:
   //== Constructors ===========================================================

   /// Deallocate and destroy this Font.
   virtual ~Font();

   /// Look up a font by name.
   Font( XX::Display *display, const std::string fontName );

   /// Look up a font by name.
   Font( XX::Display *display, const char *fontName );

   //== Accessors ==============================================================

   /// Get the font name.
   inline std::string getName( void ) const { return name; }

   /// Get the X11 font ID.
   inline ::Font getXFont( void ) const { return xfont->fid; }

   //== Operations =============================================================


}; // class
}; // namespace

#endif /* XX_FONT_HH_ */
