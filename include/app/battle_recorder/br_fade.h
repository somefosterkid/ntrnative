// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_fade.h, the lines in
// use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_FADE_H
#define BWPORT_APP_BATTLE_RECORDER_BR_FADE_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// The Battle Recorder's fades between its screens (br_fade.c): by the master brightness, by alpha blending, or to its
// color with the palettes. pokebw2's guessed names are in the comments

// The screens a fade applies to
#define BR_FADE_DISPLAY_MAIN 0x1
#define BR_FADE_DISPLAY_SUB 0x2
#define BR_FADE_DISPLAY_BOTH (BR_FADE_DISPLAY_MAIN | BR_FADE_DISPLAY_SUB)

// How a fade is done
enum {
    // The master brightness, to black or to white
    BR_FADE_TYPE_MASTER_BRIGHT_BLACK,
    BR_FADE_TYPE_MASTER_BRIGHT_WHITE,
    BR_FADE_TYPE_ALPHA_BG012,
    // The palettes, to the color. A display of 0xf fades sub BG palettes 12 and 13 to white instead
    BR_FADE_TYPE_PLTT,
    // To white on the main screen and by alpha on the sub screen
    BR_FADE_TYPE_MASTER_BRIGHT_AND_ALPHA,
};

enum {
    BR_FADE_DIR_IN,
    BR_FADE_DIR_OUT,
};

// pokebw2's BrFade_StartFade
void func_ov186_021fe784(BrFade *p_wk, u32 type, u32 display, u32 dir);
// pokebw2's BrFade_IsEnd
BOOL func_ov186_021fe7b0(const BrFade *cp_wk);

// pokebw2's BrFade_SetAlpha
// Sets the alpha of the screens' BGs, hiding them at 0
void func_ov186_021fe840(BrFade *p_wk, u32 display, u32 ev);
// pokebw2's BrFade_SetColor
void func_ov186_021fe7f4(BrFade *p_wk, u16 color);

// pokebw2's BrFade_StartFadeEx
// sync is the master brightness's frames per step, or the steps of the alpha and palette fades, 16 / the fade
// update frequency if 0
void func_ov186_021fe794(BrFade *p_wk, u32 type, u32 display, u32 dir, u32 sync);

// pokebw2's BrFade_Init
BrFade *func_ov186_021fe6c0(HeapID heapId);
// pokebw2's BrFade_Exit
void func_ov186_021fe734(BrFade *p_wk);
// pokebw2's BrFade_Main
void func_ov186_021fe768(BrFade *p_wk);
// pokebw2's BrFade_LoadPltt
// Copies the palettes in VRAM to the fade's buffers
void func_ov186_021fe7b4(BrFade *p_wk);
// pokebw2's BrFade_FillColor
// Sets the screens' palettes to the color
void func_ov186_021fe7f8(BrFade *p_wk, u32 display);
// pokebw2's BrFade_LoadPlttArc
// Loads a palette into one of the fade's PaletteFade buffers
void func_ov186_021fe8e8(BrFade *p_wk, ArcTool *handle, u32 fileId, u32 buffer, u32 offset, u32 size, HeapID heapId);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_FADE_H
