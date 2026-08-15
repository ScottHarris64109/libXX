// XXScreen.hh
// Copyright (C) 2016 by Scott Harris.  All rights reserved.

#ifndef XXSCREEN_HH_
#define XXSCREEN_HH_

#include <map>

#include "XXDisplay.hh"
#include "XXColor.hh"

namespace XX {
   class Window;

/**
 * @brief Screen is a ...
 *
 */
class Screen {
   // Only a Display can create or destroy Screens.
   friend class Display;
   // Window constructors and destructors need access to 
   // addWindow() and removeWindow().
   friend class Window;

private:
   int index_;
   int width_;
   int height_;
   int colorDepth_;
   Display *display_;
   Window *root_;
   std::map<XID, Window*> window_;

   Screen( Display *display, int index );
   virtual ~Screen();

   void addWindow( Window *w );
   void removeWindow( Window *w );

protected:

public:
   //== Accessors ==============================================================
   inline int index()      const { return index_; };
   inline int width()      const { return width_; };
   inline int height()     const { return height_; };
   inline int colorDepth() const { return colorDepth_; };

   inline Display *display() const { return display_; };
   inline Window *rootWindow() const { return root_; };
   /// Look up a Window by its XID.
   inline Window *window(XID id) { return window_[id]; };


   Color *getColor( const std::string name );
   Color *getColor( const char *name );

}; // class
}; // namespace

#endif /* XXSCREEN_HH_ */
