// XXScreen.hh
// Copyright (C) 2016 by Scott Harris.  All rights reserved.

#ifndef XXSCREEN_HH_
#define XXSCREEN_HH_

#include "XXDisplay.hh"
#include "XXColor.hh"

namespace XX {
   class Window;

/**
 * @brief Screen is a physical display device/monitor.
 *
 * Every Screen is owned and managed by the Display it is attached to.  Only a 
 * Display can create or destroy a Screen.  There is currently no mechanism for 
 * detecting "hot pluggging" events where a monitor is added to or disconnected 
 * from a Display while said Display object exists.  A Display's driver 
 * software may merge multiple physical screens into a single virtual Screen 
 * and present them as a single object.
 * <p>
 * Every Screen has a root Window that can only be created or destroyed 
 * by the Screen itself.  
 * <p>
 * When a Screen is destroyed (by the destruction of its Display object) it 
 * will close and destroy any existing Windows that were created on it.
 */
class Screen {
   // Only a Display can create or destroy Screens.
   friend class Display;
   // The Window class needs to be able to see the root Window.
   friend class Window;

private:
   int index_;
   int width_;
   int height_;
   int colorDepth_;
   Display *display_;
   Window *root_;

   Screen( Display *display, int index );
   virtual ~Screen();

   inline Window *rootWindow() const { return root_; };

protected:

public:
   //== Accessors ==============================================================
   inline int index()      const { return index_; };
   inline int width()      const { return width_; };
   inline int height()     const { return height_; };
   inline int colorDepth() const { return colorDepth_; };

   inline Display *display() const { return display_; };

   Color *getColor( const std::string name );
   Color *getColor( const char *name );

}; // class
}; // namespace

#endif /* XXSCREEN_HH_ */
