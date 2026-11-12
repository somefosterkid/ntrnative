// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_res.h, the lines in
// use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_RES_H
#define BWPORT_APP_BATTLE_RECORDER_BR_RES_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The Battle Recorder's resources (br_res.c): the font, the messages, the BG and OBJ graphics, and the color the
// player chose for it. pokebw2's guessed names are in the comments

// The graphics resources of a cell actor, as they are loaded
typedef struct {
    u32 cell;
    u32 chr;
    u32 plt;
} BrResObjData;

// pokebw2's BrRes_LoadBG, BrRes_UnloadBG, BrRes_LoadOBJ and BrRes_UnloadOBJ
void func_ov186_021fb964(BrRes *p_wk, u32 bgID, HeapID heapId);
void func_ov186_021fbda4(BrRes *p_wk, u32 bgID);
void func_ov186_021fbe54(BrRes *p_wk, u32 objID, HeapID heapId);
void func_ov186_021fc0c8(BrRes *p_wk, u32 objID);
// pokebw2's BrRes_GetObjData: whether the resource is loaded, and if it is, its data
BOOL func_ov186_021fc124(const BrRes *p_wk, u32 objID, BrResObjData *p_data);
// pokebw2's BrRes_LoadCommonPltt and BrRes_UnloadCommonPltt: the palettes of the player's color, for CLACT_VRAM_MAIN or
// CLACT_VRAM_SUB
void func_ov186_021fc148(BrRes *p_wk, u32 vramType, HeapID heapId);
void func_ov186_021fc1f8(BrRes *p_wk, u32 vramType);
// pokebw2's BrRes_GetFont and BrRes_GetMsgData
Font *func_ov186_021fc20c(const BrRes *p_wk);
MsgData *func_ov186_021fc214(const BrRes *p_wk);

// pokebw2's BrRes_GetWordSet
WordSet *func_ov186_021fc21c(const BrRes *p_wk);

// pokebw2's BrRes_GetColor
u32 func_ov186_021fc238(const BrRes *p_wk);
// pokebw2's BrRes_GetFadeColor
// The fade color of the player's color
u16 func_ov186_021fc224(const BrRes *p_wk);
// pokebw2's BrRes_LoadColorPlttToFade
void func_ov186_021fc240(BrRes *p_wk, BrFade *fade, HeapID heapId);

// Black 2's; twinned code confirms the values of BR_RES_OBJ_BROWSE_BTN_M, BR_RES_OBJ_BROWSE_BTN_S,
// BR_RES_OBJ_MUSICAL_BTN_M, BR_RES_OBJ_MUSICAL_BTN_S, BR_RES_OBJ_SIDEBAR_M and BR_RES_OBJ_SIDEBAR_S in Black
// The cell actor resources that BrRes_LoadOBJ loads. The buttons are for the main screen and then the sub screen
enum {
    BR_RES_OBJ_BROWSE_BTN_M,
    BR_RES_OBJ_BROWSE_BTN_S,
    BR_RES_OBJ_MUSICAL_BTN_M,
    BR_RES_OBJ_MUSICAL_BTN_S,
    BR_RES_OBJ_SIDEBAR_M,
    BR_RES_OBJ_SIDEBAR_S,
    BR_RES_OBJ_MAX = 14,
};

// pokebw2's BrRes_Exit
void func_ov186_021fb92c(BrRes *p_wk);

// Black 2's, not checked against Black
// The color of the Global Link's modes, which the player can't choose
#define BR_RES_COLOR_GLOBAL 1

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrRes_SetColor
void func_ov186_021fc230(BrRes *p_wk, u32 color);
// pokebw2's BrRes_Init
// isUseColor is FALSE in the Global Link's modes, which have their own color
BrRes *func_ov186_021fb884(u8 color, BOOL isBrowse, HeapID heapId);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_RES_H
