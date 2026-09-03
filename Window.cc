/*#############################################################################
 * XX::Window.cc -- Window class code.
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

#include "Display.hh"
#include "Screen.hh"
#include "Drawable.hh"
#include "Window.hh"
#include "Color.hh"
#include "PixMap.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a Window object.
 */

XX::Window::~Window( ) {
   while (!children.empty())
   {
      delete this->children.begin()->second;
   }
   // Detach from parent.
   if (this->parent) {
      this->parent->children.erase( this->getXID() );
   }
   // Detach from display.
   this->screen()->display()->window.erase( this->getXID() );
   XDestroyWindow( this->screen()->display()->xDisplay(), this->getXID() );
}

/**
 *  Create a root Window for a Screen.
 */

XX::Window::Window( XX::Screen *scr ) : Drawable( scr->display() ), 
      screen_{ scr } {
   this->parent = nullptr;
   this->is_open = false;
   this->originX = 0;
   this->originY = 0;
   this->height = this->screen()->height();
   this->width  = this->screen()->width();
   this->depth  = this->screen()->colorDepth();
   this->background = this->screen()->getColor( "white" );
   this->borderWidth = 3;
   this->border = this->background;
   this->title_ = "";

   this->xid = RootWindow( this->screen()->display()->xDisplay(), 
         this->screen()->index() );

   this->screen()->display()->window[ this->getXID() ] = this;
   this->eventMask = NoEventMask;
   this->makeContext();
}

XX::Window::Window( XX::Screen *scr,
      int originX, int originY, int width, int height, XX::Color *background,
      int borderWidth, XX::Color *border, bool overrideRedirect, 
      XX::PixMap *icon, std::string title ) : Drawable( scr->display() ),
            screen_( scr ), 
            originX( originX ), originY( originY ), 
            background( background ),
            borderWidth( borderWidth ), border( border ), title_( title ) {

   this->parent = this->screen()->rootWindow();
   this->height = height;
   this->width  = width;
   this->depth  = this->screen()->colorDepth();
   initialize( overrideRedirect, icon );
}

XX::Window::Window( XX::Window *parent, 
      int originX, int originY, int width, int height, XX::Color *background,
      int borderWidth, XX::Color *border, bool overrideRedirect,
      XX::PixMap *icon, std::string title ) : Drawable( parent->display() ),
            parent( parent ), originX( originX ), originY( originY ), 
            background( background ),
            borderWidth( borderWidth ), border( border ), title_( title ) {

   this->screen_ = parent->screen();
   this->height = height;
   this->width  = width;
   this->depth  = this->screen()->colorDepth();
   initialize( overrideRedirect, icon );
}

void XX::Window::initialize( bool overrideRedirect, XX::PixMap *icon ) {
   XSetWindowAttributes attributes;
   unsigned long        mask = 0;

   if (background == nullptr) {
      background = parent->background;
   }
   if (borderWidth < 0) {
      borderWidth = parent->borderWidth;
   }
   if (border == nullptr) {
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

   this->screen()->display()->window[ this->getXID() ] = this;
   this->parent->children[ this->getXID() ] = this;

   if (icon) {
      XWMHints hint;

      hint.icon_pixmap = icon->getXID();
      hint.initial_state = NormalState;
      hint.flags = IconPixmapHint | StateHint;

      XSetWMHints( this->screen()->display()->xDisplay(), this->getXID(), 
         &hint );
   }

   if (!this->title_.empty()) {
      XStoreName( this->screen()->display()->xDisplay(), this->getXID(),
            this->title_.c_str() );
   }

   this->eventMask = NoEventMask;
   this->closeAtom = XInternAtom( this->screen()->display()->xDisplay(), 
         "WM_DELETE_WINDOW", True);
   XSetWMProtocols( this->screen()->display()->xDisplay(), this->getXID(), 
         &this->closeAtom, 1);

   this->makeContext();
}

//== EventHandlers ============================================================

void XX::Window::setAction( int eventType, EventHandler action, void *resource )
{
   unsigned long mask = maskForEventType( eventType );

   if (mask)
   {
      this->eventMask |= mask;
      XSelectInput( this->screen()->display()->xDisplay(), getXID(), 
            this->eventMask );
   }

   this->reaction[ eventType ] = action;
   this->resources[ eventType ] = resource;
}

unsigned long XX::Window::maskForEventType( int eventType ) {
   unsigned long mask = 0L;

   switch( eventType ) {
      case ButtonPress:
         mask = ButtonPressMask;
         break;

      case ButtonRelease:
         mask = ButtonReleaseMask;
         break;

      case CirculateNotify:
      case ConfigureNotify:
      case DestroyNotify:
      case GravityNotify:
      case MapNotify:
      case ReparentNotify:
      case UnmapNotify:
         mask = StructureNotifyMask | SubstructureNotifyMask;
         break;

      case CirculateRequest:
      case ConfigureRequest:
      case MapRequest:
         mask = SubstructureRedirectMask;
         break;

      case ColormapNotify:
         mask = ColormapChangeMask;
         break;

      case CreateNotify:
         mask = SubstructureNotifyMask;
         break;

      case EnterNotify:
         mask = EnterWindowMask;
         break;

      case Expose:
         mask = ExposureMask;
         break;

      case FocusIn:
      case FocusOut:
         mask = FocusChangeMask;
         break;

      case KeyPress:
         mask = KeyPressMask;
         break;

      case KeyRelease:
         mask = KeyReleaseMask;
         break;

      case KeymapNotify:
         mask = KeymapStateMask;
         break;

      case LeaveNotify:
         mask = LeaveWindowMask;
         break;

      case MotionNotify:
         mask = ButtonMotionMask | PointerMotionMask;
         // Button1MotionMask;
         // Button2MotionMask;
         // Button3MotionMask;
         // Button4MotionMask;
         // Button5MotionMask;
         break;

      case PropertyNotify:
         mask = PropertyChangeMask;
         break;

      case ResizeRequest:
         mask = ResizeRedirectMask;
         break;

      case VisibilityNotify:
         mask = VisibilityChangeMask;
         break;

      case ClientMessage:
      case GraphicsExpose:
      case MappingNotify:
      case NoExpose:
      case SelectionClear:
      case SelectionNotify:
      case SelectionRequest:
      default:
         mask = 0L;
         break;
   }

   return mask;
}

void XX::Window::ignore( int eventType ) {
   this->reaction.erase( eventType );
   this->resources.erase( eventType );
}

bool XX::Window::actOn( XEvent& event ) {
   bool handled = false;
   EventHandler action = nullptr;

   if ((event.type == ClientMessage)
         && (event.xclient.window == this->getXID())
         && (event.xclient.data.l[0] == this->closeAtom)) {
      this->close();
      handled = true;
   } else if (action = this->reaction[ event.type ]) { // Assignment intended.
      handled = action( this, event, this->resources[ event.type ] );
   }

   return handled;
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
