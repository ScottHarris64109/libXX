// Drawing.hh -- Drawing for libXX demo.
// Copyright (C) 2026 by Scott Harris.  All rights reserved.
/**
 * @brief Drawing is a drawing/canvas for the libXX demo.
 */

#ifndef DRAWING_HH_
#define DRAWING_HH_

#include <iostream>

#include <XX/Window.hh>

#include "Palette.h"
#include "Point.hh"
#include "Line.hh"

class Drawing {

private:
   XX::Window *window{ nullptr };
   bool lineStarted{ false };
   bool traced{ false };
   Palette *palette_{ nullptr };
   Line traceLine;
   int color{0};

   void untraceLine();
protected:

public:
   //== Constructors ===========================================================

   virtual ~Drawing();
   Drawing();

   //== Accessors ==============================================================

   void setWindow( XX::Window *window );
   void setPalette( Palette *palette );

   inline bool isDrawingLines() const { return lineStarted; };
   inline Palette *palette() { return this->palette_; };

   //== Operations =============================================================

   void startLine( int x, int y );
   void traceLineTo( int x, int y );
   void drawLineTo( int x, int y );
   void stopDrawingLines();
};

#endif /* DRAWING_HH_ */
