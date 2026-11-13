// XX::Font.hh -- Font wrapper.
// Copyright (C) 2014,2015,2026 by Scott Harris.  

#ifndef XX_FONT_HH_
#define XX_FONT_HH_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "Display.hh"

namespace XX {

/**
 * @brief Font encapsulates an X11 XFontStruct.
 */

class Font {
   // Only Display can create or delete Fonts.
   friend class Display;

public:
   struct Spacing { // namespace
      static const char ANY = '*';
      static const char PROPORTIONAL = 'p';
      static const char MONOSPACED = 'm';
      static const char CHAR_CELL = 'c';
   };
   struct Slant { // namespace
      inline static const std::string ANY             = "*";
      inline static const std::string ITALIC          = "i";
      inline static const std::string OBLIQUE         = "o";
      inline static const std::string ROMAN           = "r";
      inline static const std::string REVERSE_OBLIQUE = "ro";
      inline static const std::string REVERSE_ITALIC  = "ri";
      inline static const std::string OTHER           = "ot";
      inline static const std::string POLYMORPHIC     = "0";
   };
   struct Weight { // namespace
      inline static const std::string ANY         = "*";
      inline static const std::string EXTRA_LIGHT = "extralight";
      inline static const std::string LIGHT       = "light";
      inline static const std::string THIN        = "thin";
      inline static const std::string MEDIUM      = "medium";
      inline static const std::string REGULAR     = "regular";
      inline static const std::string BOLD        = "bold";
      inline static const std::string DEMIBOLD    = "demibold";
      inline static const std::string SEMIBOLD    = "semibold";
      inline static const std::string EXTRA_BOLD  = "extrabold";
      inline static const std::string BLACK       = "black";
      inline static const std::string POLYMORPHIC = "0";
   };
   struct Width { // namespace
      inline static const std::string ANY             = "*";
      inline static const std::string EXTRA_CONDENSED = "extracondensed";
      inline static const std::string CONDENSED       = "condensed";
      inline static const std::string SEMICONDENSED   = "semicondensed";
      inline static const std::string NARROW          = "narrow";
      inline static const std::string NORMAL          = "normal";
      inline static const std::string DOUBLE_WIDE     = "double wide";
      inline static const std::string POLYMORPHIC     = "0";
   };

   //.. XFLD ...................................................................

   /**
    * A Font::Description represents an <b>X</b> <b>L</b>ogical <b>F</b>ont 
    * <b>D</b>escription (XLFD), which identifies fonts by their properties.  
    * It serves as both a name for a specific font and a search pattern for
    * font selection.
    * <p>
    * The <code>ANY</code> constant for a field value acts a wildcard for 
    * searching.  The <code>POLYMORPHIC</code> constants are generally only
    * used on polymorphic font desccriptions, where they don't need to be 
    * set or cleared manually.
    */
   class Description { // XLFD = X Logical Font Description
   private:
      bool polymorphic_{false};
      bool scalable_{false};
      std::string foundry_{"*"};
      std::string family_{"*"};
      std::string weight_{Weight::ANY};
      std::string slant_{Slant::ANY};
      std::string width_{Width::ANY};
      std::string style_{""};
      bool hasPixelSize{false};
      unsigned int pixelSize_{0};
      bool hasPointSize{false};
      float pointSize_{0.0}; // 72.27ppi
      bool hasResolution{false};
      unsigned int resolutionX_{0};
      unsigned int resolutionY_{0};
      char spacing_{Spacing::ANY};
      bool hasAverageWidth{false};
      float averageWidth_{0.0};
      std::string charsetRegistry_{"*"};
      std::string charsetEncoding_{"*"};

   public:
      Description();
      Description( std::string );
      virtual ~Description();

      std::string string() const;

      inline bool isPolymorphic() const { return polymorphic_; };
      inline bool isScalable() const { return scalable_; };
      inline std::string foundry() const { return foundry_; };
      inline std::string family() const { return family_; };
      inline std::string weight() const { return weight_; };
      inline std::string slant() const { return slant_; };
      inline std::string width() const { return width_; };
      inline std::string styling() const { return style_; };
      inline unsigned int pixelSize() const { return pixelSize_; };
      inline float pointSize() const { return pointSize_; };
      inline unsigned int resolutionX() const { return resolutionX_; };
      inline unsigned int resolutionY() const { return resolutionY_; };
      inline char spacing() const { return spacing_; };
      inline float averageWidth() const { return averageWidth_; };
      inline std::string charsetRegistry() const { return charsetRegistry_; };
      inline std::string charsetEncoding() const { return charsetEncoding_; };

      Description& setPolymorphic();
      Description& notPolymorphic();
      Description& setScalable();
      Description& notScalable();
      Description& setFoundry( std::string );
      Description& setFamily( std::string );
      Description& setWeight( std::string );
      Description& setSlant( std::string );
      Description& setWidth( std::string );
      Description& setStyling( std::string );
      Description& noPixelSize( );
      Description& setPixelSize( unsigned int );
      Description& noPointSize( );
      Description& setPointSize( float );
      Description& noResolution( );
      Description& setResolution( unsigned int, unsigned int );
      Description& setSpacing( char );
      Description& noAverageWidth( );
      Description& setAverageWidth( float );
      Description& setCharsetRegistry( std::string );
      Description& setCharsetEncoding( std::string );
   };

   //...........................................................................

private:
   XX::Display *display;
   XFontStruct *xfont; // https://tronche.com/gui/x/xlib/graphics/font-metrics/
   std::string name_;

   //== Constructors ===========================================================

   /// Deallocate and destroy this Font.
   virtual ~Font();

   /// Look up a font by name.
   Font( XX::Display *display, const std::string fontName );

protected:

public:
   //== Accessors ==============================================================

   /// Get the font name.
   inline std::string name( void ) const { return this->name_; }

   /// Get the X11 font ID.
   inline ::Font getXFont( void ) const { return xfont->fid; }

   /// Get the default character width.
   int width( ) const;

   /// Get the width of a string rendered with this font.
   int width( const std::string text ) const;

   /// Get the character height.
   int height( ) const;

   /// Get the height of a string rendered with this font.
   int height( const std::string text ) const;

   /// Get the height of the font above the origin.
   inline int ascent() const { return this->xfont->ascent; };

   /// Get the height of the font below the origin.
   inline int descent() const { return this->xfont->descent; };

   //== Operations =============================================================


}; // class
}; // namespace

#endif /* XX_FONT_HH_ */
