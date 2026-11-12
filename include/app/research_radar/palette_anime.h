// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/palette_anime.h: the
// lines in use so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_PALETTE_ANIME_H
#define BWPORT_APP_RESEARCH_RADAR_PALETTE_ANIME_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PaletteAnime_Create
PaletteAnime *func_ov217_021c29c8(HeapID heapId);
// pokebw2's PaletteAnime_Delete
void func_ov217_021c29d8(PaletteAnime *anime);
// pokebw2's PaletteAnime_Setup
// Animates count colors at dst, from the colors at src, which it copies
void func_ov217_021c29e8(PaletteAnime *anime, u16 *dst, const u16 *src, u8 count);
// pokebw2's PaletteAnime_Restore
// Puts back the colors that were copied
void func_ov217_021c2a08(PaletteAnime *anime);
// pokebw2's PaletteAnime_Start
// Starts an animation of the colors toward color; the modes pulse, flash or fade in different ways
void func_ov217_021c29f8(PaletteAnime *anime, u32 mode, u16 color);
// pokebw2's PaletteAnime_Stop
void func_ov217_021c2a00(PaletteAnime *anime);
// pokebw2's PaletteAnime_Update
void func_ov217_021c29f0(PaletteAnime *anime);
// pokebw2's PaletteAnime_IsActive
BOOL func_ov217_021c2a10(PaletteAnime *anime);

#endif // BWPORT_APP_RESEARCH_RADAR_PALETTE_ANIME_H
