// XXWindow.hh -- XXWindow class header.
// Copyright (C) 2014,2016,2026 by Scott Harris.  All rights reserved.

#ifndef XXWINDOW_HH_
#define XXWINDOW_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "XXDisplay.hh"
#include "XXColor.hh"

namespace XX {
/**
 * @brief XXWindow is an X11 Window.
 */

class XXWindow {
   // XXDisplay needs to call the parameterless constructor and makeRoot().
   friend class XX::XXDisplay;

private:
   XX::XXDisplay *display;
   XXWindow  *parent;
   Window     xid;
   XX::XXColor   *background;
   XX::XXColor   *border;
   bool       is_open;
   int        screen;
   int        originX;
   int        originY;
   int        height;
   int        width;
   int        borderWidth;
   unsigned long eventMask;

   void initialize( bool overrideRedirect );

   XXWindow( void );
   void makeRoot( XX::XXDisplay *display, int screen );

protected:

public:
   //== Constructors ===========================================================

   virtual ~XXWindow();

   XXWindow( XX::XXDisplay *display, int screen, 
        int atX, int atY, int width, int height, 
        XX::XXColor *background=NULL, int borderWidth=-1, 
        XX::XXColor *borderColor=NULL, bool overrideRedirect=false );

   XXWindow( XX::XXDisplay *display, int atX, int atY, int width, int height, 
        XX::XXColor *background=NULL, int borderWidth=-1, 
        XX::XXColor *borderColor=NULL, bool overrideRedirect=false );

   XXWindow( XXWindow *parent, int atX, int atY, int width, int height, 
        XX::XXColor *background=NULL, int borderWidth=-1, 
        XX::XXColor *borderColor=NULL, bool overrideRedirect=false );

   //== Accessors ==============================================================

   inline Window getXID( void ) const { return xid; }
   inline XX::XXDisplay *getDisplay( void ) const { return display; }
   inline XX::XXColor *getBackground( void ) const { return background; }
   inline bool isOpen( void ) const { return is_open; }
   inline int getScreen( void ) const { return screen; }
   inline int getWidth( void ) const { return width; }
   inline int getHeight( void ) const { return height; }

   inline int getColorDepth( void ) const { 
      return this->display->colorDepth( this->getScreen() );
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
