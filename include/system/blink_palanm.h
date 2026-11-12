// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/blink_palanm.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_BLINK_PALANM_H
#define BWPORT_SYSTEM_BLINK_PALANM_H

#include "types.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BlinkPalAnm_InitAnime
void func_0202eb04(BlinkPalAnm *anm);
// pokebw2's BlinkPalAnm_SetPalBufferArcTool
void func_0202e9ac(BlinkPalAnm *anm, ArcTool *arc, u32 fileId, u32 startPos, u32 endPos);
// pokebw2's BlinkPalAnm_Main
// Steps the animation and queues the colors for the next V-blank
void func_0202ea40(BlinkPalAnm *anm);
// pokebw2's BlinkPalAnm_Create
// Animates count colors from offset in the palette
BlinkPalAnm *func_0202e940(u16 offset, u16 count, u16 palette, HeapID heapId);
// pokebw2's BlinkPalAnm_Free
void func_0202ea20(BlinkPalAnm *anm);

// Black's layout as the 4 functions that reach it have it (tools/struct_infer.py), its size its allocation
struct BlinkPalAnm {
    // The colors at the start and at the end of the blink
    u16 *unk_00;
    u16 *unk_04;
    // The colors queued for the next V-blank
    u16 *unk_08;
    u16 unk_0C;
    u16 unk_0E;
    u16 unk_10;
    u16 unk_12;
    u32 unk_14;
    u16 unk_18;
    u8 unk_1A[0x2];
};

#endif // BWPORT_SYSTEM_BLINK_PALANM_H
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/blink_palanm.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_BLINK_PALANM_H

#include "types.h"
#include "gfl/heap.h"

// Black 2's, not checked against Black
// The palette to animate, a BG frame from 0 to 7 or the main or sub engine's OBJ palette
#define BLINK_PALANM_OBJ_MAIN 0xfffe

#endif // BWPORT_SYSTEM_BLINK_PALANM_H
