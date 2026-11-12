// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/bmp.h, the lines in use so far.
#ifndef BWPORT_GFL_BMP_H
#define BWPORT_GFL_BMP_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "nnsys/g2d.h"

// Bitmaps of 16 or 256 color tiles, as BG and OBJ characters are stored: a tile's rows follow each other, and the tiles
// of a row of tiles follow each other. The pixels are the bitmap's own, or wrap memory such as VRAM. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// A bitmap of tileWidth by tileHeight tiles, of tileSize bytes each, or one over existing pixels
GFLBitmap *GFL_BitmapCreate(u32 tileWidth, u32 tileHeight, u32 tileSize, HeapID heapId);
GFLBitmap *GFL_BitmapWrap(void *pixels, u32 tileWidth, u32 tileHeight, u32 tileSize, HeapID heapId);
void GFL_BitmapFree(GFLBitmap *bitmap);
u8 *GFL_BitmapGetPixelData(GFLBitmap *bitmap);
// Copies a rectangle between bitmaps of the same color depth, skipping colorKey, clipped to both
void GFL_BitmapCopyArea(GFLBitmap *src, GFLBitmap *dest, u32 srcX, u32 srcY, int destX, int destY, u32 width,
                        u32 height, u16 colorKey);
void GFL_BitmapFillArea(GFLBitmap *bitmap, s16 x, s16 y, u16 width, u16 height, u8 color);
void GFL_BitmapFill(GFLBitmap *bitmap, u8 color);

// What the pixels of a bitmap are
enum {
    GFL_BITMAP_OWNED, // allocated with the bitmap, and freed with it
    GFL_BITMAP_WRAPPED,
    GFL_BITMAP_WRAPPED_VRAM,
};

struct GFLBitmap {
    u8 *pixels;
    u16 width;
    u16 height;
    u16 tileSize;
    u16 storage;
};

// The color a copy takes as opaque
#define GFL_BITMAP_NO_COLOR_KEY 0xffff

GFLBitmap *GFL_BitmapWrapVRAM(void *pixels, u32 tileWidth, u32 tileHeight, u32 tileSize, HeapID heapId);
u16 GFL_BitmapGetWidth(GFLBitmap *bitmap);
u16 GFL_BitmapGetHeight(GFLBitmap *bitmap);
u32 GFL_BitmapCalcPixelDataSize(const GFLBitmap *bitmap);
// Copies as many pixels as both hold
void GFL_BitmapCopy(const GFLBitmap *src, GFLBitmap *dest);
// A bitmap of a character file of an archive
GFLBitmap *GFL_G2DIOLoadBitmap(u32 arcId, u32 fileId, BOOL compressed, HeapID heapId);
u8 GFL_BitmapGetBytesPerTile(GFLBitmap *bitmap);
// The same from a 16 color bitmap to a 256 color one, adding paletteOffset to each color but 0
void GFL_BitmapCopyAreaRebased(const GFLBitmap *src, GFLBitmap *dest, u32 srcX, u32 srcY, int destX, int destY,
                               u32 width, u32 height, u16 colorKey, u16 paletteOffset);
// Rearranges the pixels from tiles into rows, in a new bitmap, or in place unless keepAsNew
GFLBitmap *GFL_BitmapMakeLinear(GFLBitmap *bitmap, BOOL keepAsNew, HeapID heapId);

void GFL_BitmapConvFromCHAR(GFLBitmap *bitmap, NNSG2dCharacterData *chars, HeapID heapId);

#endif // BWPORT_GFL_BMP_H
