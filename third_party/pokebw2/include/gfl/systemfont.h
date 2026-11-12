#ifndef POKEBW2_GFL_SYSTEMFONT_H
#define POKEBW2_GFL_SYSTEMFONT_H

#include "types.h"

// The font for debug text (systemfont.c)

// Loads the font from a file, or uses the built-in one when path is NULL
void GFL_SystemFontInit(const char *path);
// Draws a character as a 16 color tile, followed by its width
void GFL_SystemFontGetGlyph(int c, u8 *dest);
u8 GFL_SystemFontGetHeight(void);
u16 GFL_SystemFontGetTabSize(void);
void GFL_SystemFontSetColorIndices(u32 fg, u32 bg);

#endif // POKEBW2_GFL_SYSTEMFONT_H
