// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/sta_act_bg.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/sta_act_bg.h"
#include "constants/arc.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"

// Overlay 209's sta_act_bg.c: the stage's background, a 3D model with three animations, from ARCID_MUSICAL's
// files 30 (the models), 36, 41 and 46 (their animations) on

#define STA_ACT_BG_FILE_MODEL 30
#define STA_ACT_BG_FILE_ANIME1 36
#define STA_ACT_BG_FILE_ANIME2 46
#define STA_ACT_BG_FILE_ANIME3 41

static void func_ov112_021f7048(StaActBg *bg);

// pokebw2's sStaActBgMatrix
static SRTMatrix data_ov112_021f8418 = {
    { FX32_CONST(16), FX32_CONST(3), FX32_CONST(-6) },
    { FX32_CONST(0.2), FX32_CONST(0.2), FX32_CONST(0.2) },
};

// pokebw2's StaActBg_InitSystem
StaActBg *func_ov112_021f6f24(HeapID heapId, StaActing *stage) {
    StaActBg *bg = GFL_HeapAllocate(heapId, sizeof(StaActBg), FALSE, "sta_act_bg.c", 81);

    bg->heapId = heapId;
    bg->stage = stage;
    bg->scroll = 0;
    bg->mdlRes = NULL;
    return bg;
}

// pokebw2's StaActBg_TermSystem
void func_ov112_021f6f4c(StaActBg *bg) {
    if (bg->mdlRes != NULL) {
        func_ov112_021f7048(bg);
    }
    GFL_HeapFree(bg);
}

// pokebw2's StaActBg_UpdateSystem
void func_ov112_021f6f64(StaActBg *bg) {
    u8 i;

    if (bg->mdlRes != NULL) {
        for (i = 0; i < 3; i++) {
            GFL_G3DActorStepAnmFrameLoop(bg->actor, i, FX32_ONE);
        }
    }
}

// pokebw2's StaActBg_DrawSystem
void func_ov112_021f6f8c(StaActBg *bg) {
    if (bg->mdlRes != NULL) {
        MTX_Identity33_(&data_ov112_021f8418.rotation);
        GFL_G3DSysDrawObj(bg->actor, &data_ov112_021f8418);
    }
}

// pokebw2's StaActBg_LoadBg
void func_ov112_021f6fb0(StaActBg *bg, u8 bgNo) {
    u8 i;

    if (bg->mdlRes != NULL) {
        func_ov112_021f7048(bg);
    }
    bg->mdlRes = GFL_G3DSysReadArcSysResource(ARCID_MUSICAL, bgNo + STA_ACT_BG_FILE_MODEL);
    bg->anmRes[0] = GFL_G3DSysReadArcSysResource(ARCID_MUSICAL, bgNo + STA_ACT_BG_FILE_ANIME1);
    bg->anmRes[1] = GFL_G3DSysReadArcSysResource(ARCID_MUSICAL, bgNo + STA_ACT_BG_FILE_ANIME2);
    bg->anmRes[2] = GFL_G3DSysReadArcSysResource(ARCID_MUSICAL, bgNo + STA_ACT_BG_FILE_ANIME3);
    if (GFL_G3DResUploadTexData(bg->mdlRes)) {
        bg->mdl = GFL_G3DMdlCreate(bg->mdlRes, 0, bg->mdlRes);
        for (i = 0; i < 3; i++) {
            bg->anms[i] = GFL_G3DAnmCreate(bg->mdl, bg->anmRes[i], 0);
        }
        bg->actor = GFL_G3DActorCreate(bg->mdl, bg->anms, 3);
        for (i = 0; i < 3; i++) {
            GFL_G3DActorBindAnm(bg->actor, i);
        }
    }
}

// pokebw2's StaActBg_UnloadBg
static void func_ov112_021f7048(StaActBg *bg) {
    u8 i;

    GFL_G3DActorFree(bg->actor);
    for (i = 0; i < 3; i++) {
        GFL_G3DAnmFree(bg->anms[i]);
    }
    GFL_G3DMdlFree(bg->mdl);
    for (i = 0; i < 3; i++) {
        GFL_G3DResFree(bg->anmRes[i]);
    }
    GFL_G3DResFreeTexData(bg->mdlRes);
    GFL_G3DResFree(bg->mdlRes);
    bg->mdlRes = NULL;
}

// pokebw2's StaActBg_SetScrollOffset
void func_ov112_021f7098(StaActBg *bg, u16 scroll) {
    bg->scroll = scroll;
}
