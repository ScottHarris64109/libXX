// XXDisplay.hh
// Copyright (C) 2014,2016,2026 by Scott Harris.  All rights reserved.
/** 
 * \mainpage XX Class Library
 *
 * XX provides C++ wrappers around the X11 library.
 */
#ifndef XXDISPLAY_HH_
#define XXDISPLAY_HH_

#include <string>
#include <map>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>

using namespace std;
class XXWindow;
class XXColor;

/**
 * @brief XXDisplay is an X11 Display object.  
 * In hardware terms it represents a display card driving at least one 
 * display screen.
 * <p>
 * Screens are numbered 0 through screenCount()-1.  If a screen number 
 * parameter is omitted or passed as -1, the default screen -- usually 0 --
 * will be used.
 */
class XXDisplay {
   // XXWindow constructors and destructors need access to 
   // addWindow() and removeWindow().
   friend class XXWindow;

private:
   const char *name_;
   Display *display;
   XXWindow *rootWindow;
   map<XID, XXWindow*> window_;

   void init( void );

   int validScreen( int screen ) const;

   void addWindow( XXWindow *w );
   void removeWindow( XXWindow *w );

protected:

public:
   //== Constructors ===========================================================

   /// Create an XXDisplay for the default X display.
   XXDisplay();

   /// Create an XXDisplay for a named X server.
   XXDisplay( const char *displayName );

   /// Create an XXDisplay for a named X server.
   XXDisplay( string displayName );

   /// Shut down the XXDisplay and release its resources.
   virtual ~XXDisplay();

   //== Accessors ==============================================================

   /// Get the X11 Display pointer wrapped by this XXDisplay.
   inline Display *xDisplay( void ) { return display; }

   /// Get the name of the X server/display.
   string name( void ) const;

   /// Get the X server's vendor's name.
   string vendorName( void ) const;

   /// Get the X server's release number.
   int vendorRelease( void ) const;

   /// Get the X protocol version supported by the X server.
   int protocolVersion( void ) const;

   /// Get the X protocol revision number supported by the X server.
   int protocolRevision( void ) const;

   /// Get the number of screens the display has.
   int screenCount( void ) const;

   /// Get the default screen number for the display.
   int defaultScreen( void ) const;

   /// Get the width of the screen in pixels.
   int width(int screen=-1) const;

   /// Get the height of the screen in pixels.
   int height(int screen=-1) const;

   /// Get the color depth (number of bit planes) supported by the screen.
   int colorDepth(int screen=-1) const;

   /// Return the root XXWindow of the screen.
   XXWindow *root(int screen=-1) const;

   /// Look up an XXWindow by its XID.
   inline XXWindow *window(XID id) { return window_[id]; };

   /// Get a color by name.
   XXColor *getColor( const string name, int screen=-1 );

   /// Get a color by name.
   XXColor *getColor( const char *name, int screen=-1 );

   //== Operations =============================================================

   /// Flush the event queue.
   void flush( void );

   /// Get the next available XEvent.
   XEvent *getNextEvent( XEvent *event, bool block = true );

   /// Get the next available XEvent whose type is in the mask.
   XEvent *getNextEvent( XEvent *event, unsigned long eventTypes,
        bool block = true );

   /// Peek at but do not get the next available XEvent.
   XEvent *peekNextEvent( XEvent *event );

   /// Return the number of XEvents waiting to be processed.
   int countPendingEvents( bool flushQueue = true );

};

#endif /* XXDISPLAY_HH_ */
