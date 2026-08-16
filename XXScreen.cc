/*#############################################################################
 * XXScreen.cc -- Screen class code.
 * Copyright (C) 2026 by Scott Harris.  
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library.  If not, see <https://www.gnu.org/licenses/>.
 *#############################################################################
 */
#include <cstring>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "XXDisplay.hh"
#include "XXScreen.hh"
#include "XXWindow.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a Screen object.
 */

XX::Screen::~Screen( ) {
   if (this->root_) delete this->root_;
}

/**
 *  Create a Screen object.
 */

XX::Screen::Screen( XX::Display *display, int index ) :
      display_{ display }, index_{ index } {
   this->width_ = DisplayWidth( this->display()->xDisplay(), this->index() );
   this->height_ = DisplayHeight( this->display()->xDisplay(), this->index() );
   this->colorDepth_ = DefaultDepth( this->display()->xDisplay(), this->index() );
   this->root_ = nullptr;
   this->root_ = new XX::Window( this );
}

/**
 *  Look up a color by name.
 *
 * @return the Color or nullptr
 */

XX::Color *XX::Screen::getColor( const char *name ) {
   XColor definition, hardwareColor;
   int found = 0;
   char softName[ 255 ];
   XX::Color *color = nullptr;

   if (strlen(name) < 255)
   {
      strcpy( softName, name );
      found = XLookupColor( this->display()->xDisplay(), 
            DefaultColormap( this->display()->xDisplay(), this->index() ),
            softName, &definition, &hardwareColor );
   }

   if (found == 0) {
      //throw std::runtime_error( "No such color." );
   } else {
      color = new XX::Color( definition.red, definition.green, 
            definition.blue );
   }

   return color;
}

/**
 *  Look up a color by name.
 *
 * @return the Color or nullptr
 */

XX::Color *XX::Screen::getColor( const std::string name ) {
   return this->getColor( name.c_str() );
}
