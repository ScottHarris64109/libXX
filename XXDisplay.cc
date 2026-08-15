/*#############################################################################
 * XXDisplay.cc -- Display card/server.
 *
 *  Copyright (C) 2012,2016,2026 by Scott Harris.  
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
#include <string.h>
#include <string>
#include <stdexcept>
#include <iostream>

#include "XXDisplay.hh"
#include "XXScreen.hh"
#include "XXColor.hh"

#define BUFFER_SIZE 255

namespace XX {
static bool X11_hasBeenInitialized = false;

extern "C" int criticalErrorHandler( ::Display *xdisplay, 
      ::XErrorEvent *error );
extern "C" int fatalErrorHandler( ::Display *xdisplay );
void initializeX11();
};

//== Constructors =============================================================

/**
 *  Destroy and deallocate a Display object.
 */

XX::Display::~Display( ) {
   if (this->xdisplay != nullptr) {
      XCloseDisplay( this->xdisplay );
   }

   if (screen_ != nullptr) {
      delete[] screen_;
   }
}

/**
 *  Initialize a Display object.  This is to be called by constructors only.
 *
 * @param x X.
 * @return output.
 * @throws Exception if problem.
 */

void XX::Display::init( void ) {
   if (!XX::X11_hasBeenInitialized) {
      XX::initializeX11();
   }

   this->xdisplay = XOpenDisplay( this->name_ );
   this->name_ = DisplayString( this->xdisplay );
   screen_ = (XX::Screen **)calloc( this->screenCount(), sizeof(XX::Screen *) );
   if (!screen_)
   {
            this->screenCount(), this->name_ );
      std::bad_alloc exception;
      throw exception;
   }
   for (int s = 0; s < this->screenCount(); s++) {
      screen_[s] = new XX::Screen( this, s );
   }
}

/*
 *  Initialize the X11 system (actually, our interaction with it).  This 
 *  should be called before the first Display object is fully instantiated 
 *  and never needs to be called again.
 */
void XX::initializeX11() {
      XSetErrorHandler( XX::criticalErrorHandler );
      XSetIOErrorHandler( XX::fatalErrorHandler );

      XX::X11_hasBeenInitialized = true;
}

extern "C" int XX::criticalErrorHandler( ::Display *xdisplay, 
      ::XErrorEvent *error ) {
   char text[ BUFFER_SIZE+1 ];

   XGetErrorText( xdisplay, error->error_code, text, BUFFER_SIZE );

   fprintf( stderr, "X11 error:  %s\n", text );
   fprintf( stderr, "   Request Code: %d.%d   Error Code: %d\n", 
         error->request_code, error->minor_code, error->error_code );
   fprintf( stderr, "   Resource ID: %ld on display %s.\n", 
         error->resourceid, DisplayString( xdisplay ) );
   return 0;
}

extern "C" int XX::fatalErrorHandler( ::Display *xdisplay ) {
   fprintf( stderr, "X Server failed for display %s\n", 
         DisplayString( xdisplay ) );
   exit( EXIT_FAILURE );
}

/**
 *  Connect to the default X11 Display.
 */

XX::Display::Display( ) {
   this->name_ = NULL;
   init();
}

/**
 *  Connect to the named X11 Display.  If no name is given, the default
 *  display (typically "localhost:0.0") will be connected.
 */

XX::Display::Display( const char *displayName ) {
   if ((displayName != NULL) && (strlen(displayName) == 0)) {
      this->name_ = NULL;
   } else {
      this->name_ = displayName;
   }
   this->init();
}

/**
 *  Connect to the named X11 Display.  If no name is given, the default
 *  display (typically "localhost:0.0") will be connected.
 */

XX::Display::Display( std::string displayName ) {
   this->name_ = (displayName == "") ? NULL: displayName.c_str();
   this->init();
}

//== Operations ===============================================================

/**
 *  Flush the display.
 */
void XX::Display::flush( void ) {
   XFlush( this->xDisplay() );
}

/**
 *  Get the next pending event, but do not remove it from the queue.
 */
XEvent *XX::Display::peekNextEvent( XEvent *event ) {
   XPeekEvent( this->xDisplay(), event );
   return event;
}

/**
 *  Get the next pending event.
 */
XEvent *XX::Display::getNextEvent( XEvent *event, bool block ) {
   if (!block && !countPendingEvents()) {
      return NULL;
   }
   XNextEvent( this->xDisplay(), event );
   return event;
}

/**
 *  Get the next event whose type is found in the mask.
 */
XEvent *XX::Display::getNextEvent( XEvent *event, unsigned long eventTypes, 
      bool block ) {
   if (block) {
      XMaskEvent( this->xDisplay(), eventTypes, event );
   } else if (!XCheckMaskEvent( this->xDisplay(), eventTypes, event )) {
      return NULL;
   }
   return event;
}

/**
 *  Get the number of pending events.
 */
int XX::Display::countPendingEvents( bool flushQueue ) {
   if (flushQueue) {
      flush();
   }
   return XPending( this->xDisplay() );
}
//== Accessors ================================================================

/**
 *  Return the value of the X11 display name.
 *
 * @return the X11 display name.
 */

std::string XX::Display::name( void ) const {
   std::string output( XDisplayName( this->name_ ) );
   return output;
}

/**
 *  Return the value of the X11 vendor name.
 *
 * @return the X11 vendor name.
 */

std::string XX::Display::vendorName( void ) const {
   std::string output( ServerVendor( this->xDisplay() ) );
   return output;
}

/**
 *  Return the value of the X11 vendor release.
 *
 * @return the X11 vendor release.
 */

int XX::Display::vendorRelease( void ) const {
   return VendorRelease( this->xDisplay() );
}

/**
 *  Return the value of the X11 protocol version.
 *
 * @return the X11 protocol version.
 */

int XX::Display::protocolVersion( void ) const {
   return ProtocolVersion( this->xDisplay() );
}

/**
 *  Return the value of the X11 protocol revision.
 *
 * @return the X11 protocol revision.
 */

int XX::Display::protocolRevision( void ) const {
   return ProtocolRevision( this->xDisplay() );
}

/**
 *  Return the number of screens associated with this Display.
 *
 * @return the X11 protocol revision.
 */

int XX::Display::screenCount( void ) const {
   return ScreenCount( this->xDisplay() );
}

/**
 *  Return the requested screen or throw an exception.  
 *  If the screen number is -1, return the default screen for 
 *  the display.
 */
XX::Screen *XX::Display::screen( int which ) const {
   int s = which;

   if (which == -1) {
      s = DefaultScreen( this->xDisplay() );
   }
   else if (which < 0) {
      throw std::range_error( "Screen number cannot be less than zero." );
   }
   else if (which >= this->screenCount()) {
      throw std::range_error( "Screen number is too high." );
   }

   return this->screen_[s];
}
