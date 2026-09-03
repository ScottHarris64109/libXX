// XX::Window.hh -- XX Window class header.
// Copyright (C) 2014,2016,2026 by Scott Harris.  All rights reserved.

#ifndef XX_WINDOW_HH_
#define XX_WINDOW_HH_

#include <unordered_map>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "Display.hh"
#include "Screen.hh"
#include "Drawable.hh"
#include "Color.hh"

namespace XX {
   class PixMap;
   class Window;

   typedef bool (*EventHandler)( Window *, XEvent& );

/**
 * @brief Window is a GUI window.
 *
 * A Window can only exist within the context of its parent Window or Screen.
 * (A Window created directly from a Screen is actually a child of the Screen's 
 * root Window, which is only accessible by the Screen itself).  When a Window 
 * is closed or destroyed, all of its children are closed and destroyed as well.
 * <p>
 * Windows are passed around as raw pointers.  Applications can create and
 * destroy Windows as needed and can let Window pointers go out of scope without
 * worrying about resource leakage.  Use-after-free errors are still possible, 
 * however.
 * <p>
 * <b><code> typedef bool (*EventHandler)( Window *, XEvent& ); </code></b>
 * <p>
 * An EventHandler is a callback function for processing XEvents that happen 
 * to the Window.  It returns <code>true</code> if it processes the event and 
 * <code>false</code> if it rejects or ignores it.  An unprocessed event may 
 * be handed off to another resource.
 * <p>
 * EventHandlers are assigned using <code>setAction()</code>.
 */

class Window : public XX::Drawable {
   // Screen needs the root Window constructor.
   friend class Screen;

private:
   XX::Screen *screen_;
   Window  *parent;
   std::unordered_map<XID, Window*> children;
   std::unordered_map<int, EventHandler> reaction;
   XX::Color   *background;
   XX::Color   *border;
   Atom       closeAtom;
   bool       is_open;
   int        originX;
   int        originY;
   int        borderWidth;
   unsigned long eventMask;

   Window( XX::Screen *s );  // Screen root

   void initialize( bool overrideRedirect, XX::PixMap *icon );

   unsigned long maskForEventType( int eventType );

protected:

public:
   //== Constructors ===========================================================

   virtual ~Window();

   /**
    * Create a Window as a child of the Screen's root Window.  
    * Defaulted parameters will be inherited from the parent.  
    * <p>
    * The <code>overrideRedirect</code> flag is used for borderless 
    * popup Windows.  Its use will be refined in the future and it may be 
    * renamed.
    * <p>
    * The <code>icon</code> PixMap must be created from the same Screen or 
    * one of its Windows.
    */
   Window( XX::Screen *screen, int atX, int atY, int width, int height, 
        XX::Color *background=nullptr, int borderWidth=-1, 
        XX::Color *borderColor=nullptr, bool overrideRedirect=false,
        XX::PixMap *icon=nullptr );

   /**
    * Create a Window as a child of an existing Window.  
    * Defaulted parameters will be inherited from the parent.  
    * <p>
    * The <code>overrideRedirect</code> flag is used for borderless 
    * popup Windows.  Its use will be refined in the future and it may be 
    * renamed.
    * <p>
    * The <code>icon</code> PixMap must be created from the same Screen or 
    * one of its Windows.
    */
   Window( Window *parent, int atX, int atY, int width, int height, 
        XX::Color *background=nullptr, int borderWidth=-1, 
        XX::Color *borderColor=nullptr, bool overrideRedirect=false,
        XX::PixMap *icon=nullptr );

   //== Accessors ==============================================================

   inline XX::Screen *screen( void ) const { return this->screen_; }
   inline XX::Color *getBackground( void ) const { return background; }
   inline bool isOpen( void ) const { return is_open; }

   //== Operations =============================================================

   void open( bool immediately=false );
   void close( bool immediately=false );

   /**
    *  Assign an event handler for an event type.
    */
   void setAction( int eventType, EventHandler action );

   /**
    * Execute the event handler on the event.  Return <code>true</code> 
    * if the event was processed.
    */
   bool actOn( XEvent& event );

   XEvent *getNextEvent( XEvent *event, bool block=true );
   XEvent *getNextEvent( XEvent *event, unsigned long eventTypes, 
         bool block=true );
}; // class
}; // namespace

#endif /* XX_WINDOW_HH_ */
