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
#include "XXWindow.hh"
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
   if (this->xdisplay != NULL) {
      XCloseDisplay( this->xdisplay );
   }

   if (rootWindow != NULL) {
      delete[] rootWindow;
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
   rootWindow = new XX::Window[ this->screenCount() ];
   for (int screen = 0; screen < this->screenCount(); screen++) {
      rootWindow[ screen ].makeRoot( this, screen );
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
 *  Return the validated screen number or throw an exception.  
 *  If the screen number is -1, return the default screen number for 
 *  the display.
 */
int XX::Display::validScreen( int screen ) const {
   if (screen == -1) {
      return this->defaultScreen();
   }
   if (screen < 0) {
      throw std::range_error( "Screen number cannot be less than zero." );
   }
   if (screen >= screenCount()) {
      throw std::range_error( "Screen number is too high." );
   }
   return screen;
}

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
 *  Return the value of the X11 protocol revision.
 *
 * @return the X11 protocol revision.
 */

int XX::Display::defaultScreen( void ) const {
   return DefaultScreen( this->xDisplay() );
}


/**
 *  Return the value of the X11 protocol revision.
 *
 * @return the X11 protocol revision.
 */

int XX::Display::screenCount( void ) const {
   return ScreenCount( this->xDisplay() );
}


/**
 *  Return the width of the screen in pixels.
 *
 * @param screen screen number
 * @return the screen width
 */

int XX::Display::width( int screen ) const {
   return DisplayWidth( this->xDisplay(), validScreen( screen ) );
}

/**
 *  Return the height of the screen in pixels.
 *
 * @param screen screen number
 * @return the screen height
 */

int XX::Display::height( int screen ) const {
   return DisplayHeight( this->xDisplay(), validScreen( screen ) );
}

/**
 *  Return the number of color bits for the screen.
 *
 * @param screen screen number
 * @return the screen's color depth.
 */

int XX::Display::colorDepth( int screen ) const {
   return DefaultDepth( this->xDisplay(), validScreen( screen ) );
}

XX::Window *XX::Display::root( int screen ) const {
   return &rootWindow[ validScreen( screen ) ];
}

void XX::Display::addWindow( XX::Window *w ) {
   window_[ w->getXID() ] = w;
}

void XX::Display::removeWindow( XX::Window *w ) {
   window_.erase( w->getXID() );
}

/**
 *  Look up a color by name.
 *
 * @return the Color or NULL
 */

XX::Color *XX::Display::getColor( const char *name, int screen ) {
   XColor definition, hardwareColor;
   int found = 0;
   char softName[ 255 ];
   strcpy( softName, name );
   XX::Color *color = NULL;

   found = XLookupColor( this->xDisplay(), 
         DefaultColormap( this->xDisplay(), validScreen( screen ) ),
         softName, &definition, &hardwareColor );
   if (found == 0) {
      //throw std::runtime_error( "No such color." );
   } else {
      color = new XX::Color( definition.red, definition.green, definition.blue );
   }

   return color;
}

/**
 *  Look up a color by name.
 *
 * @return the Color or NULL
 */

XX::Color *XX::Display::getColor( const std::string name, int screen ) {
   return this->getColor( name.c_str(), screen );
}
