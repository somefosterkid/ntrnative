// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/palanm.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_PALANM_H
#define BWPORT_SYSTEM_PALANM_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Palette fades (palanm.c): copies of the palettes of each kind of palette memory, unfaded and faded, which a task
// fades toward a color step by step

// The buffers, one per kind of palette memory
enum {
    PALFADE_BUFFER_MAIN_BG,
    PALFADE_BUFFER_SUB_BG,
    PALFADE_BUFFER_MAIN_OBJ,
    PALFADE_BUFFER_SUB_OBJ,
    PALFADE_BUFFER_MAIN_BG_EX0,
    PALFADE_BUFFER_MAIN_BG_EX1,
    PALFADE_BUFFER_MAIN_BG_EX2,
    PALFADE_BUFFER_MAIN_BG_EX3,
    PALFADE_BUFFER_SUB_BG_EX0,
    PALFADE_BUFFER_SUB_BG_EX1,
    PALFADE_BUFFER_SUB_BG_EX2,
    PALFADE_BUFFER_SUB_BG_EX3,
    PALFADE_BUFFER_MAIN_OBJ_EX,
    PALFADE_BUFFER_SUB_OBJ_EX,
    PALFADE_BUFFER_COUNT,
};

// Copies size bytes of colors to both copies of a buffer, from the color at offset (pokebw2's PaletteFade_LoadData)
void func_020215bc(PaletteFade *fade, const void *src, u32 buffer, u16 offset, u16 size);
// Loads a palette file's colors from srcOffset to a buffer at offset, size bytes of them or all for 0 (pokebw2's
// PaletteFade_LoadNCLREx)
void func_020215ec(PaletteFade *fade, u32 arcId, u32 fileId, HeapID heapId, u32 buffer, u32 size, u16 offset,
                   u16 srcOffset);

// Turns colors to gray and maps the grays through a table
void ColorFilter_ApplyLUT(u16 *colors, int count, const u8 *lut);

// The palette memory PaletteFade_LoadFromVRAM copies
enum {
    PALFADE_VRAM_MAIN_BG,
    PALFADE_VRAM_SUB_BG,
    PALFADE_VRAM_MAIN_OBJ,
    PALFADE_VRAM_SUB_OBJ,
};

// pokebw2's PaletteFade_LoadFromVRAM
// Copies a standard palette memory's colors (PALFADE_VRAM_*) to the buffer of the same number
void func_020216d4(PaletteFade *fade, u32 vram, u16 offset, u32 size);

// pokebw2's BlendColors
// Blends count colors toward color by fraction of 16
void func_02021f64(const u16 *src, u16 *dst, u16 count, u8 fraction, u16 color);

// pokebw2's PaletteFade_Transfer
// Copies the faded buffers to VRAM, those that changed or all of them
void func_02021d50(PaletteFade *fade);
// pokebw2's PaletteFade_BlendPalettes
// PaletteFade_BlendBuffer for the 16-color palettes of paletteMask
void func_02022004(PaletteFade *fade, u32 buffer, u16 paletteMask, u8 fraction, u16 color);
// pokebw2's PaletteFade_SetTransferAll
// Makes PaletteFade_Transfer copy every buffer
void func_02021ee4(PaletteFade *fade, u32 transferAll);
// pokebw2's PaletteFade_FreeBuffer
void func_020215a0(PaletteFade *fade, u32 buffer);
// pokebw2's PaletteFade_LoadNCLR
void func_02021640(PaletteFade *fade, u32 arcId, u32 fileId, HeapID heapId, u32 buffer, u32 size, u16 offset);

// pokebw2's PaletteFade_GetActiveMask
// The buffers still fading
u16 func_02021ed8(PaletteFade *fade);
// pokebw2's PaletteFade_StartFade
// Starts fading the buffers in bufferMask that aren't fading, the palettes in paletteMask of each, from step start to
// step end of 16 toward color, applying the first step at once. A delay of 0 or more moves 2 steps every delay + 1
// frames, a negative one -delay + 2 steps (at most 16) every frame. Returns whether a buffer started
u8 func_0202173c(PaletteFade *fade, u16 bufferMask, u16 paletteMask, s8 delay, u8 start, u8 end, u16 color,
                         TCBManager *tcbManager);

// pokebw2's PaletteFade_BlendBuffer
// BlendColors from a buffer's unfaded colors into its faded ones
void func_02021fd8(PaletteFade *fade, u32 buffer, u16 offset, u16 count, u8 fraction, u16 color);

// pokebw2's PaletteFade_GetActiveMask
// The buffers still fading
// pokebw2's PaletteFade_StartFade
// Starts fading the buffers in bufferMask that aren't fading, the palettes in paletteMask of each, from step start to
// step end of 16 toward color, applying the first step at once. A delay of 0 or more moves 2 steps every delay + 1
// frames, a negative one -delay + 2 steps (at most 16) every frame. Returns whether a buffer started

// pokebw2's PaletteFade_GetActiveMask
// The buffers still fading

// pokebw2's PaletteFade_BlendBuffer
// BlendColors from a buffer's unfaded colors into its faded ones

// pokebw2's PaletteFade_RequestStop
// Stops the fade task at its next step
void func_02021b18(PaletteFade *fade);

// pokebw2's PaletteFade_GetFadedBuffer
u16 *func_02021730(PaletteFade *fade, u32 buffer);

// pokebw2's PaletteFade_SetAllActive
void func_02021f08(PaletteFade *fade, u32 active);

// pokebw2's PaletteFade_GetUnfadedBuffer
u16 *func_02021728(PaletteFade *fade, u16 buffer);

// pokebw2's PaletteFade_RestartFade
// PaletteFade_StartFade, restarting the buffers that are fading too
u8 func_02021848(PaletteFade *fade, u16 bufferMask, u16 paletteMask, s8 delay, u8 start, u8 end, u16 color,
                           TCBManager *tcbManager);

// The buffers of standard palettes (16 palettes of 16 colors) and of extended ones (16 of 256)
#define PALFADE_BUFFERS_STD 4

// PaletteFade_GetColor's choice of buffer
#define PALFADE_FADED 0

#define PALFADE_UNFADED 1

// pokebw2's PaletteFade_GetColor
u16 func_02021f3c(PaletteFade *fade, u32 buffer, u32 which, u16 pos);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PaletteFade_Create
PaletteFade *func_02021518(u32 heapId);
// pokebw2's PaletteFade_AllocBuffer
// Allocates a buffer's two copies of size bytes
void func_0202155c(PaletteFade *fade, u32 buffer, u32 size, HeapID heapId);
// pokebw2's PaletteFade_Free
void func_02021540(PaletteFade *fade);
// pokebw2's PaletteFade_LoadArcNCLR
void func_020216b4(PaletteFade *fade, ArcTool *arc, u32 fileId, HeapID heapId, u32 buffer, u32 size,
                             u16 offset);
// pokebw2's PaletteFade_LoadArcNCLREx
void func_02021660(PaletteFade *fade, ArcTool *arc, u32 fileId, HeapID heapId, u32 buffer, u32 size,
                               u16 offset, u16 srcOffset);

#endif // BWPORT_SYSTEM_PALANM_H
