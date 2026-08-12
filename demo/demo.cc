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

#include <XXDisplay.hh>
#include <XXWindow.hh>
#include <XXColor.hh>
#include <XXGC.hh>

#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xproto.h>

using namespace std;

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

multimap<string,string> parse_args( int argc, char *argv[] );
string getArg( const multimap<string,string> &arg, string key, int index=0, 
      string defaultValue = "" );
void scribble( XXGC *gc, XXColor *color1, XXColor *color2 );
void loop( XXDisplay *display, XXWindow *window, XXGC *gc, XXFont *font, 
      XXColor *fg1, XXColor *fg2 );
string modState( unsigned int eventState );
extern "C" int criticalErrorHandler( Display *display, XErrorEvent *error );
extern "C" int fatalErrorHandler( Display *display );
extern "C" void signalHandler( int theSignal );

int main( int argc, char *argv[] ) {
   multimap<string,string> arg = parse_args( argc, argv );
   string displayName = getArg( arg, "-display" );
   string fg1name = getArg( arg, "-foreground", 0, "black" );
   string fg2name = getArg( arg, "-foreground", 1, "red" );
   string bgname  = getArg( arg, "-background", 0, "white" );
   XXDisplay *display;
   XXWindow *window;
   XXColor *fg1   = NULL;
   XXColor *fg2 = NULL;
   XXColor *background = NULL;
   XXGC *gc;
   XXFont *font;

   signal( SIGABRT, signalHandler );
   signal( SIGTERM, signalHandler );

   cout << 
      "libXX demo  Copyright (C) 2014,2016,2026  Scott Harris\n\n"
      "This program comes with ABSOLUTELY NO WARRANTY. \n"
      "This is free software, and you are welcome to redistribute it \n"
      "under certain conditions.  (See the GNU GPL version 3 for details).\n\n";

   display = new XXDisplay( displayName );
   cout << "Got display.\n";
   cout << display->vendorName() << " | " << display->name() <<"\n";
   cout << display->screenCount() << " screens.\n";
   for (int screen=0; screen < display->screenCount(); screen++)
   {
      cout << "  " << screen << ". " << display->width( screen ) 
	      << " x " << display->height( screen ) 
	      << " x " << display->colorDepth( screen ) << "\n";
   }

   XSetErrorHandler( criticalErrorHandler );
   XSetIOErrorHandler( fatalErrorHandler );

   cout << "Getting colors.\n";
   background = display->getColor( bgname );
   fg1 = display->getColor( fg1name );
   fg2 = display->getColor( fg2name );
   cout << "Got colors.\n";

   window = new XXWindow( display, 500, 100, 500, 500, background );
   cout << "Got window.\n";

   gc = new XXGC( window );

   font = new XXFont( display, "variable" );
   gc->setFont( font );

   window->open( true );

   scribble( gc, fg1, fg2 );

   loop( display, window, gc, font, fg1, fg2 );

   window->close( true );

   delete font;
   delete gc;
   delete window;
   delete display;
   exit( EXIT_SUCCESS );
}

multimap<string,string> parse_args( int argc, char *argv[] ) {
   multimap<string,string> args;
   string key = "";
   string value = "";

   for (int a=1; a < argc; a++) {
      if (argv[a][0] == '-') {
         if ((key != "") && (args.count( key ) == 0)) {
            args.insert( pair<string,string>( key, "" ) );
         }
         key = string( argv[a] );
         if (key == "-bg")   key = "-background";
         if (key == "-bd")   key = "-bordercolor";
         if (key == "-bw")   key = "-borderwidth";
         if (key == "-fg")   key = "-foreground";
         if (key == "-fn")   key = "-font";
         if (key == "-h")    key = "-help";
         if (key == "-geom") key = "-geometry";
         if (key == "-name") key = "-title";
      } else {
         value = string( argv[a] );
         args.insert( pair<string,string>( key, value ));
      }
   }

   return args;
}

string getArg( const multimap<string,string> &arg, string key, int index, 
      string defaultValue ) {
   if (arg.count( key ) <= index) return defaultValue;
   return arg.find(key)->second;
/*
typedef std::multimap<T1, T2>::iterator iter;
for (std::pair<iter, iter> range(myMap.equal_range(myKey));
     range.first != range.second;
     ++range.first)
{
    //In each iteration range.first will refer to a different object
    //In each case, range.first->first will be equivalent to myKey
    //and range.first->second will be a value that range.first->first maps to.
}
*/
}

void loop( XXDisplay *display, XXWindow *window, XXGC *gc, XXFont *font, 
      XXColor *fg1, XXColor *fg2 ) {
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
               cout << "Main window closed.\n";
               finished = true;
            }
            break;

         case Expose:
            cout << "Exposed.\n";
            scribble( gc, fg1, fg2 );
            break;

         case MapNotify:
            cout << "Mapped.\n";
            scribble( gc, fg1, fg2 );
            break;

         case ConfigureNotify:
            cout << "Reconfigured.\n";
            scribble( gc, fg1, fg2 );
            break;

         case ButtonPress:
            cout << "Pressed " << modState( event.xbutton.state ) << " ";
            cout << "button " << event.xbutton.button << ".\n";
            break;
	 // Button 4 = Scroll up
	 // Button 5 = Scroll down

         case ButtonRelease:
            cout << "Released " << modState( event.xbutton.state ) << " ";
            cout << "button " << event.xbutton.button << ".\n";
            break;

         case KeyPress:
            chars = XLookupString( &event.xkey, keyBuffer, BUFFER_SIZE, 
                  &keySym, &composeStatus );
            cout << "Pressed " << modState( event.xkey.state );
            if ((chars > 0) && (keySym >= ' ') && (keySym <= '~')) {
               if (event.xkey.state & ControlMask) {
                  char letter = (char) keySym;
                  cout << "\"^" << letter << "\" #" 
                       << hex << (int) keySym << ".\n";
               } else {
                  cout << "\"" << keyBuffer[0] << "\".\n";
               }
            } else {
               switch( keySym ) {
                  case XK_Return:
                     cout << "Return";
                     break;
                  case XK_BackSpace:
                     cout << "BackSpace";
                     break;
                  case XK_Escape:
                     cout << "Escape";
                     finished = true;
                     break;
                  case XK_Delete:
                     cout << "Delete";
                     break;
                  case XK_Up:
                     cout << "Up";
                     break;
                  case XK_Down:
                     cout << "Down";
                     break;
                  case XK_Right:
                     cout << "Right";
                     break;
                  case XK_Left:
                     cout << "Left";
                     break;
                  case XK_Home:
                     cout << "Home";
                     break;
                  case XK_Prior:
                     cout << "Page Up";
                     break;
                  case XK_Next:
                     cout << "Page down";
                     break;
                  case XK_Begin:
                     cout << "Begin";
                     break;
                  case XK_End:
                     cout << "End";
                     break;
                  case XK_Insert:
                     cout << "Insert";
                     break;
                  case XK_Help:
                     cout << "Help";
                     break;
                  case XK_Menu:
                     cout << "Menu";
                     break;
                  case XK_Print:
                     cout << "Print";
                     break;
                  case XK_Tab:
                     cout << "Tab";
                     break;
                  case XK_Break: // Ctrl-Pause/Break
                     cout << "Break";
                     break;
		  case XK_Sys_Req: // Unreachable?
		     cout << "SysReq";

                  case XK_Control_L:
                     cout << "Control L";
                     break;
                  case XK_Control_R:
                     cout << "Control R";
                     break;
                  case XK_Shift_L:
                     cout << "Shift L";
                     break;
                  case XK_Shift_R:
                     cout << "Shift R";
                     break;
                  case XK_Alt_L:
                     cout << "Alt L";
                     break;
                  case XK_Alt_R:
                     cout << "Alt R";
                     break;
                  case XK_Super_L: // Window key
                     cout << "Super L";
                     break;
                  case XK_Super_R: // Window key
                     cout << "Super R";
                     break;
                  case XK_Hyper_L:
                     cout << "Hyper L";
                     break;
                  case XK_Hyper_R:
                     cout << "Hyper R";
                     break;
                  case XK_Meta_L:
                     cout << "Meta L";
                     break;
                  case XK_Meta_R:
                     cout << "Meta R";
                     break;
                  case XK_Caps_Lock:
                     cout << "Caps Lock";
                     break;
                  case XK_Num_Lock:
                     cout << "Num Lock";
                     break;
                  case XK_Pause:
                     cout << "Pause";
                     break;
                  case XK_Scroll_Lock:
                     cout << "Scroll Lock";
                     break;

                  case XK_F1:
                     cout << "F1";
                     break;
                  case XK_F2:
                     cout << "F2";
                     break;
                  case XK_F3:
                     cout << "F3";
                     break;
                  case XK_F4:
                     cout << "F4";
                     break;
                  case XK_F5:
                     cout << "F5";
                     break;
                  case XK_F6:
                     cout << "F6";
                     break;
                  case XK_F7:
                     cout << "F7";
                     break;
                  case XK_F8:
                     cout << "F8";
                     break;
                  case XK_F9:
                     cout << "F9";
                     break;
                  case XK_F10:
                     cout << "F10";
                     break;
                  case XK_F11:
                     cout << "F11";
                     break;
                  case XK_F12:
                     cout << "F12";
                     break;
                  case XK_F13:
                     cout << "F13";
                     break;
                  case XK_F14:
                     cout << "F14";
                     break;
                  case XK_F15:
                     cout << "F15";
                     break;

                  case XK_KP_Enter:
                     cout << "Keypad Enter";
                     break;
                  case XK_KP_Delete:
                     cout << "Keypad Delete";
                     break;
                  case XK_KP_Up:
                     cout << "Keypad Up";
                     break;
                  case XK_KP_Down:
                     cout << "Keypad Down";
                     break;
                  case XK_KP_Right:
                     cout << "Keypad Right";
                     break;
                  case XK_KP_Left:
                     cout << "Keypad Left";
                     break;
                  case XK_KP_Home:
                     cout << "Keypad Home";
                     break;
                  case XK_KP_Prior:
                     cout << "Keypad Page Up";
                     break;
                  case XK_KP_Next:
                     cout << "Keypad Page down";
                     break;
                  case XK_KP_Begin:
                     cout << "Keypad Begin";
                     break;
                  case XK_KP_End:
                     cout << "Keypad End";
                     break;
                  case XK_KP_Insert:
                     cout << "Keypad Insert";
                     break;
                  case XK_KP_Equal:
                     cout << "Keypad Equal";
                     break;
                  case XK_KP_Multiply:
                     cout << "Keypad *";
                     break;
                  case XK_KP_Divide:
                     cout << "Keypad /";
                     break;
                  case XK_KP_Add:
                     cout << "Keypad +";
                     break;
                  case XK_KP_Subtract:
                     cout << "Keypad -";
                     break;
                  case XK_KP_Decimal:
                     cout << "Keypad .";
                     break;
                  case XK_KP_0:
                     cout << "Keypad 0";
                     break;
                  case XK_KP_1:
                     cout << "Keypad 1";
                     break;
                  case XK_KP_2:
                     cout << "Keypad 2";
                     break;
                  case XK_KP_3:
                     cout << "Keypad 3";
                     break;
                  case XK_KP_4:
                     cout << "Keypad 4";
                     break;
                  case XK_KP_5:
                     cout << "Keypad 5";
                     break;
                  case XK_KP_6:
                     cout << "Keypad 6";
                     break;
                  case XK_KP_7:
                     cout << "Keypad 7";
                     break;
                  case XK_KP_8:
                     cout << "Keypad 8";
                     break;
                  case XK_KP_9:
                     cout << "Keypad 9";
                     break;

                  default:
                     cout << "unrecognized key #" << hex << (int) keySym;
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
               cout << ".\n";
            }
            break;

            /*
         case KeyRelease:
            chars = XLookupString( &event.xkey, keyBuffer, BUFFER_SIZE, 
                  &keySym, &composeStatus );
            if (chars > 0) {
               cout << "Released \"" << keyBuffer[0] << ".\n";
            } else {
               cout << "Released a non-ASCII key.\n";
            }
            if (keyBuffer[0] == 'q') {
               finished = true;
            }
            break;
            */
      }
   }

}

void scribble( XXGC *gc, XXColor *color1, XXColor *color2 ) {
   gc->setForeground( color1 );
   gc->drawLine( 10, 10, 50, 50 );
   gc->drawRectangle( 10, 60, 40, 40 );
   gc->drawArc( 60, 10, 100, 100, 0.0, 360.0 );
   gc->setForeground( color2 );
   gc->fillArc( 60, 20, 90, 90, 0.0, 90.0 );
   gc->setForeground( color1 );
   gc->drawText( 10, 400, "Hello, World!" );

   gc->getWindow()->getDisplay()->flush();
}

string modState( unsigned int eventState ) {
   string out="[";
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

extern "C" int criticalErrorHandler( Display *display, XErrorEvent *error ) {
   char text[ BUFFER_SIZE+1 ];

   XGetErrorText( display, error->error_code, text, BUFFER_SIZE );

   fprintf( stderr, "X11 error:  %s\n", text );
   fprintf( stderr, "   Request Code: %d.%d   Error Code: %d\n", 
         error->request_code, error->minor_code, error->error_code );
   fprintf( stderr, "   Resource ID: %ld on display %s.\n", 
         error->resourceid, DisplayString( display ) );
   return 0;
}

extern "C" int fatalErrorHandler( Display *display ) {
   fprintf( stderr, "X Server failed for display %s\n", 
         DisplayString( display ) );
   exit( EXIT_FAILURE );
}

extern "C" void signalHandler( int theSignal ) {
   string signalName;

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

   cerr << "Caught " << signalName << " signal.\n";
   finished = true;
}
