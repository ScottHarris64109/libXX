/*#############################################################################
 *  libXX demo -- Demonstrate how to use the XX library.
 *  Copyright (C) 2014,2016,2026  Scott Harris
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *#############################################################################
 */

#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <csignal>
#include <stdexcept>
#include <string>
#include <map>

#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xproto.h>

#include <XX/Display.hh>
#include <XX/Window.hh>
#include <XX/PixMap.hh>
#include <XX/Color.hh>
#include <XX/Font.hh>

#include "Palette.h"
#include "Drawing.hh"
#include "Point.hh"
#include "Line.hh"
#include "DemoEventHandler.hh"

// Used by error handlers to break the main loop.
bool aborted = false;

std::multimap<std::string,std::string> parse_args( int argc, char *argv[] );
std::string getArg( const std::multimap<std::string,std::string> &arg, 
      std::string key, int index=0, std::string defaultValue = "" );
extern "C" void signalHandler( int theSignal );

//==============================================================================
int main( int argc, char *argv[] ) {
   std::multimap<std::string,std::string> arg = parse_args( argc, argv );
   std::string displayName = getArg( arg, "-display" );
   std::string fg1name = getArg( arg, "-foreground", 0, "black" );
   std::string fg2name = getArg( arg, "-foreground", 1, "red" );
   std::string bgname  = getArg( arg, "-background", 0, "white" );

   // XX::EventHandlers
   PopupDraw drawPopup;
   PopupClose closeWindow;
   MainWindowButtonPress   mainWindowButtonPress;
   MainWindowButtonRelease mainWindowButtonRelease;
   MainWindowMouseMove     mainWindowMouseMove;
   MainWindowKeyPress      mainWindowKeyPress;
   MainWindowDraw          drawMainWindow;

   Point a;
   Point b(1,2);
   Point c( {3,4} );
   Point d = a;
   d = {7,8};
   d.move( 5,6 );
   d.move( (int[]){ 1,1 } );

   Line ab(a,b);
   Line cd(c,c);

   cd.setPoint(1,d);
   d.move(-1,-1);

   std::cout << "Point A = " << a << "\n";
   std::cout << "Point B = " << b << "\n";
   std::cout << "Point C = " << c << "\n";
   std::cout << "Point D = " << d << "\n";
   std::cout << "Line AB = " << ab << "\n";
   std::cout << "Line CD = " << cd << "\n";

   signal( SIGABRT, signalHandler );
   signal( SIGTERM, signalHandler );

   std::cout << 
      "libXX demo  Copyright (C) 2014,2016,2026  Scott Harris\n\n"
      "This program comes with ABSOLUTELY NO WARRANTY. \n"
      "This is free software, and you are welcome to redistribute it \n"
      "under certain conditions.  (See the GNU GPL version 3 for details).\n\n";

   XX::Display display( displayName );
   std::cout << "Got display.\n";
   std::cout << display.vendorName() << " | " << display.name() <<"\n";
   if (display.screenCount() > 1) {
      std::cout << display.screenCount() << " screens.\n";
   } else {
      std::cout << display.screenCount() << " screen.\n";
   }
   for (int s=0; s < display.screenCount(); s++)
   {
      std::cout << "  " << s << ". " << display.screen( s )->width() 
         << " x " << display.screen( s )->height() 
         << " x " << display.screen( s )->colorDepth() << "\n";
   }

   Palette palette;
   palette.font = new XX::Font( &display, "variable" );
   palette.background = display.screen()->getColor( bgname );
   palette.color[0]   = display.screen()->getColor( fg1name );
   palette.color[1]   = display.screen()->getColor( fg2name );
   drawMainWindow.setPalette( &palette );
   drawPopup.setPalette( &palette );

   Drawing drawing;
   drawing.setPalette( &palette );
   drawMainWindow.setDrawing( &drawing );
   mainWindowButtonPress.setDrawing( &drawing );
   mainWindowMouseMove.setDrawing( &drawing );

   XX::PixMap *icon = new XX::PixMap( display.screen(), 24, 24 );
   icon->fillRectangle( palette.background, 0,0, 24,24 );
   icon->drawLine( palette.color[0], 0, 0, 16, 12 );
   icon->drawLine( palette.color[0], 16, 12, 0, 24 );
   icon->drawLine( palette.color[1], 24, 0, 8, 12 );
   icon->drawLine( palette.color[1], 8, 12, 24, 24 );

   XX::Window *mainWindow = new XX::Window( display.screen(), 
         500, 100, 500, 500, 
         palette.background, -1, nullptr, false, icon, "libXX demo"  );
   mainWindow->setAction( ButtonPress, &mainWindowButtonPress );
   mainWindow->setAction( ButtonRelease, &mainWindowButtonRelease );
   mainWindow->setAction( MotionNotify, &mainWindowMouseMove );
   mainWindow->setAction( KeyPress, &mainWindowKeyPress );
   mainWindow->setAction( Expose, &drawMainWindow );
   mainWindow->setAction( MapNotify, &drawMainWindow );
   mainWindow->setAction( ConfigureNotify, &drawMainWindow );
   drawing.setWindow( mainWindow );

   XX::Window *popup = new XX::Window( mainWindow, 
         0, 0, 270, 30, palette.background, 3, palette.color[0], true );
   popup->setAction( ButtonPress, &closeWindow );
   popup->setAction( Expose, &drawPopup );
   popup->setAction( MapNotify, &drawPopup );
   popup->setAction( ConfigureNotify, &drawPopup );
   mainWindowButtonPress.setPopup( popup );

   // Main loop
   mainWindow->open( true );

   XEvent event;
   while (!aborted && mainWindow->isOpen()) {
      event = display.getNextEvent( );
      display.dispatch( event );
   }

   mainWindow->close( true );

   delete palette.font;
   delete palette.color[0];
   delete palette.color[1];
   delete palette.background;
   delete icon;
   exit( EXIT_SUCCESS );
}

//------------------------------------------------------------------------------
std::multimap<std::string,std::string> parse_args( int argc, char *argv[] ) {
   std::multimap<std::string,std::string> args;
   std::string key = "";
   std::string value = "";

   for (int a=1; a < argc; a++) {
      if (argv[a][0] == '-') {
         if ((key != "") && (args.count( key ) == 0)) {
            args.insert( std::pair<std::string,std::string>( key, "" ) );
         }
         key = std::string( argv[a] );
         if (key == "-bg")   key = "-background";
         if (key == "-bd")   key = "-bordercolor";
         if (key == "-bw")   key = "-borderwidth";
         if (key == "-fg")   key = "-foreground";
         if (key == "-fn")   key = "-font";
         if (key == "-h")    key = "-help";
         if (key == "-geom") key = "-geometry";
         if (key == "-name") key = "-title";
      } else {
         value = std::string( argv[a] );
         args.insert( std::pair<std::string,std::string>( key, value ));
      }
   }

   return args;
}

//------------------------------------------------------------------------------
std::string getArg( const std::multimap<std::string,std::string> &arg, 
      std::string key, int index, std::string defaultValue ) {
   if (arg.count( key ) <= index) return defaultValue;

   auto it = arg.find(key);
   for (int step = 0; step < index; step++) it++;
   return it->second;
}

//------------------------------------------------------------------------------
extern "C" void signalHandler( int theSignal ) {
   std::string signalName;

   switch( theSignal ) {
      case SIGABRT:
         signalName = "abort";
         break;
      case SIGTERM:
         signalName = "terminate";
         break;
      default:
         signalName = "unexpected #"+theSignal;
         break;
   }

   std::cerr << "Caught " << signalName << " signal.\n";
   aborted = true;
}
