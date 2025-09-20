/* vim: set ai et ts=4 sw=4: */
#ifndef __ILI9341_FONTS_H__
#define __ILI9341_FONTS_H__

#include "stdint.h"

typedef struct {
    const uint32_t width;
    const uint32_t height;
    const uint32_t intsPerGlyph;
    const uint32_t* data;
} ILI9341_FontDef;

extern ILI9341_FontDef ILI9341_Font_Spleen5x8;
extern ILI9341_FontDef ILI9341_Font_Spleen6x12;
extern ILI9341_FontDef ILI9341_Font_Spleen8x16;
extern ILI9341_FontDef ILI9341_Font_Spleen12x24;
extern ILI9341_FontDef ILI9341_Font_Spleen16x32;
extern ILI9341_FontDef ILI9341_Font_Spleen32x64;

#endif  // __ILI9341_FONTS_H__
