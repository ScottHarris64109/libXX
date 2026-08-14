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

namespace XX {
   class Window;
   class Color;

/**
 * @brief Display is an X11 Display object.  
 * In hardware terms it represents a display card driving at least one 
 * display screen.
 * <p>
 * Screens are numbered 0 through screenCount()-1.  If a screen number 
 * parameter is omitted or passed as -1, the default screen -- usually 0 --
 * will be used.
 */
class Display {
   // Window constructors and destructors need access to 
   // addWindow() and removeWindow().
   friend class Window;

private:
   const char *name_;
   ::Display *xdisplay;
   Window *rootWindow;
   std::map<XID, Window*> window_;

   void init( void );

   int validScreen( int screen ) const;

   void addWindow( Window *w );
   void removeWindow( Window *w );

protected:

public:
   //== Constructors ===========================================================

   Display();
   Display( const char *displayName );
   Display( std::string displayName );

   virtual ~Display();

   //== Accessors ==============================================================

   /// Get the X11 Display pointer wrapped by this Display.
   inline ::Display *xDisplay( void ) const { return xdisplay; }

   std::string name( void ) const;
   std::string vendorName( void ) const;
   int vendorRelease( void ) const;
   int protocolVersion( void ) const;
   int protocolRevision( void ) const;
   int screenCount( void ) const;
   int defaultScreen( void ) const;
   int width(int screen=-1) const;
   int height(int screen=-1) const;
   int colorDepth(int screen=-1) const;

   Window *root(int screen=-1) const;

   /// Look up a Window by its XID.
   inline Window *window(XID id) { return window_[id]; };

   Color *getColor( const std::string name, int screen=-1 );
   Color *getColor( const char *name, int screen=-1 );

   //== Operations =============================================================

   void flush( void );
   XEvent *getNextEvent( XEvent *event, bool block = true );
   XEvent *getNextEvent( XEvent *event, unsigned long eventTypes,
        bool block = true );
   XEvent *peekNextEvent( XEvent *event );
   int countPendingEvents( bool flushQueue = true );

}; // class
}; // namespace

#endif /* XXDISPLAY_HH_ */
