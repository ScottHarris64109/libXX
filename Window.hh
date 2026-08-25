// XX::Window.hh -- XX Window class header.
// Copyright (C) 2014,2016,2026 by Scott Harris.  All rights reserved.

#ifndef XX_WINDOW_HH_
#define XX_WINDOW_HH_

#include <unordered_map>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "Display.hh"

#include "Screen.hh"
#include "Color.hh"

namespace XX {
/**
 * @brief Window is a wrapper around an X11 Window.
 */

class Window {
   // Screen needs the root Window constructor.
   friend class Screen;

private:
   XX::Screen *screen_;
   Window  *parent;
   std::unordered_map<XID, Window*> children;
   ::Window     xid;
   XX::Color   *background;
   XX::Color   *border;
   bool       is_open;
   int        originX;
   int        originY;
   int        height;
   int        width;
   int        borderWidth;
   unsigned long eventMask;

   Window( XX::Screen *s );  // Screen root constructor

   void initialize( bool overrideRedirect );

protected:

public:
   //== Constructors ===========================================================

   virtual ~Window();

   Window( XX::Screen *screen, int atX, int atY, int width, int height, 
        XX::Color *background=nullptr, int borderWidth=-1, 
        XX::Color *borderColor=nullptr, bool overrideRedirect=false );

   Window( Window *parent, int atX, int atY, int width, int height, 
        XX::Color *background=nullptr, int borderWidth=-1, 
        XX::Color *borderColor=nullptr, bool overrideRedirect=false );

   //== Accessors ==============================================================

   inline ::Window getXID( void ) const { return xid; }
   inline XX::Display *display( void ) const { 
      return this->screen()->display(); 
   }
   inline XX::Screen *screen( void ) const { return this->screen_; }
   inline XX::Color *getBackground( void ) const { return background; }
   inline bool isOpen( void ) const { return is_open; }
   inline int getWidth( void ) const { return width; }
   inline int getHeight( void ) const { return height; }

   inline int getColorDepth( void ) const { 
      return this->screen()->colorDepth();
   }

   Atom getCloseAtom();

   //== Operations =============================================================

   void open( bool immediately=false );
   void close( bool immediately=false );

   void listenFor( unsigned long eventTypes );
   XEvent *getNextEvent( XEvent *event, bool block=true );
   XEvent *getNextEvent( XEvent *event, unsigned long eventTypes, 
         bool block=true );
}; // class
}; // namespace

#endif /* XX_WINDOW_HH_ */
