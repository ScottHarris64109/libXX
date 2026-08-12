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

#include "XXDisplay.hh"
#include "XXWindow.hh"
#include "XXColor.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a XXDisplay object.
 */

XXDisplay::~XXDisplay( ) {
   if (display != NULL) {
      XCloseDisplay( display );
   }

   if (rootWindow != NULL) {
      delete[] rootWindow;
   }

}

/**
 *  Initialize a XXDisplay object.  This is to be called by constructors only.
 *
 * @param x X.
 * @return output.
 * @throws Exception if problem.
 */

void XXDisplay::init( void ) {
   display = XOpenDisplay( this->name_ );
   rootWindow = new XXWindow[ this->screenCount() ];
   for (int screen = 0; screen < this->screenCount(); screen++) {
      rootWindow[ screen ].makeRoot( this, screen );
   }
}

/**
 *  Connect to the default X11 Display.
 */

XXDisplay::XXDisplay( ) {
   this->name_ = NULL;
   init();
}

/**
 *  Connect to the named X11 Display.  If no name is given, the default
 *  display (typically "localhost:0.0") will be connected.
 */

XXDisplay::XXDisplay( const char *displayName ) {
   if ((displayName != NULL) && (strlen(displayName) == 0)) {
      this->name_ = NULL;
   } else {
      this->name_ = displayName;
   }
   init();
}

/**
 *  Connect to the named X11 Display.  If no name is given, the default
 *  display (typically "localhost:0.0") will be connected.
 */

XXDisplay::XXDisplay( string displayName ) {
   this->name_ = (displayName == "") ? NULL: displayName.c_str();
   init();
}

//== Operations ===============================================================

/**
 *  Return the validated screen number or throw an exception.  
 *  If the screen number is -1, return the default screen number for 
 *  the display.
 */
int XXDisplay::validScreen( int screen ) const {
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
void XXDisplay::flush( void ) {
   XFlush( this->xDisplay() );
}

/**
 *  Get the next pending event, but do not remove it from the queue.
 */
XEvent *XXDisplay::peekNextEvent( XEvent *event ) {
   XPeekEvent( this->xDisplay(), event );
   return event;
}

/**
 *  Get the next pending event.
 */
XEvent *XXDisplay::getNextEvent( XEvent *event, bool block ) {
   if (!block && !countPendingEvents()) {
      return NULL;
   }
   XNextEvent( this->xDisplay(), event );
   return event;
}

/**
 *  Get the next event whose type is found in the mask.
 */
XEvent *XXDisplay::getNextEvent( XEvent *event, unsigned long eventTypes, 
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
int XXDisplay::countPendingEvents( bool flushQueue ) {
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

string XXDisplay::name( void ) const {
   string output( XDisplayName( this->name_ ) );
   return output;
}

/**
 *  Return the value of the X11 vendor name.
 *
 * @return the X11 vendor name.
 */

string XXDisplay::vendorName( void ) const {
   string output( ServerVendor( display ) );
   return output;
}

/**
 *  Return the value of the X11 vendor release.
 *
 * @return the X11 vendor release.
 */

int XXDisplay::vendorRelease( void ) const {
   return VendorRelease( display );
}

/**
 *  Return the value of the X11 protocol version.
 *
 * @return the X11 protocol version.
 */

int XXDisplay::protocolVersion( void ) const {
   return ProtocolVersion( display );
}

/**
 *  Return the value of the X11 protocol revision.
 *
 * @return the X11 protocol revision.
 */

int XXDisplay::protocolRevision( void ) const {
   return ProtocolRevision( display );
}


/**
 *  Return the value of the X11 protocol revision.
 *
 * @return the X11 protocol revision.
 */

int XXDisplay::defaultScreen( void ) const {
   return DefaultScreen( display );
}


/**
 *  Return the value of the X11 protocol revision.
 *
 * @return the X11 protocol revision.
 */

int XXDisplay::screenCount( void ) const {
   return ScreenCount( display );
}


/**
 *  Return the width of the screen in pixels.
 *
 * @param screen screen number
 * @return the screen width
 */

int XXDisplay::width( int screen ) const {
   return DisplayWidth( display, validScreen( screen ) );
}

/**
 *  Return the height of the screen in pixels.
 *
 * @param screen screen number
 * @return the screen height
 */

int XXDisplay::height( int screen ) const {
   return DisplayHeight( display, validScreen( screen ) );
}

/**
 *  Return the number of color bits for the screen.
 *
 * @param screen screen number
 * @return the screen's color depth.
 */

int XXDisplay::colorDepth( int screen ) const {
   return DefaultDepth( display, validScreen( screen ) );
}

XXWindow *XXDisplay::root( int screen ) const {
   return &rootWindow[ validScreen( screen ) ];
}

void XXDisplay::addWindow( XXWindow *w ) {
   window_[ w->getXID() ] = w;
}

void XXDisplay::removeWindow( XXWindow *w ) {
   window_.erase( w->getXID() );
}

/**
 *  Look up a color by name.
 *
 * @return the XXColor or NULL
 */

XXColor *XXDisplay::getColor( const char *name, int screen ) {
   XColor definition, hardwareColor;
   int found = 0;
   char softName[ 255 ];
   strcpy( softName, name );
   XXColor *color = NULL;

   found = XLookupColor( this->xDisplay(), 
         DefaultColormap( this->xDisplay(), validScreen( screen ) ),
         softName, &definition, &hardwareColor );
   if (found == 0) {
      //throw std::runtime_error( "No such color." );
   } else {
      color = new XXColor( definition.red, definition.green, definition.blue );
   }

   return color;
}

/**
 *  Look up a color by name.
 *
 * @return the XXColor or NULL
 */

XXColor *XXDisplay::getColor( const string name, int screen ) {
   return this->getColor( name.c_str(), screen );
}
