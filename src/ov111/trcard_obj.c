// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov186/trcard_obj.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/trainer_card.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "system/app_menu_common.h"

// The trainer card screen's cell actors: its buttons, the scroll arrows and the rank. The ROM has no name for this
// file; trcard_obj.c is a guess

typedef struct {
    u16 x;
    u16 y;
    // Which of the work's resources the actor uses: the card's own (0) or the touch bar's (1)
    u8 charRes;
    u8 paletteRes;
    u8 cellRes;
    u8 bgPriority;
    u32 sequence;
} TrCardObjActorDef;

// pokebw2's TrCardObj_Init
void func_ov111_021c6d9c(TrainerCardObjWork *obj, const BGSysVRAMConfig *vram) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_TRAINER_CARD_GRA, obj->heapId);

    ClActSys_Create(&data_020a1448, vram, obj->heapId);
    obj->unit = func_0204b100(35, 0, obj->heapId);
    obj->res[0].palette = func_0204ac18(arc, 12, CLACT_VRAM_SUB, 0, 0, 5, obj->heapId);
    obj->res[0].chars = func_0204a6c8(arc, 29, FALSE, CLACT_VRAM_SUB, obj->heapId);
    obj->res[0].cellAnims = func_0204af28(arc, 46, 48, obj->heapId);
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), obj->heapId);
    obj->res[1].palette = func_0204abf0(arc, func_02027614(), CLACT_VRAM_SUB, 0xa0, obj->heapId);
    obj->res[1].chars = func_0204a6c8(arc, func_02027618(), FALSE, CLACT_VRAM_SUB, obj->heapId);
    obj->res[1].cellAnims = func_0204af28(arc, func_0202761c(0), func_02027620(0), obj->heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysSetBGEnabledA(0x10, TRUE);
    GFL_BGSysSetBGEnabledB(0x10, TRUE);
}

// pokebw2's TrCardObj_AddActors
void func_ov111_021c6e8c(TrainerCardObjWork *obj) {
    TrCardObjActorDef defs[TRCARD_ACTOR_COUNT] = {
        { 228, 168, 1, 1, 1, 0, 1 },  { 204, 168, 1, 1, 1, 0, 0 }, { 4, 168, 0, 0, 0, 0, 16 },
        { 0, 0, 0, 0, 0, 0, 5 },      { 36, 168, 0, 0, 0, 0, 18 }, { 68, 168, 0, 0, 0, 0, 6 },
        { 100, 168, 0, 0, 0, 0, 22 }, { 180, 172, 1, 1, 1, 0, 6 },
    };
    int i;
    ClActorSetup setup;

    for (i = 0; i < TRCARD_ACTOR_COUNT; i++) {
        setup.x = defs[i].x;
        setup.y = defs[i].y;
        setup.sequence = defs[i].sequence;
        setup.bgPriority = defs[i].bgPriority;
        // BUG: the palette's resource is passed for the characters and the characters' for the palette. Both kinds
        // are numbered from 0 in the order they are loaded in, so the indices are the same
#ifdef BUGFIX
        obj->actors[i] = func_0204b294(obj->unit, obj->res[defs[i].charRes].chars, obj->res[defs[i].paletteRes].palette,
                                       obj->res[defs[i].cellRes].cellAnims, &setup, CLACT_SURFACE_SUB, obj->heapId);
#else
        obj->actors[i] = func_0204b294(obj->unit, obj->res[defs[i].paletteRes].palette, obj->res[defs[i].charRes].chars,
                                       obj->res[defs[i].cellRes].cellAnims, &setup, CLACT_SURFACE_SUB, obj->heapId);
#endif
        func_0204b3dc(obj->actors[i], TRUE);
        func_0204b954(obj->actors[i], FX32_ONE);
        func_0204b92c(obj->actors[i], TRUE);
        if (i == TRCARD_ACTOR_3) {
            func_0204b3dc(obj->actors[i], FALSE);
        }
    }
    func_0204b3dc(obj->actors[TRCARD_ACTOR_BRUSH], FALSE);
}

// pokebw2's TrCardObj_Exit
void func_ov111_021c6f4c(TrainerCardObjWork *obj) {
    u8 i;

    for (i = 0; i < 2; i++) {
        func_0204ada4(obj->res[i].palette);
        func_0204a8d4(obj->res[i].chars);
        func_0204afd8(obj->res[i].cellAnims);
    }
    func_0204b1cc(obj->unit);
    func_0204a5ac();
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
}

// Restarts an actor on an animation
// pokebw2's TrCardObj_SetAnime
void func_ov111_021c6fa0(TrainerCardObjWork *obj, int actor, u8 sequence, u8 visible) {
    func_0204b878(obj->actors[actor], sequence);
    func_0204b9a0(obj->actors[actor]);
    func_0204b3dc(obj->actors[actor], visible);
}

