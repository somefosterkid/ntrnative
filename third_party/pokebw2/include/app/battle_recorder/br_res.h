#ifndef POKEBW2_APP_BATTLE_RECORDER_BR_RES_H
#define POKEBW2_APP_BATTLE_RECORDER_BR_RES_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "struct_decls.h"
#include "system/printsys.h"
#include "system/wordset.h"

// The Battle Recorder's resources (br_res.c): the font, the messages, the BG and OBJ graphics, and the color the
// player chose for it

// The color of the Global Link's modes, which the player can't choose
#define BR_RES_COLOR_GLOBAL 1

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

// The graphics resources of a cell actor, as they are loaded
typedef struct {
    u32 cell;
    u32 chr;
    u32 plt;
} BrResObjData;

// isUseColor is FALSE in the Global Link's modes, which have their own color
BrRes *BrRes_Init(u32 color, BOOL isUseColor, HeapID heapId);
void BrRes_Exit(BrRes *p_wk);
void BrRes_LoadBG(BrRes *p_wk, u32 bgID, HeapID heapId);
void BrRes_UnloadBG(BrRes *p_wk, u32 bgID);
void BrRes_LoadOBJ(BrRes *p_wk, u32 objID, HeapID heapId);
void BrRes_UnloadOBJ(BrRes *p_wk, u32 objID);
// Whether the resource is loaded, and if it is, its data
BOOL BrRes_GetObjData(const BrRes *p_wk, u32 objID, BrResObjData *p_data);
// The palettes of the player's color, for CLACT_VRAM_MAIN or CLACT_VRAM_SUB
void BrRes_LoadCommonPltt(BrRes *p_wk, u32 vramType, HeapID heapId);
void BrRes_UnloadCommonPltt(BrRes *p_wk, u32 vramType);
Font *BrRes_GetFont(const BrRes *p_wk);
MsgData *BrRes_GetMsgData(const BrRes *p_wk);
WordSet *BrRes_GetWordSet(const BrRes *p_wk);
// The fade color of the player's color
u16 BrRes_GetFadeColor(const BrRes *p_wk);
void BrRes_SetColor(BrRes *p_wk, u32 color);
u32 BrRes_GetColor(const BrRes *p_wk);
void BrRes_LoadColorPlttToFade(BrRes *p_wk, BrFade *fade, HeapID heapId);

#endif // POKEBW2_APP_BATTLE_RECORDER_BR_RES_H
