#include "global.h"

#include <dwc.h>
struct GTSApplicationState;
#include "gf_gfx_planes.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_0202D230.h"
#include "unk_02037C94.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_72_wfc_state.h"
#include "overlay_72_wfc_connect.h"

extern NNSFndHeapHandle ov72_0223B92C;

void *ov72_02238778(DWCAllocType param_1, u32 param_2, int param_3);

int ov72_022387C4(void);

void ov72_022387D0(NintendoWfcConnect *arg0, s32 arg1, s32 arg2);
void ov72_022380FC(void *);
s32 ov72_02238124(void);
s32 ov72_02238164(void);
s32 ov72_02238194(NintendoWfcConnect *);
s32 ov72_02238408(void *);
void ov72_022387D8(NintendoWfcConnect *arg0);
void ov72_02238800(NintendoWfcConnect *arg0);
s32 ov72_02238680(void *, s32, s32);
s32 ov72_022389C8(BgConfig *);
s32 ov72_02238BEC(void *);
s32 ov72_02238EE4(void *);
s32 ov72_02239040(NintendoWfcConnect *arg0);
s32 ov72_0223A350(void *, MsgData *, s32, s32, s32);
s32 ov72_0223A420(NintendoWfcConnect *);
s32 ov72_0223886C(NintendoWfcConnect *arg0);

void *ov72_02238778(DWCAllocType param_1, u32 param_2, int param_3) {
    OSIntrMode state;
    void *pvVar1;

    state = OS_DisableInterrupts();
    pvVar1 = NNS_FndAllocFromExpHeapEx(ov72_0223B92C, param_2, param_3);
    OS_RestoreInterrupts(state);
    return pvVar1;
}

void ov72_022387A0(DWCAllocType param_1, void *param_2, u32 param_3);

void ov72_022387A0(DWCAllocType param_1, void *param_2, u32 param_3) {
    OSIntrMode state;

    if (param_2 != NULL) {
        state = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(ov72_0223B92C, param_2);
        OS_RestoreInterrupts(state);
    }
    return;
}

int ov72_022387C4(void) {
    return WM_LINK_LEVEL_3 - DWC_GetLinkLevel();
}

void ov72_022387D0(NintendoWfcConnect *arg0, s32 arg1, s32 arg2) {
    arg0->nextScreen = arg1;
    arg0->unk_0018 = arg2;
}

void ov72_022387D8(NintendoWfcConnect *arg) {
    ov72_02238124();
    ov72_02238164();
    ov72_02238194(arg);
    ov72_02238408(arg);
    Main_SetVBlankIntrCB(ov72_022380FC, arg);
}

void ov72_02238800(NintendoWfcConnect *arg0) {
    s32 v0;

    SpriteTransfer_DeleteCharTransferTask(arg0->resObjs[0]);
    SpriteTransfer_DeleteCharTransferTask(arg0->resObjs[4]);
    SpriteTransfer_DeletePlttTransferTask(arg0->resObjs[1]);
    SpriteTransfer_DeletePlttTransferTask(arg0->resObjs[5]);
    v0 = 0;
    do {
        Destroy2DGfxResObjMan(arg0->resMan[v0]);
        v0 += 1;
    } while (v0 < 4);
    SpriteList_Delete(arg0->spriteList);
    arg0->spriteList = NULL;
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

s32 ov72_0223886C(NintendoWfcConnect *arg0) {
    NintendoWifiConnectArgs *v0;

    ov72_02239040(arg0);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 8, 1, HEAP_ID_67);
    ov72_022389C8(arg0->bgConfig);
    ov72_02238BEC(arg0);
    ov72_02238EE4(arg0);
    GfGfx_EngineATogglePlanes(1U, 1U);
    GfGfx_EngineATogglePlanes(2U, 1U);
    GfGfx_EngineBTogglePlanes(1U, 1U);
    GfGfx_EngineBTogglePlanes(2U, 1U);
    arg0->ownRank = sub_0202D488(arg0->args->frontierData, 0U);
    arg0->rank = arg0->ownRank;
    arg0->room = 1;
    arg0->waitingIcon = NULL;
    if (DWC_CheckInet() == 0) {
        v0 = arg0->args;
        if (v0->unk24 != 0) {
            sub_02039418(v0->saveData);
            ov72_0223A350(arg0, arg0->msgDataWfc, 1, 1, 0xF0F);
            ov72_02238680(arg0, 0x2F, 2);
            ov72_0223A420(arg0);
        } else {
            Sys_ClearSleepDisableFlag(4);
            arg0->state = NINTENDO_WFC_STATE_ASK_CONNECT;
        }
    } else {
        sub_02039418(arg0->args->saveData);
        arg0->state = NINTENDO_WFC_STATE_ASK_LOG_OUT;
    }
    return 2;
}
