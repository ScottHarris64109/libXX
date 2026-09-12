/*#############################################################################
 * DemoEventHandler.cc -- EventHandlers for the libXX demo.
 * Copyright (C) 2026 by Scott Harris.  All rights reserved.
 *#############################################################################
 */
#include <iostream>
#include <string.h>
#include <errno.h>

#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xproto.h>

#include "Palette.h"
#include "DemoEventHandler.hh"

void DemoEventHandler::setPalette( Palette *palette ) {
   this->palette = palette;
}

std::string DemoEventHandler::modState( unsigned int eventState ) {
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
   if (eventState & Button4Mask) { // Scroll Wheel Up
      if (started) out += "+";
      out += "#4";
      started = true;
   }
   if (eventState & Button5Mask) { // Scroll Wheel Down
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

void MainWindowButtonPress::setPopup( XX::Window *popup ) {
   this->popup = popup;
}

bool MainWindowButtonPress::operator()( XX::Window *window, XEvent& event ) {
   switch( event.xbutton.button ) {
      case 1:
         if (this->isDrawing) {
            this->isDrawing = false;
         } else {
            this->isDrawing = true;
         }
         break;

      case 3:
         if (!this->popup->isOpen()) {
            this->popup->moveTo( event.xbutton.x, event.xbutton.y );
            this->popup->open();
         }
         break;

      // Button 4 = Scroll up
      // Button 5 = Scroll down
      default:
         std::cout << "Window[" << window->getXID() << "]: ";
         std::cout << "Pressed " << modState( event.xbutton.state ) << " ";
         std::cout << "button " << event.xbutton.button << ".\n";
         break;
   }
   return true;
}

bool MainWindowButtonRelease::operator()( XX::Window *window, XEvent& event ) {
   std::cout << "Window[" << window->getXID() << "]: ";
   std::cout << "Released " << modState( event.xbutton.state ) << " ";
   std::cout << "button " << event.xbutton.button << ".\n";
   return true;
}

#define BUFFER_SIZE 255
bool MainWindowKeyPress::operator()( XX::Window *window, XEvent& event ) {
   XComposeStatus composeStatus;
   KeySym         keySym;
   char           keyBuffer[ BUFFER_SIZE+1 ];
   int            chars = 0;

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
            window->close();
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
            break;

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
            std::cout << "unrecognized key #" << std::hex << (int) keySym;
            break;
      }
      std::cout << ".\n";
   }

   return true;
}

bool MainWindowDraw::operator()( XX::Window *window, XEvent& event ) {
   switch( event.type ) {

      case Expose:
         std::cout << "Exposed.\n";
         break;

      case MapNotify:
         std::cout << "Mapped.\n";
         break;

      case ConfigureNotify:
         std::cout << "Reconfigured.\n";
         break;

   }
   window->drawLine( palette->color[0], 10, 10, 50, 50 );
   window->drawRectangle( palette->color[0], 10, 60, 40, 40 );
   window->drawArc( palette->color[0], 60, 10, 100, 100, 0.0, 360.0 );
   window->fillArc( palette->color[1], 60, 20, 90, 90, 0.0, 90.0 );
   window->drawText( palette->color[0], palette->font, 10, 400, 
         "Hello, World!" );

   window->display()->flush();
   return true;
}

//-- Popup Window -------------------------------------------------------------

bool PopupDraw::operator()( XX::Window *window, XEvent& event ) {
   window->drawText( palette->color[0], palette->font, 10, 20, 
         "Popup! Click anywhere to close." );
   window->display()->flush();
   return true;
}

bool PopupClose::operator()( XX::Window *window, XEvent& event ) {
   window->close();
   return true;
}

