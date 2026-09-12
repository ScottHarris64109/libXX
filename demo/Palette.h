/*#############################################################################
 * Palette.h -- Palette struct for libXX demo.
 *
 *  Copyright (C) 2026 by Scott Harris.  All rights reserved.
 *#############################################################################
 */

#ifndef PALETTE_H_
#define PALETTE_H_

#include <XX/Color.hh>
#include <XX/Font.hh>

typedef struct {
   XX::Color  *color[2] = { nullptr, nullptr };
   XX::Color  *background = nullptr;
   XX::Font   *font = nullptr;
} Palette;

#endif /* PALETTE_H_ */
