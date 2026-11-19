#include "global.h"

#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "touchscreen_list_menu.h"
#include "yes_no_prompt.h"
#include "overlay_43_palpad.h"

void ov43_0222A81C(PalPadGfx *gfx);
void ov43_0222A87C(PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222A8A8(PalPadGfx *gfx);
void ov43_0222A8C0(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapID);
void ov43_0222A960(PalPadGfx *gfx);

void ov43_0222A81C(PalPadGfx *gfx) {
    s32 i;

    SpriteTransfer_DeleteCharTransferTask(gfx->spriteResources[0]);
    SpriteTransfer_DeletePlttTransferTask(gfx->spriteResources[1]);
    for (i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(gfx->resourceManagers[i], gfx->spriteResources[i]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(gfx->resourceManagers[i]);
    }
    SpriteList_Delete(gfx->spriteList);
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
    GF_DestroyVramTransferManager();
}

void ov43_0222A87C(PalPadGfx *gfx, enum HeapID heapID) {
    gfx->msgFormat = MessageFormat_New(heapID);
    gfx->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x30B, heapID);
    FontID_Alloc(FONT_UNK_4, heapID);
}

void ov43_0222A8A8(PalPadGfx *gfx) {
    FontID_Release(FONT_UNK_4);
    DestroyMsgData(gfx->msgData);
    MessageFormat_Delete(gfx->msgFormat);
}

void ov43_0222A8C0(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapID) {
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_11_OFFSET, heapID);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_14_OFFSET, heapID);
    LoadUserFrameGfx2(gfx->bgConfig, GF_BG_LYR_SUB_0_F, 0x46U, 0xCU, shared->frame, heapID);
    InitWindow(&gfx->titleWindow);
    AddWindowParameterized(gfx->bgConfig, &gfx->titleWindow, 1U, 1U, 0U, 0x18U, 3U, 0xBU, 0x64U);
    gfx->titleText = String_New(0x80U, heapID);
    gfx->titleBuffer = String_New(0x80U, heapID);
    gfx->pageText = String_New(0x80U, heapID);
    gfx->pageBuffer = String_New(0x80U, heapID);
    gfx->listMenuSpawner = TouchscreenListMenuSpawner_Create(heapID, NULL);
    gfx->yesNoPrompt = YesNoPrompt_Create(heapID);
}

void ov43_0222A960(PalPadGfx *gfx) {
    YesNoPrompt_Destroy(gfx->yesNoPrompt);
    TouchscreenListMenuSpawner_Destroy(gfx->listMenuSpawner);
    String_Delete(gfx->pageText);
    String_Delete(gfx->pageBuffer);
    String_Delete(gfx->titleText);
    String_Delete(gfx->titleBuffer);
    RemoveWindow(&gfx->titleWindow);
}
