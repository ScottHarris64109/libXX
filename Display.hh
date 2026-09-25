// XX::Display.hh
// Copyright (C) 2014,2016,2026 by Scott Harris.  All rights reserved.

#ifndef XX_DISPLAY_HH_
#define XX_DISPLAY_HH_

#include <string>
#include <vector>
#include <unordered_map>
#include <X11/Xlib.h>

namespace XX {
   class Screen;
   class Window;
   class Font;

/**
 * @brief Display represents a display card or X11 server driving at least one 
 * screen.
 * <p>
 * Screens are numbered 0 through screenCount()-1.  If a screen number 
 * parameter is omitted or passed as -1, the default screen -- usually 0 --
 * will be used.
 */
class Display {
   // Windows need to add and remove themselves from the Window map.
   friend class Window;

private:
   std::string name_;
   ::Display *xdisplay;
   Screen **screen_;
   std::unordered_map<std::string, Font*> fontByName;

   void init( void );

protected:
   /* Locate a Window for event dispatch.  This map can't be a static member of
    * the Window class because XIDs aren't guaranteed to be unique across 
    * Display instances.
    */
   std::unordered_map<XID, Window*> window;

public:
   //== Constructors ===========================================================

   Display( std::string displayName="" );

   virtual ~Display();

   //== Accessors ==============================================================

   /// Get the X11 Display pointer wrapped by this Display.
   inline ::Display *xDisplay( void ) const { return xdisplay; }

   inline std::string name( void ) const { return this->name_; };
   std::string vendorName( void ) const;
   int vendorRelease( void ) const;
   int protocolVersion( void ) const;
   int protocolRevision( void ) const;

   int screenCount( void ) const;
   Screen *screen( int which=-1 ) const;

   /// List available font names.
   std::vector<std::string> fontNames( std::string pattern="*" );

   /// Get a font by name.
   Font *getFont( const std::string fontName );

   /// Unload a font that is no longer needed.  Returns <code>nullptr</code>.
   Font *freeFont( Font *font );

   /// Unload a font that is no longer needed.  Returns <code>nullptr</code>.
   Font *freeFont( const std::string fontName );

   //== Operations =============================================================

   void flush( void );
   XEvent peekNextEvent( void );
   XEvent getNextEvent( void );
   /*
   XEvent *getNextEvent( XEvent *event, unsigned long eventTypes,
        bool block = true );
    */
   bool dispatch( XEvent& event );
   int countPendingEvents( bool flushQueue = true );

}; // class
}; // namespace

#endif /* XX_DISPLAY_HH_ */
