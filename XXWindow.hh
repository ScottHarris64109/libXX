// XXWindow.hh -- XX Window class header.
// Copyright (C) 2014,2016,2026 by Scott Harris.  All rights reserved.

#ifndef XXWINDOW_HH_
#define XXWINDOW_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "XXDisplay.hh"
#include "XXScreen.hh"
#include "XXColor.hh"

namespace XX {
/**
 * @brief Window is a wrapper around an X11 Window.
 */

class Window {

private:
   XX::Display *display;
   XX::Screen *screen_;
   Window  *parent;
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

   void initialize( bool overrideRedirect );

protected:

public:
   //== Constructors ===========================================================

   virtual ~Window();

   Window( XX::Screen *s );

   Window( XX::Screen *screen, int atX, int atY, int width, int height, 
        XX::Color *background=NULL, int borderWidth=-1, 
        XX::Color *borderColor=NULL, bool overrideRedirect=false );

   Window( Window *parent, int atX, int atY, int width, int height, 
        XX::Color *background=NULL, int borderWidth=-1, 
        XX::Color *borderColor=NULL, bool overrideRedirect=false );

   //== Accessors ==============================================================

   inline ::Window getXID( void ) const { return xid; }
   inline XX::Display *getDisplay( void ) const { return this->screen()->display(); }
   inline XX::Color *getBackground( void ) const { return background; }
   inline bool isOpen( void ) const { return is_open; }
   inline XX::Screen *screen( void ) const { return this->screen_; }
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

#endif /* XXWINDOW_HH_ */
