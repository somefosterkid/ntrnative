#include "global.h"
struct TradeRoom;

#include "heap.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "party.h"
#include "player_data.h"
#include "unk_02037C94.h"
#include "bg_window.h"
#include "overlay_65_trade_room.h"
#include "gf_gfx_planes.h"

extern s32 ov65_0221E050;

void ov65_0221D0EC(struct TradeRoom *arg0, OverlayManager *arg1);

void ov65_0221D1C8(BgConfig *bgConfig);

void ov65_0221D0EC(TradeRoom *arg0, OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetArgs(man);
    *((void * *)&arg0->args) = v0;
    *((s32 *)&arg0->unk_6CC) = 4;
    *((s32 *)&arg0->subAppActive) = 0;
    *((s32 *)&arg0->yesNoMenuStep) = 0;
    *((s32 *)&arg0->unk_16C) = 6;
    *((s32 *)&arg0->unk_170) = 6;
    *((s32 *)&arg0->selectedSlot[0]) = 0;
    *((s32 *)&arg0->selectedSlot[1]) = 6;
    *((s32 *)&arg0->subStepResult) = 0;
    *((s32 *)&arg0->phase) = 0;
    *((s32 *)&arg0->connectStep) = 0;
    *((s32 * *)&arg0->subStepCallback) = &ov65_0221E050;
    *((s32 *)&arg0->partnerSyncStatus[0]) = 0;
    *((s32 *)&arg0->partnerSyncStatus[1]) = 0;
    *((s32 *)&arg0->didConfirmTrade) = 0;
    *((s32 *)&arg0->pendingPreviewLoadSide) = 0;
    *((s32 *)&arg0->lastSyncedValue) = -1;
    *((s32 *)&arg0->lastSyncedCmd) = -1;
    *((s32 *)&arg0->staggerCountdown) = 0;
    *((s32 *)&arg0->connectionTimeoutArmed) = 0;
    *((s32 *)&arg0->connectionTimeoutFrames) = 0;
    *((s32 *)&arg0->playerParty) = (s32) * (s32 *)((u8 *)v0 + 8);
    *((s32 *)&arg0->palPad) = (s32) * (s32 *)((u8 *)v0 + 0xC);
    *((s32 *)&arg0->saveData) = (s32) * (s32 *)((u8 *)v0 + 0x10);
    arg0->partnerParty = Heap_Alloc(HEAP_ID_26, SaveArray_Party_sizeof());
    Party_InitWithMaxSize(arg0->partnerParty, 6);
    memset(arg0->partnerParty, 0xFF, SaveArray_Party_sizeof());
    arg0->playerNameStr = PlayerProfile_GetPlayerName_NewString(*(PlayerProfile **)((u8 *)v0 + 4), HEAP_ID_26);
    arg0->itemLabelStr = NewString_ReadMsgData(*((MsgData * *)&arg0->msgLoader), 0x2A);
    sub_02039F68(*(SaveWiFiHistory **)((u8 *)v0 + 0x14));
}

void ov65_0221D1C8(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);

    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
}
