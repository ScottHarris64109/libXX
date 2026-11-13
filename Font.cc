/*#############################################################################
 * XX::Font.cc -- X11 Font wrapper.
 *
 *  Copyright (C) 2012,2016,2026 by Scott Harris.  
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
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "Display.hh"
#include "Font.hh"

#define XLFD_FIELD_SEPARATOR '-'
#define XLFD_WILDCARD "*"

//== Constructors ==============================================================

/**
 *  Destroy and deallocate a Font object.
 */

XX::Font::~Font( ) {
   XFreeFont( this->display->xDisplay(), this->xfont );
// void XFreeFontSet(Display *display, XFontSet font_set); 
}

/**
 *  Get the named Font from the Display.
 */

XX::Font::Font( XX::Display *display, const std::string name ) : 
      display{ display }, name_{ name } {
   xfont = XLoadQueryFont( display->xDisplay(), this->name_.c_str() );
/*
   char **missing_charset;
   int missing_charsets;
   char *default_string; // What will be used for missing characters. Owned by Xlib.

   // name = -foundry-family-weight-slant-setwidth-addstyle-pixelsize-pointsize-xresolution-yresolution-spacing-avgwidth-charsetregistry-charsetencoding
   // -adobe-helvetica-medium-o-normal--10-100-75-75-p-57-iso8859-1
   // foundry = adobe
   // family = helvetica
   // weight = medium (medium|bold| black|light|extralight)
   // slant = o (r=Roman|i=Italic|o=Oblique|ri=ReverseItalic|ro=ReverseOblique|*)
   // setwidth = normal (normal|condensed|narrow|double wide)
   // addstyle = (serif|sans serif|informal|decorated)?
   // pixelsize = 10 // 0=Scalable
   // pointsize = 100 // decipoints = points *10.  72.27 ppi. 0=Scalable
   // xresolution = 75
   // yresolution = 75
   // spacing = p (p=proportional|m=monospaced|c=char cell)
   // avgwidth = 57 // tenths of pixels.  Leading ~ indicates right-to-left.
   // charsetregistry = iso8859
   // charsetencoding = 1

   XFontSet fontset = XCreateFontSet( display->xDisplay, 
         "*-18-*", 
         &missing_charset,
  	      &missing_charsets. 
         &default_string);
   if (missing_charsets > 0) {
  	   fprintf(stderr, "&s: missing charsets.\n", program_name);
 	   XFreeStringList(missing_charset);  
   }  

   char **fontName;
   int fonts;
   XFontStruct **fontStruct

   fonts = XFontsOfFontSet( fontset, &fontStruct, &fontName );
   // fontStruct and fontName lists belong to the fontset and will be freed by it.
 */
}

//== Accessors =================================================================

int XX::Font::width( ) const {
   return this->xfont->max_bounds.rbearing - this->xfont->max_bounds.lbearing;
}

int XX::Font::width( const std::string text ) const {
   return XTextWidth( this->xfont, text.c_str(), text.length() );
}

int XX::Font::height( ) const {
   return this->xfont->ascent + this->xfont->descent;
}

int XX::Font::height( const std::string text ) const {
   int fullHeight = this->height();
   // TODO:  Count lines.
   return fullHeight;
}

//## XLFD ######################################################################

XX::Font::Description::~Description() {
}

/**
 *  Create a Description that will match almost any font.  It defaults to 
 *  non-scalable non-polymorphic with wildcards for all fields.
 */
XX::Font::Description::Description() {
}

/**
 * Generate a Font::Description from an XLFD font name or pattern string.
 */
XX::Font::Description::Description( std::string fontspec ) {
   std::stringstream xfld( fontspec );
   std::string field;
   int x, y;

   if (fontspec[0] != XLFD_FIELD_SEPARATOR) {
      throw std::invalid_argument( "Not a valid font description." );
   }

   std::getline( xfld, field, XLFD_FIELD_SEPARATOR ); // Skip leading hyphen.
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR );
   if (!field.empty()) {
      try {
         this->setFoundry( field );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         this->setFamily( field );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         this->setWeight( field );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         this->setSlant( field );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         this->setWidth( field );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         this->setStyling( field );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         this->setPixelSize( stoi(field) );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         this->setPointSize( stof(field)/10.0 );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD && !xfld.eof()) {
      try {
         x = stoi(field);
         std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
         if (!field.empty() && field != XLFD_WILDCARD) {
            y = stoi(field);
            this->setResolution( x, y );
         }
      } catch (...) { // Skip these fields.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         this->setSpacing( field[0] );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         if (field[0] == '~') {
            field[0] = '-';
         }
         this->setAverageWidth( stof(field)/10.0 );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         this->setCharsetRegistry( field );
      } catch (...) { // Skip this field.
      }
   }

   if (xfld.eof()) goto XLFD_EOF;
   std::getline( xfld, field, XLFD_FIELD_SEPARATOR  );
   if (!field.empty() && field != XLFD_WILDCARD) {
      try {
         this->setCharsetEncoding( field );
      } catch (...) { // Skip this field.
      }
   }

XLFD_EOF: ;
}

/**
 * Get the XLFD string for this Font::Description.
 */
std::string XX::Font::Description::string() const {
   std::string name;
   std::string pixelSizeField = XLFD_WILDCARD;
   std::string pointSizeField = XLFD_WILDCARD;
   std::string resolutionFields = XLFD_WILDCARD "-" XLFD_WILDCARD;
   std::string averageWidthField = XLFD_WILDCARD;

   if (this->hasPixelSize) {
      pixelSizeField = std::to_string( this->pixelSize() );
   }

   if (this->hasPointSize) {
      int decipoints = this->pointSize() *10.0;
      pointSizeField = std::to_string( decipoints );
   }

   if (this->hasResolution) {
      resolutionFields = std::to_string( this->resolutionX() ) 
                         +XLFD_FIELD_SEPARATOR+
                         std::to_string( this->resolutionY() );
   }

   if (this->hasAverageWidth) {
      int decipixels = std::abs( this->averageWidth() *10.0 );
      std::string sign = (this->averageWidth() < 0.0) ? "~" : "";
      averageWidthField = sign + std::to_string( decipixels );
   }

   name = XLFD_FIELD_SEPARATOR + this->foundry() 
         +XLFD_FIELD_SEPARATOR + this->family() 
         +XLFD_FIELD_SEPARATOR + this->weight() 
         +XLFD_FIELD_SEPARATOR + this->slant() 
         +XLFD_FIELD_SEPARATOR + this->width()
         +XLFD_FIELD_SEPARATOR + this->styling()
         +XLFD_FIELD_SEPARATOR + pixelSizeField
         +XLFD_FIELD_SEPARATOR + pointSizeField
         +XLFD_FIELD_SEPARATOR + resolutionFields
         +XLFD_FIELD_SEPARATOR + this->spacing()
         +XLFD_FIELD_SEPARATOR + averageWidthField
         +XLFD_FIELD_SEPARATOR + this->charsetRegistry()
         +XLFD_FIELD_SEPARATOR + this->charsetEncoding()
   ;

   return name;
}

/**
 *  Make this a Description for a polymorphic Font.  The weight, width, 
 *  and slant criteria will be set to POLYMORPHIC.
 */
XX::Font::Description& XX::Font::Description::setPolymorphic() {
   this->setWeight( XX::Font::Weight::POLYMORPHIC );
   this->setWidth(  XX::Font::Width::POLYMORPHIC );
   this->setSlant(  XX::Font::Slant::POLYMORPHIC );

   this->polymorphic_ = true;

   return *this;
}

/**
 *  Make this a Description for a non-polymorphic Font.  The weight, width, 
 *  and slant criteria will be set to ANY.
 */
XX::Font::Description& XX::Font::Description::notPolymorphic() {
   this->polymorphic_ = false;

   this->setWeight( XX::Font::Weight::ANY );
   this->setWidth(  XX::Font::Width::ANY );
   this->setSlant(  XX::Font::Slant::ANY );

   return *this;
}

/**
 *  Make this a Description for a scalable Font.  The pixelSize, pointSize, 
 *  and averageWidth criteria will be zeroed out.
 */
XX::Font::Description& XX::Font::Description::setScalable() {
   this->scalable_ = true;

   this->setPixelSize(0);
   this->setPointSize(0.0);
   this->setAverageWidth(0.0);

   return *this;
}

/**
 *  Make this a Description for a non-scalable Font.  The pixelSize, pointSize, 
 *  resolution, and averageWidth criteria will be cleared.
 */
XX::Font::Description& XX::Font::Description::notScalable() {
   this->scalable_ = false;

   this->hasPixelSize = false;
   this->hasPointSize = false;
   this->hasResolution = false;
   this->hasAverageWidth = false;

   return *this;
}

/**
 * The name of the organization that created or provided the font.
 */
XX::Font::Description& XX::Font::Description::setFoundry( std::string foundry ) {
   this->foundry_ = foundry;
   return *this;
}

/**
 * The typeface of the font.  Examples include arial, courier, gothic, 
 * helvetica, lucida, roboto, and times.
 */
XX::Font::Description& XX::Font::Description::setFamily( std::string family ) {
   this->family_ = family;
   return *this;
}

/**
 * Typographic weight should be set using the XX::Font::Weight constants:
 * <ul>
 * <li><code>XX::Font::Weight::<b>ANY</b></code>
 * <li><code>XX::Font::Weight::<i>POLYMORPHIC</i></code>
 * <li><code>XX::Font::Weight::<b>BLACK</b></code>
 * <li><code>XX::Font::Weight::<b>BOLD</b></code>
 * <li><code>XX::Font::Weight::<b>DEMIBOLD</b></code>
 * <li><code>XX::Font::Weight::<b>EXTRA_BOLD</b></code>
 * <li><code>XX::Font::Weight::<b>EXTRA_LIGHT</b></code>
 * <li><code>XX::Font::Weight::<b>LIGHT</b></code>
 * <li><code>XX::Font::Weight::<b>MEDIUM</b></code>
 * <li><code>XX::Font::Weight::<b>REGULAR</b></code>
 * <li><code>XX::Font::Weight::<b>SEMIBOLD</b></code>
 * <li><code>XX::Font::Weight::<b>THIN</b></code>
 * </ul>
 */
XX::Font::Description& XX::Font::Description::setWeight( std::string weight ) {
   this->weight_ = weight;
   return *this;
}

/**
 * Slant should be set using the XX::Font::Slant constants:
 * <ul>
 * <li><code>XX::Font::Slant::<b>ANY</b></code>
 * <li><code>XX::Font::Slant::<i>POLYMORPHIC</i></code>
 * <li><code>XX::Font::Slant::<b>ITALIC</b></code>
 * <li><code>XX::Font::Slant::<b>OBLIQUE</b></code>
 * <li><code>XX::Font::Slant::<b>REVERSE_ITALIC</b></code>
 * <li><code>XX::Font::Slant::<b>REVERSE_OBLIQUE</b></code>
 * <li><code>XX::Font::Slant::<b>ROMAN</b></code>
 * <li><code>XX::Font::Slant::<b>OTHER</b></code>
 * </ul>
 */
XX::Font::Description& XX::Font::Description::setSlant( std::string slant ) {
   this->slant_ = slant;
   return *this;
}

/**
 * Typograqphic proportional width should be set using the XX::Font::Width 
 * constants:
 * <ul>
 * <li><code>XX::Font::Width::<b>ANY</b></code>
 * <li><code>XX::Font::Width::<i>POLYMORPHIC</i></code>
 * <li><code>XX::Font::Width::<b>CONDENSED</b></code>
 * <li><code>XX::Font::Width::<b>DOUBLE_WIDE</b></code>
 * <li><code>XX::Font::Width::<b>EXTRA_CONDENSED</b></code>
 * <li><code>XX::Font::Width::<b>NARROW</b></code>
 * <li><code>XX::Font::Width::<b>NORMAL</b></code>
 * <li><code>XX::Font::Width::<b>SEMICONDENSED</b></code>
 * </ul>
 */
XX::Font::Description& XX::Font::Description::setWidth( std::string width ) {
   this->width_ = width;
   return *this;
}

/**
 * Styling is additional typographic styling information needed to uniquely 
 * identify a font.  It is completely arbitrarily assigned by the 
 * foundry/provider and is not generally useful for searching unless you 
 * already know what it is.
 * <p>
 * Examples include things like "serif", "sans", "informal", and "decorated".
 */
XX::Font::Description& XX::Font::Description::setStyling( std::string style ) {
   this->style_ = style;
   return *this;
}

/// Clear the pixelSize criterion.
XX::Font::Description& XX::Font::Description::noPixelSize( ) {
   this->hasPixelSize = false;
   return *this;
}

/**
 * Body size of the font given a particular point size and resolution.  It is
 * typically used query fonts according to device-dependent size, regardless 
 * of the point size or vertical resolution for which the font was designed.
 */
// TODO: matrix?
XX::Font::Description& XX::Font::Description::setPixelSize( unsigned int size ) {
   this->pixelSize_ = size;
   this->hasPixelSize = true;
   return *this;
}

/// Clear the pointSize criterion.
XX::Font::Description& XX::Font::Description::noPointSize( ) {
   this->hasPointSize = false;
   return *this;
}

/**
 * Body size in typographic points (72.27 points per inch).
 */
// TODO: matrix?
XX::Font::Description& XX::Font::Description::setPointSize( float size ) {
   this->pointSize_ = size;
   this->hasPointSize = true;
   return *this;
}

/// Clear the resolution criteria.
XX::Font::Description& XX::Font::Description::noResolution( ) {
   this->hasResolution = false;
   return *this;
}

/**
 * Font resolution in dpi.
 */
XX::Font::Description& XX::Font::Description::setResolution( 
      unsigned int x, unsigned int y) {
   this->resolutionX_ = x;
   this->resolutionY_ = y;
   this->hasResolution = true;
   return *this;
}

/**
 * Spacing should be set using the XX::Font::Spacing constants:
 * <ul>
 * <li><code>XX::Font::Spacing::<b>ANY</b></code>
 * <li><code>XX::Font::Spacing::<b>PROPORTIONAL</b></code>
 * <li><code>XX::Font::Spacing::<b>MONOSPACED</b></code>
 * <li><code>XX::Font::Spacing::<b>CHAR_CELL</b></code>
 * </ul>
 */
XX::Font::Description& XX::Font::Description::setSpacing( char spacing ) {
   this->spacing_ = spacing;
   return *this;
}

/// Clear the averageWidth criterion.
XX::Font::Description& XX::Font::Description::noAverageWidth( ) {
   this->hasAverageWidth = false;
   return *this;
}

/**
 * Unweighted arithmetic mean of the absolute value of the width of each glyph 
 * in the font in pixels.  For monospaced and character cell fonts, this is 
 * the width of all glyphs in the font. 
 * <p>
 * A negative average width indicates that the font is written right-to-left.
 */
XX::Font::Description& XX::Font::Description::setAverageWidth( float width ) {
   this->averageWidth_ = width;
   this->hasAverageWidth = true;
   return *this;
}

/**
 * The registration authority for the character encoding of the font.
 */
XX::Font::Description& XX::Font::Description::setCharsetRegistry( 
      std::string registry ) {
   this->charsetRegistry_ = registry;
   return *this;
}

/**
 * The specific character set within the registry.
 */
XX::Font::Description& XX::Font::Description::setCharsetEncoding( 
      std::string encoding ) {
   this->charsetEncoding_ = encoding;
   return *this;
}

