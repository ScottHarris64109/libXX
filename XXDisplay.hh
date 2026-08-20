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
#include <X11/Xlib.h>

namespace XX {
   class Screen;

/**
 * @brief Display wraps an X11 Display object.  
 * It represents a display card or X11 server driving at least one screen.
 * <p>
 * Screens are numbered 0 through screenCount()-1.  If a screen number 
 * parameter is omitted or passed as -1, the default screen -- usually 0 --
 * will be used.
 */
class Display {

private:
   const char *name_;
   ::Display *xdisplay;
   Screen **screen_;

   void init( void );

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

   Screen *screen( int which=-1 ) const;

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
