// XXFont.hh -- Font wrapper.
// Copyright (C) 2014,2015,2026 by Scott Harris.  

#ifndef XXFONT_HH_
#define XXFONT_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "XXDisplay.hh"

namespace XX {

/**
 * @brief XXFont encapsulates an X11 XFontStruct.
 */

class XXFont {

private:
   XX::XXDisplay *display;
   XFontStruct *font;
   std::string name;

protected:

public:
   //== Constructors ===========================================================

   /// Deallocate and destroy this XXFont.
   virtual ~XXFont();

   /// Look up a font by name.
   XXFont( XX::XXDisplay *display, const std::string fontName );

   /// Look up a font by name.
   XXFont( XX::XXDisplay *display, const char *fontName );

   //== Accessors ==============================================================

   /// Get the font name.
   inline std::string getName( void ) const { return name; }

   /// Get the X11 font ID.
   inline Font getXFont( void ) const { return font->fid; }

   //== Operations =============================================================


}; // class
}; // namespace

#endif /* XXFONT_HH_ */
