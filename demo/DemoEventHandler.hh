// DemoEventHandler.hh 
// Copyright (C) 2026 by Scott Harris.  All rights reserved.

#ifndef DEMO_EVENT_HANDLER_HH_
#define DEMO_EVENT_HANDLER_HH_

#include <string>
#include <iostream>

#include <XX/Window.hh>

#include "Palette.h"
#include "Drawing.hh"

/**
 * @brief DemoEventHandler is a semi-abstract local superclass for the 
 *    EventHandlers in this demo.  It encapsulates the functor method and
 *    modifier key state settings.
 */
class DemoEventHandler: public XX::Window::EventHandler {

private:

protected:
   std::string modState( unsigned int eventState );

public:
   /*
   virtual bool operator()( XX::Window *window, XEvent& event ) { 
      return false; 
   };
    */
};


class MainWindowButtonPress: public DemoEventHandler {
private:
   XX::Window *popup{ nullptr };
   Drawing    *drawing{ nullptr };
public:
   bool operator()( XX::Window *window, XEvent& event );
   void setPopup( XX::Window *popup );
   void setDrawing( Drawing *drawing );
};

class MainWindowButtonRelease: public DemoEventHandler {
public:
   bool operator()( XX::Window *window, XEvent& event );
};

class MainWindowMouseMove: public DemoEventHandler {
private:
   Drawing *drawing{ nullptr };
public:
   bool operator()( XX::Window *window, XEvent& event );
   void setDrawing( Drawing *drawing );
};

class MainWindowKeyPress: public DemoEventHandler {
public:
   bool operator()( XX::Window *window, XEvent& event );
};

class MainWindowDraw: public DemoEventHandler {
protected:
   Palette *palette{ nullptr };
   Drawing *drawing{ nullptr };
public:
   bool operator()( XX::Window *window, XEvent& event );
   void setPalette( Palette *palette );
   void setDrawing( Drawing *drawing );
};

class PopupDraw: public DemoEventHandler {
private:
   std::string msg;
protected:
   Palette *palette{ nullptr };
public:
   bool operator()( XX::Window *window, XEvent& event );
   void setPalette( Palette *palette );
   void setMessage( const std::string message );
   inline std::string message() const { return this->msg; };
};

class PopupClose: public DemoEventHandler {
public:
   bool operator()( XX::Window *window, XEvent& event );
};

#endif //#ifndef DEMO_EVENT_HANDLER_HH_
