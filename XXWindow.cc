/*#############################################################################
 * XXWindow.cc -- Window class code.
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
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "XXDisplay.hh"
#include "XXWindow.hh"
#include "XXScreen.hh"
#include "XXColor.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a Window object.
 */

XX::Window::~Window( ) {
   this->screen()->removeWindow( this );
   XDestroyWindow( this->screen()->display()->xDisplay(), this->getXID() );
}

/**
 *  Create a Window for a Screen.
 */

XX::Window::Window( XX::Screen *scr ) : screen_{ scr } {
   this->parent = this->screen()->rootWindow();
   this->originX = 0;
   this->originY = 0;
   this->height = this->screen()->height();
   this->width  = this->screen()->width();

   if (this->parent)
   {
      this->background  = this->parent->background;
      this->borderWidth = this->parent->borderWidth;
      this->border      = this->parent->border;
      this->initialize( false );
   }
   else // We are creating the root window for this screen.
   {
      this->is_open = false;
      this->background = this->screen()->getColor( "white" );
      this->borderWidth = 3;
      this->border = this->background;

      this->xid = RootWindow( this->screen()->display()->xDisplay(), 
            this->screen()->index() );
   }

   this->screen()->addWindow( this );
}

/*
Window::Window( Window *parent, 
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
         display->screen( screen )->colorDepth(),
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

XX::Window::Window( XX::Screen *scr,
      int originX, int originY, int width, int height, XX::Color *background,
      int borderWidth, XX::Color *border, bool overrideRedirect ) :
            screen_( scr ), 
            originX( originX ), originY( originY ), 
            width( width ), height( height ), background( background ),
            borderWidth( borderWidth ), border( border ) {

   parent = this->screen()->rootWindow();
   initialize( overrideRedirect );
}

XX::Window::Window( XX::Window *parent, 
      int originX, int originY, int width, int height, XX::Color *background,
      int borderWidth, XX::Color *border, bool overrideRedirect ) :
            parent( parent ), originX( originX ), originY( originY ), 
            width( width ), height( height ), background( background ),
            borderWidth( borderWidth ), border( border ) {

   screen_ = parent->screen();
   initialize( overrideRedirect );
}

void XX::Window::initialize( bool overrideRedirect ) {
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

   attributes.border_pixel     = border->getPixel();
   mask |= CWBorderPixel;
   attributes.background_pixel = background->getPixel();
   mask |= CWBackPixel;
   attributes.override_redirect = (overrideRedirect) ? True: False;
   mask |= CWOverrideRedirect;

   xid = XCreateWindow( this->screen()->display()->xDisplay(), 
         parent->getXID(), 
         originX, originY, width, height, borderWidth,
         screen()->colorDepth(),
         InputOutput, CopyFromParent, mask, &attributes );

   this->screen()->addWindow( this );
}

//== Accessors ================================================================

/**
 * Get the X11 Atom needed to trap the close event for this window.
 */
Atom XX::Window::getCloseAtom() {
   Atom closeAtom=XInternAtom( this->screen()->display()->xDisplay(), 
         "WM_DELETE_WINDOW", True);
   XSetWMProtocols( this->screen()->display()->xDisplay(), this->getXID(), 
         &closeAtom, 1);
   return closeAtom;
}

//== Operations ===============================================================

void XX::Window::open( bool immediately ) {
   XMapWindow( this->screen()->display()->xDisplay(), getXID() );
   is_open = true;
   if (immediately) {
      this->screen()->display()->flush();
   }
}

void XX::Window::close( bool immediately ) {
   XUnmapWindow( this->screen()->display()->xDisplay(), getXID() );
   is_open = false;
   if (immediately) {
      this->screen()->display()->flush();
   }
}

void XX::Window::listenFor( unsigned long events ) {
   eventMask = events;
   XSelectInput( this->screen()->display()->xDisplay(), getXID(), events );
}

XEvent *XX::Window::getNextEvent( XEvent *event, bool block ) {
   if (block) {
      XWindowEvent( this->screen()->display()->xDisplay(), getXID(), eventMask, event );
   } else if (!XCheckWindowEvent( this->screen()->display()->xDisplay(), getXID(), 
            eventMask, event )) {
      return NULL;
   }
   return event;
}

XEvent *XX::Window::getNextEvent( XEvent *event, unsigned long eventTypes, 
      bool block ) {
   if (block) {
      XWindowEvent( this->screen()->display()->xDisplay(), getXID(), eventTypes, event );
   } else if (!XCheckWindowEvent( this->screen()->display()->xDisplay(), getXID(), 
            eventTypes, event )) {
      return NULL;
   }
   return event;
}
