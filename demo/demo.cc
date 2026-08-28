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
#include <XX/GC.hh>

#define BUFFER_SIZE 255

typedef struct {
   char *displayName;
   char *title;
   char *foregroundColorName;
   char *backgroundColorName;
   char *borderColorName;
   char *fontName;
   int borderWidth;
   int x;
   int y;
   int height;
   int width;
   bool iconic;
} WindowPreferences;

bool finished = false;

std::multimap<std::string,std::string> parse_args( int argc, char *argv[] );
std::string getArg( const std::multimap<std::string,std::string> &arg, 
      std::string key, int index=0, std::string defaultValue = "" );
void scribble( XX::GC *gc, XX::Color *color1, XX::Color *color2 );
void loop( XX::Display *display, XX::Window *window, XX::GC *gc, XX::Font *font, 
      XX::Color *fg1, XX::Color *fg2 );
std::string modState( unsigned int eventState );
extern "C" void signalHandler( int theSignal );

int main( int argc, char *argv[] ) {
   std::multimap<std::string,std::string> arg = parse_args( argc, argv );
   std::string displayName = getArg( arg, "-display" );
   std::string fg1name = getArg( arg, "-foreground", 0, "black" );
   std::string fg2name = getArg( arg, "-foreground", 1, "red" );
   std::string bgname  = getArg( arg, "-background", 0, "white" );
   XX::Display *display = nullptr;
   XX::Window *window = nullptr;
   XX::PixMap *icon = nullptr;
   XX::Color *fg1   = nullptr;
   XX::Color *fg2 = nullptr;
   XX::Color *background = nullptr;
   XX::GC *gc = nullptr;
   XX::Font *font = nullptr;

   signal( SIGABRT, signalHandler );
   signal( SIGTERM, signalHandler );

   std::cout << 
      "libXX demo  Copyright (C) 2014,2016,2026  Scott Harris\n\n"
      "This program comes with ABSOLUTELY NO WARRANTY. \n"
      "This is free software, and you are welcome to redistribute it \n"
      "under certain conditions.  (See the GNU GPL version 3 for details).\n\n";

   display = new XX::Display( displayName );
   std::cout << "Got display.\n";
   std::cout << display->vendorName() << " | " << display->name() <<"\n";
   std::cout << display->screenCount() << " screens.\n";
   for (int s=0; s < display->screenCount(); s++)
   {
      std::cout << "  " << s << ". " << display->screen( s )->width() 
         << " x " << display->screen( s )->height() 
         << " x " << display->screen( s )->colorDepth() << "\n";
   }

   std::cout << "Getting colors.\n";
   background = display->screen()->getColor( bgname );
   fg1 = display->screen()->getColor( fg1name );
   fg2 = display->screen()->getColor( fg2name );
   std::cout << "Got colors.\n";

   icon = new XX::PixMap( display->screen(), 24, 24 );
   gc = new XX::GC( icon );
   gc->setForeground( background );
   gc->fillRectangle( 0,0, 24,24 );
   gc->setBackground( background );
   gc->setForeground( fg1 );
   gc->drawLine( 0, 0, 16, 12 );
   gc->drawLine( 16, 12, 0, 24 );
   gc->setForeground( fg2 );
   gc->drawLine( 24, 0, 8, 12 );
   gc->drawLine( 8, 12, 24, 24 );
   delete gc;
   gc = nullptr;

   window = new XX::Window( display->screen(), 500, 100, 500, 500, background, 
       -1, nullptr, false, icon );
   std::cout << "Got window.\n";

   gc = new XX::GC( window );

   font = new XX::Font( display, "variable" );
   gc->setFont( font );

   window->open( true );

   scribble( gc, fg1, fg2 );

   loop( display, window, gc, font, fg1, fg2 );

   window->close( true );

   delete font;
   delete gc;
   delete icon;
   delete window;
   delete display;
   exit( EXIT_SUCCESS );
}

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

std::string getArg( const std::multimap<std::string,std::string> &arg, 
      std::string key, int index, std::string defaultValue ) {
   if (arg.count( key ) <= index) return defaultValue;

   auto it = arg.find(key);
   for (int step = 0; step < index; step++) it++;
   return it->second;
}

void loop( XX::Display *display, XX::Window *window, XX::GC *gc, XX::Font *font, 
      XX::Color *fg1, XX::Color *fg2 ) {
   XEvent event;
   Atom windowClosed;
   char keyBuffer[ BUFFER_SIZE+1 ];
   XComposeStatus composeStatus;
   KeySym keySym;
   int chars = 0;

   window->listenFor( ButtonPressMask | ButtonReleaseMask | 
         KeyPressMask | KeyReleaseMask |
         ExposureMask | StructureNotifyMask );
   windowClosed = window->getCloseAtom();

   while (!finished) {
      display->getNextEvent( &event );
      // window->getNextEvent( &event ); // Doesn't get the WM ClientMessage.
      switch( event.type ) {

         case ClientMessage:
            if (event.xclient.data.l[0] == windowClosed) {
               std::cout << "Main window closed.\n";
               finished = true;
            }
            break;

         case Expose:
            std::cout << "Exposed.\n";
            scribble( gc, fg1, fg2 );
            break;

         case MapNotify:
            std::cout << "Mapped.\n";
            scribble( gc, fg1, fg2 );
            break;

         case ConfigureNotify:
            std::cout << "Reconfigured.\n";
            scribble( gc, fg1, fg2 );
            break;

         case ButtonPress:
            std::cout << "Pressed " << modState( event.xbutton.state ) << " ";
            std::cout << "button " << event.xbutton.button << ".\n";
            break;
    // Button 4 = Scroll up
    // Button 5 = Scroll down

         case ButtonRelease:
            std::cout << "Released " << modState( event.xbutton.state ) << " ";
            std::cout << "button " << event.xbutton.button << ".\n";
            break;

         case KeyPress:
            chars = XLookupString( &event.xkey, keyBuffer, BUFFER_SIZE, 
                  &keySym, &composeStatus );
            std::cout << "Pressed " << modState( event.xkey.state );
            if ((chars > 0) && (keySym >= ' ') && (keySym <= '~')) {
               if (event.xkey.state & ControlMask) {
                  char letter = (char) keySym;
                  std::cout << "\"^" << letter << "\" #" 
                       << std::hex << (int) keySym << ".\n";
               } else {
                  std::cout << "\"" << keyBuffer[0] << "\".\n";
               }
            } else {
               switch( keySym ) {
                  case XK_Return:
                     std::cout << "Return";
                     break;
                  case XK_BackSpace:
                     std::cout << "BackSpace";
                     break;
                  case XK_Escape:
                     std::cout << "Escape";
                     finished = true;
                     break;
                  case XK_Delete:
                     std::cout << "Delete";
                     break;
                  case XK_Up:
                     std::cout << "Up";
                     break;
                  case XK_Down:
                     std::cout << "Down";
                     break;
                  case XK_Right:
                     std::cout << "Right";
                     break;
                  case XK_Left:
                     std::cout << "Left";
                     break;
                  case XK_Home:
                     std::cout << "Home";
                     break;
                  case XK_Prior:
                     std::cout << "Page Up";
                     break;
                  case XK_Next:
                     std::cout << "Page down";
                     break;
                  case XK_Begin:
                     std::cout << "Begin";
                     break;
                  case XK_End:
                     std::cout << "End";
                     break;
                  case XK_Insert:
                     std::cout << "Insert";
                     break;
                  case XK_Help:
                     std::cout << "Help";
                     break;
                  case XK_Menu:
                     std::cout << "Menu";
                     break;
                  case XK_Print:
                     std::cout << "Print";
                     break;
                  case XK_Tab:
                     std::cout << "Tab";
                     break;
                  case XK_Break: // Ctrl-Pause/Break
                     std::cout << "Break";
                     break;
                  case XK_Sys_Req: // Unreachable?
                     std::cout << "SysReq";

                  case XK_Control_L:
                     std::cout << "Control L";
                     break;
                  case XK_Control_R:
                     std::cout << "Control R";
                     break;
                  case XK_Shift_L:
                     std::cout << "Shift L";
                     break;
                  case XK_Shift_R:
                     std::cout << "Shift R";
                     break;
                  case XK_Alt_L:
                     std::cout << "Alt L";
                     break;
                  case XK_Alt_R:
                     std::cout << "Alt R";
                     break;
                  case XK_Super_L: // Window key
                     std::cout << "Super L";
                     break;
                  case XK_Super_R: // Window key
                     std::cout << "Super R";
                     break;
                  case XK_Hyper_L:
                     std::cout << "Hyper L";
                     break;
                  case XK_Hyper_R:
                     std::cout << "Hyper R";
                     break;
                  case XK_Meta_L:
                     std::cout << "Meta L";
                     break;
                  case XK_Meta_R:
                     std::cout << "Meta R";
                     break;
                  case XK_Caps_Lock:
                     std::cout << "Caps Lock";
                     break;
                  case XK_Num_Lock:
                     std::cout << "Num Lock";
                     break;
                  case XK_Pause:
                     std::cout << "Pause";
                     break;
                  case XK_Scroll_Lock:
                     std::cout << "Scroll Lock";
                     break;

                  case XK_F1:
                     std::cout << "F1";
                     break;
                  case XK_F2:
                     std::cout << "F2";
                     break;
                  case XK_F3:
                     std::cout << "F3";
                     break;
                  case XK_F4:
                     std::cout << "F4";
                     break;
                  case XK_F5:
                     std::cout << "F5";
                     break;
                  case XK_F6:
                     std::cout << "F6";
                     break;
                  case XK_F7:
                     std::cout << "F7";
                     break;
                  case XK_F8:
                     std::cout << "F8";
                     break;
                  case XK_F9:
                     std::cout << "F9";
                     break;
                  case XK_F10:
                     std::cout << "F10";
                     break;
                  case XK_F11:
                     std::cout << "F11";
                     break;
                  case XK_F12:
                     std::cout << "F12";
                     break;
                  case XK_F13:
                     std::cout << "F13";
                     break;
                  case XK_F14:
                     std::cout << "F14";
                     break;
                  case XK_F15:
                     std::cout << "F15";
                     break;

                  case XK_KP_Enter:
                     std::cout << "Keypad Enter";
                     break;
                  case XK_KP_Delete:
                     std::cout << "Keypad Delete";
                     break;
                  case XK_KP_Up:
                     std::cout << "Keypad Up";
                     break;
                  case XK_KP_Down:
                     std::cout << "Keypad Down";
                     break;
                  case XK_KP_Right:
                     std::cout << "Keypad Right";
                     break;
                  case XK_KP_Left:
                     std::cout << "Keypad Left";
                     break;
                  case XK_KP_Home:
                     std::cout << "Keypad Home";
                     break;
                  case XK_KP_Prior:
                     std::cout << "Keypad Page Up";
                     break;
                  case XK_KP_Next:
                     std::cout << "Keypad Page down";
                     break;
                  case XK_KP_Begin:
                     std::cout << "Keypad Begin";
                     break;
                  case XK_KP_End:
                     std::cout << "Keypad End";
                     break;
                  case XK_KP_Insert:
                     std::cout << "Keypad Insert";
                     break;
                  case XK_KP_Equal:
                     std::cout << "Keypad Equal";
                     break;
                  case XK_KP_Multiply:
                     std::cout << "Keypad *";
                     break;
                  case XK_KP_Divide:
                     std::cout << "Keypad /";
                     break;
                  case XK_KP_Add:
                     std::cout << "Keypad +";
                     break;
                  case XK_KP_Subtract:
                     std::cout << "Keypad -";
                     break;
                  case XK_KP_Decimal:
                     std::cout << "Keypad .";
                     break;
                  case XK_KP_0:
                     std::cout << "Keypad 0";
                     break;
                  case XK_KP_1:
                     std::cout << "Keypad 1";
                     break;
                  case XK_KP_2:
                     std::cout << "Keypad 2";
                     break;
                  case XK_KP_3:
                     std::cout << "Keypad 3";
                     break;
                  case XK_KP_4:
                     std::cout << "Keypad 4";
                     break;
                  case XK_KP_5:
                     std::cout << "Keypad 5";
                     break;
                  case XK_KP_6:
                     std::cout << "Keypad 6";
                     break;
                  case XK_KP_7:
                     std::cout << "Keypad 7";
                     break;
                  case XK_KP_8:
                     std::cout << "Keypad 8";
                     break;
                  case XK_KP_9:
                     std::cout << "Keypad 9";
                     break;

                  default:
                     std::cout << "unrecognized key #" << std::hex 
                           << (int) keySym;
                     /* 1008ff19 Mail
                      * 1008ff2e Home
                      * 1008ff30 Star
                      *
                      * 1008ff31 Pause/Play
                      * 1008ff16 Rewind
                      * 1008ff17 Fast Forward
                      * 1008ff2c Stop
                      *
                      * 1008ff11 Volume Down
                      * 1008ff13 Volume Up
                      * 1008ff12 Mute
                      *
                      * 1008ff32 Music
                      * 1008ff33 Screen/Terminal
                      * 1008ff1d Menu
                      */
                     break;
               }
               std::cout << ".\n";
            }
            break;

         /*
         case KeyRelease:
            chars = XLookupString( &event.xkey, keyBuffer, BUFFER_SIZE, 
                  &keySym, &composeStatus );
            if (chars > 0) {
               std::cout << "Released \"" << keyBuffer[0] << ".\n";
            } else {
               std::cout << "Released a non-ASCII key.\n";
            }
            if (keyBuffer[0] == 'q') {
               finished = true;
            }
            break;
          */
      }
   }

}

void scribble( XX::GC *gc, XX::Color *color1, XX::Color *color2 ) {
   gc->setForeground( color1 );
   gc->drawLine( 10, 10, 50, 50 );
   gc->drawRectangle( 10, 60, 40, 40 );
   gc->drawArc( 60, 10, 100, 100, 0.0, 360.0 );
   gc->setForeground( color2 );
   gc->fillArc( 60, 20, 90, 90, 0.0, 90.0 );
   gc->setForeground( color1 );
   gc->drawText( 10, 400, "Hello, World!" );

   gc->getWindow()->display()->flush();
}

std::string modState( unsigned int eventState ) {
   std::string out="[";
   bool started = false;

   if (eventState & ShiftMask) {
      if (started) out += "+";
      out += "Shift";
      started = true;
   }
   if (eventState & Mod1Mask) { // Meta
      if (started) out += "+";
      out += "Alt";
      started = true;
   }

   if (eventState & ControlMask) {
      if (started) out += "+";
      out += "Ctrl";
      started = true;
   }
   if (eventState & LockMask) {
      if (started) out += "+";
      out += "CapsLock";
      started = true;
   }
   if (eventState & Mod2Mask) {
      if (started) out += "+";
      out += "NumLock";
      started = true;
   }
   if (eventState & Mod3Mask) {
      if (started) out += "+";
      out += "Mod3";
      started = true;
   }
   if (eventState & Mod4Mask) { // Window key
      if (started) out += "+";
      out += "Super";
      started = true;
   }
   if (eventState & Mod5Mask) {
      if (started) out += "+";
      out += "Mod5";
      started = true;
   }
   if (eventState & Button1Mask) {
      if (started) out += "+";
      out += "#1";
      started = true;
   }
   if (eventState & Button2Mask) {
      if (started) out += "+";
      out += "#2";
      started = true;
   }
   if (eventState & Button3Mask) {
      if (started) out += "+";
      out += "#3";
      started = true;
   }
   if (eventState & Button4Mask) { //Up
      if (started) out += "+";
      out += "#4";
      started = true;
   }
   if (eventState & Button5Mask) { //Down
      if (started) out += "+";
      out += "#5";
      started = true;
   }

   if (started) {
      out += "] ";
   } else {
      out = "";
   }

   return out;
}

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
   finished = true;
}
