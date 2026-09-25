/*#############################################################################
 * XX::Display.cc -- Display card/server.
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
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>

#include <X11/Xlib.h>

#include "Display.hh"
#include "Screen.hh"
#include "Window.hh"
#include "Color.hh"
#include "Font.hh"

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
      while (!fontByName.empty()) {
         XX::Font *font = this->fontByName.begin()->second;
         this->fontByName.erase( this->fontByName.begin()->first );
         delete font;
      }
      XCloseDisplay( this->xdisplay );
   }

   if (screen_ != nullptr) {
      delete[] screen_;
   }
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
 *  Connect to the named X11 Display.  If no name is given, the default
 *  display (typically "localhost:0.0") will be connected.
 */

XX::Display::Display( std::string displayName ) : name_( displayName ) {

   if (!XX::X11_hasBeenInitialized) {
      XSetErrorHandler( XX::criticalErrorHandler );
      XSetIOErrorHandler( XX::fatalErrorHandler );
      XX::X11_hasBeenInitialized = true;
   }

   this->xdisplay = XOpenDisplay( this->name_.c_str() );
   this->name_ = DisplayString( this->xdisplay );
   screen_ = (XX::Screen **)calloc( this->screenCount(), sizeof(XX::Screen *) );
   if (!screen_)
   {
      fprintf( stderr, "Cannot allocate %d Screens for Display \"%s\".\n",
            this->screenCount(), this->name().c_str() );
      std::bad_alloc exception;
      throw exception;
   }
   for (int s = 0; s < this->screenCount(); s++) {
      screen_[s] = new XX::Screen( this, s );
   }
}

//== Accessors ================================================================

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
 * @return the number of screens available.
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

/**
 * Return a vector containing the available font names.  The optional 
 * <code>pattern</code> can be used as a filter where "*" = one or more 
 * characters and "?" is a single character wildcard.
 */
std::vector<std::string> XX::Display::fontNames( std::string pattern ) {
   std::vector<std::string> names;
   char **foundName = nullptr;
   int foundNames = 0, n = 0;

   foundName = XListFonts( this->xDisplay(), pattern.c_str(), INT_MAX, 
         &foundNames );
   if (foundName) {
      for (n = 0; n < foundNames; n++) {
         names.push_back( foundName[n] );
      }
      XFreeFontNames( foundName );
      foundName = nullptr;
   }

   std::sort( names.begin(), names.end() );
   return names;
}

/**
 * Return a single font by name.  The server will load it if needed.
 * <p>
 * The server will automatically release all loaded fonts when it shuts down.
 * The <code>freeFont(*)</code> methods can be used to manually unload fonts
 * and free up server resources.
 */
XX::Font *XX::Display::getFont( const std::string fontName ) {
   XX::Font *font = this->fontByName[ fontName ];

   if (!font) {
      font = new XX::Font( this, fontName );
      this->fontByName[ font->name() ] = font;
   }

   return font;
}

/**
 * Unload a font that is no longer needed.  
 */
XX::Font *XX::Display::freeFont( XX::Font *font ) {
   this->fontByName.erase( font->name() );
   delete font;
   return nullptr;
}

/**
 * Unload a font that is no longer needed.  
 */
XX::Font *XX::Display::freeFont( const std::string fontName ) {
   XX::Font *font = this->fontByName[ fontName ];
   return this->freeFont( font );
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
 *  If countPendingEvents() is zero, this method will block until an event is 
 *  queued.
 */
XEvent XX::Display::peekNextEvent( ) {
   XEvent event;
   XPeekEvent( this->xDisplay(), &event );
   return event;
}

/**
 *  Get the next pending event.
 *  If countPendingEvents() is zero, this method will block until an event is 
 *  queued.
 */
XEvent XX::Display::getNextEvent( ) {
   XEvent event;
   XNextEvent( this->xDisplay(), &event );
   return event;
}

/**
 *  Get the next event whose type is found in the mask.
XEvent *XX::Display::getNextEvent( unsigned long eventTypes, bool blocking ) {
   XEvent event;

   if (blocking) {
      XMaskEvent( this->xDisplay(), eventTypes, &event );
   } else if (!XCheckMaskEvent( this->xDisplay(), eventTypes, &event )) {
      return NULL;
   }
   return event;
}
 */

/**
 *  Get the number of pending events.
 */
int XX::Display::countPendingEvents( bool flushQueue ) {
   if (flushQueue) {
      flush();
   }
   return XPending( this->xDisplay() );
}

bool XX::Display::dispatch( XEvent& event ) {
   XX:Window *recipient = this->window[ event.xany.window ];
   bool dispatched = false;

   if (recipient) {
      dispatched = recipient->actOn( event );
   }

   return dispatched;
}

