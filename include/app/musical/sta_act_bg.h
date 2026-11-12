// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/sta_act_bg.h: the lines in use
// so far.
#ifndef BWPORT_APP_MUSICAL_STA_ACT_BG_H
#define BWPORT_APP_MUSICAL_STA_ACT_BG_H

#include "types.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of heapId, stage, scroll, mdlRes, anmRes, mdl, anms and actor, and its
// size in Black
struct StaActBg {
    HeapID heapId;
    // NULL in the photo
    StaActing *stage;
    // How far the stage has scrolled, in pixels
    u16 scroll;
    u8 unkA[0x12];
    // The model's resource, NULL until a background is loaded
    void *mdlRes;
    void *anmRes[3];
    G3DModel *mdl;
    void *anms[3];
    G3DActor *actor;
};

// pokebw2's StaActBg_InitSystem
StaActBg *func_ov112_021f6f24(HeapID heapId, StaActing *stage);
// pokebw2's StaActBg_TermSystem
void func_ov112_021f6f4c(StaActBg *bg);
// pokebw2's StaActBg_UpdateSystem
void func_ov112_021f6f64(StaActBg *bg);
// pokebw2's StaActBg_DrawSystem
void func_ov112_021f6f8c(StaActBg *bg);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's StaActBg_LoadBg
void func_ov112_021f6fb0(StaActBg *bg, u8 bgNo);
// pokebw2's StaActBg_SetScrollOffset
void func_ov112_021f7098(StaActBg *bg, u16 scroll);

#endif // BWPORT_APP_MUSICAL_STA_ACT_BG_H
