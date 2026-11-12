// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_graphic.h, the lines
// in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_GRAPHIC_H
#define BWPORT_APP_BATTLE_RECORDER_BR_GRAPHIC_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The Battle Recorder's graphics system (br_graphic.c): the BG system, the cell actors and the VBlank task. pokebw2's
// guessed names are in the comments

// pokebw2's BrGraphic_GetClunit
ClActUnit *func_ov186_021fc4dc(const BrGraphic *p_wk);
// pokebw2's BrGraphic_StartMain3D and BrGraphic_EndMain3D
void func_ov186_021fc610(BrGraphic *p_wk, HeapID heapId);
void func_ov186_021fc640(BrGraphic *p_wk, HeapID heapId);

// The VRAM setup: the Battle Recorder's, or the musical photos', which draw in 3D
enum {
    BR_GRAPHIC_TYPE_NORMAL,
    BR_GRAPHIC_TYPE_MUSICAL,
};

// pokebw2's BrGraphic_Init
BrGraphic *func_ov186_021fc3bc(u32 type, u32 displayLayout, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrGraphic_Exit
void func_ov186_021fc474(BrGraphic *p_wk);
// pokebw2's BrGraphic_Main
void func_ov186_021fc4c8(BrGraphic *p_wk);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_GRAPHIC_H
