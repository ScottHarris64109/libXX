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

   typedef bool (*EventHandler)( Window *, XEvent&, void * );

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
 * <b><code>typedef bool (*EventHandler)( Window *, XEvent&, void * );</code></b>
 * <p>
 * An EventHandler is a callback function for processing XEvents that happen 
 * to the Window.  It returns <code>true</code> if it processes the event and 
 * <code>false</code> if it rejects or ignores it.  An unprocessed event may 
 * be handed off to another resource.
 * <p>
 * Parameters to an EventHandler are the Window that executes it, the XEvent 
 * that triggered it, and a pointer to a resource bundle it can access 
 * during execution.  The resource pointer is non-owning; whatever it points
 * to (if anything) must be managed by the application.
 * <p>
 * EventHandlers are assigned using <code>setAction()</code>.
 */

class Window : public XX::Drawable {
   // Screen needs the root Window constructor.
   friend class Screen;

private:
   XX::Screen *screen_;
   Window  *parent;
   std::string title_;
   std::unordered_map<XID, Window*> children;
   std::unordered_map<int, EventHandler> reaction;
   std::unordered_map<int, void *> resources;
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
        XX::PixMap *icon=nullptr, std::string title="" );

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
        XX::PixMap *icon=nullptr, std::string title="" );

   //== Accessors ==============================================================

   inline XX::Screen *screen( void ) const { return this->screen_; }
   inline XX::Color *getBackground( void ) const { return this->background; }
   inline bool isOpen( void ) const { return this->is_open; }
   inline std::string title( void ) const { return this->title_; }

   //== EventHandlers ==========================================================

   /**
    *  Assign an event handler for an event type.
    */
   void setAction( int eventType, EventHandler action, void *resource=nullptr );

   /**
    * Ignopre events of this type.  If an EventHandler and resource bundle have 
    * been assigned to this event type, references to them will be dropped.
    * <p>
    * Some event types such as ClientMessage cannot be ignored.
    */
   void ignore( int eventType );

   /**
    * Execute the event handler on the event.  Return <code>true</code> 
    * if the event was processed.
    */
   bool actOn( XEvent& event );

   //== Operations =============================================================

   void open( bool immediately=false );
   void close( bool immediately=false );

}; // class
}; // namespace

#endif /* XX_WINDOW_HH_ */
