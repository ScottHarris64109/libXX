/*#############################################################################
 * XXWindow.cc -- XXWindow class code.
 * Copyright (C) 2012,2016,2026 by Scott Harris.  
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library.  If not, see <https://www.gnu.org/licenses/>.
 *#############################################################################
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "XXDisplay.hh"
#include "XXWindow.hh"
#include "XXColor.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a XXWindow object.
 */

XXWindow::~XXWindow( ) {
   this->display->removeWindow( this );
   XDestroyWindow( this->display->xDisplay(), this->getXID() );
}

/**
 *  Create the root XXWindow for an XXDisplay.
 */

XXWindow::XXWindow( ) {
   parent = NULL;
   display = NULL;
   is_open = false;
   //background = new XXColor( 0xff, 0xff, 0xff );
}

/**
 *  Make this the root XXWindow for an XXDisplay.
 */

void XXWindow::makeRoot( XXDisplay* display, int screen ) {
   this->display = display;
   this->screen = screen;
   parent = NULL;
   xid = RootWindow( display->xDisplay(), screen );
   originX = 0;
   originY = 0;
   height = display->height( screen );
   width  = display->width( screen );
   background = display->getColor( "white", screen );
   borderWidth = 3;
   border = background;
   this->display->addWindow( this );
}

/*
XXWindow::XXWindow( XXWindow *parent, 
      int originX, int originY, int width, int height ) :
            parent( parent ), originX( originX ), originY( originY ), 
            width( width ), height( height ) {

   XSetWindowAttributes attributes;
//   XSizeHints           sizeHints;
   unsigned long        mask = 0;

   background = parent->getBackground();
   border = parent->border;
   borderWidth = parent->borderWidth;
   display = parent->display;
   screen = parent->screen;
   is_open = false;

   attributes.border_pixel     = border->getPixel();
   mask |= CWBorderPixel;
   attributes.background_pixel = background->getPixel();
   mask |= CWBackPixel;
   attributes.override_redirect = True;
   mask |= CWOverrideRedirect;

   xid = XCreateWindow( display->xDisplay(), parent->getXID(), 
         originX, originY, width, height, borderWidth,
         display->colorDepth( screen ),
         InputOutput, CopyFromParent, mask, &attributes );

   sizeHints.x      = originX;
   sizeHints.y      = originY;
   sizeHints.width  = width;
   sizeHints.height = height;
   sizeHints.flags  = PPosition | PSize;

   XSetNormalHints( display->xDisplay(), getXID(), &sizeHints );
   this->display->addWindow( this );
}
*/

XXWindow::XXWindow( XXDisplay *display, int screen,
      int originX, int originY, int width, int height, XXColor *background,
      int borderWidth, XXColor *border, bool overrideRedirect ) :
            display( display ), screen( screen ), 
            originX( originX ), originY( originY ), 
            width( width ), height( height ), background( background ),
            borderWidth( borderWidth ), border( border ) {

   parent = display->root( screen );
   initialize( overrideRedirect );
}

XXWindow::XXWindow( XXDisplay *display, 
      int originX, int originY, int width, int height, XXColor *background,
      int borderWidth, XXColor *border, bool overrideRedirect ) :
            display( display ), originX( originX ), originY( originY ), 
            width( width ), height( height ), background( background ),
            borderWidth( borderWidth ), border( border ) {

   screen = display->defaultScreen();
   parent = display->root( screen );
   initialize( overrideRedirect );
}

XXWindow::XXWindow( XXWindow *parent, 
      int originX, int originY, int width, int height, XXColor *background,
      int borderWidth, XXColor *border, bool overrideRedirect ) :
            parent( parent ), originX( originX ), originY( originY ), 
            width( width ), height( height ), background( background ),
            borderWidth( borderWidth ), border( border ) {

   display = parent->display;
   screen = parent->screen;
   initialize( overrideRedirect );
}

void XXWindow::initialize( bool overrideRedirect ) {
   XSetWindowAttributes attributes;
   unsigned long        mask = 0;

   if (background == NULL) {
      background = parent->background;
   }
   if (borderWidth < 0) {
      borderWidth = parent->borderWidth;
   }
   if (border == NULL) {
      border = parent->border;
   }
   is_open = false;

   attributes.border_pixel     = background->getPixel();
   mask |= CWBorderPixel;
   attributes.background_pixel = background->getPixel();
   mask |= CWBackPixel;
   attributes.override_redirect = (overrideRedirect) ? True: False;
   mask |= CWOverrideRedirect;

   xid = XCreateWindow( display->xDisplay(), parent->getXID(), 
         originX, originY, width, height, borderWidth,
         display->colorDepth( screen ),
         InputOutput, CopyFromParent, mask, &attributes );

   this->display->addWindow( this );
}

//== Accessors ================================================================

/**
 * Get the X11 Atom needed to trap the close event for this window.
 */
Atom XXWindow::getCloseAtom() {
   Atom closeAtom=XInternAtom(display->xDisplay(), "WM_DELETE_WINDOW", True);
   XSetWMProtocols(display->xDisplay(), this->getXID(), &closeAtom, 1);
   return closeAtom;
}

//== Operations ===============================================================

void XXWindow::open( bool immediately ) {
   XMapWindow( display->xDisplay(), getXID() );
   is_open = true;
   if (immediately) {
      display->flush();
   }
}

void XXWindow::close( bool immediately ) {
   XUnmapWindow( display->xDisplay(), getXID() );
   is_open = false;
   if (immediately) {
      display->flush();
   }
}

void XXWindow::listenFor( unsigned long events ) {
   eventMask = events;
   XSelectInput( display->xDisplay(), getXID(), events );
}

XEvent *XXWindow::getNextEvent( XEvent *event, bool block ) {
   if (block) {
      XWindowEvent( display->xDisplay(), getXID(), eventMask, event );
   } else if (!XCheckWindowEvent( display->xDisplay(), getXID(), 
            eventMask, event )) {
      return NULL;
   }
   return event;
}

XEvent *XXWindow::getNextEvent( XEvent *event, unsigned long eventTypes, 
      bool block ) {
   if (block) {
      XWindowEvent( display->xDisplay(), getXID(), eventTypes, event );
   } else if (!XCheckWindowEvent( display->xDisplay(), getXID(), 
            eventTypes, event )) {
      return NULL;
   }
   return event;
}
