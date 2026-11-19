#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "game_stats.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "render_text.h"
#include "save.h"
#include "signature_app.h"
#include "save_trainer_card.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "system.h"
#include "unk_020210A0.h"

void ov52_021E83A4(void);

void ov52_021E83C4(BgConfig *bgConfig);

void ov52_021E84CC(SignatureApp *signatureApp);

void ov52_021E85DC(SignatureApp *signatureApp, NARC *narc);

void ov52_021E86DC(void);

void ov52_021E870C(SignatureApp *signatureApp, NARC *narc);

void ov52_021E888C(SignatureApp *signatureApp);

void ov52_021E89D4(SignatureApp *signatureApp, OverlayManager *manager);

void ov52_021E837C(void *param0);

s32 TrainerCardSignature_Init(OverlayManager *manager, s32 *state);

s32 TrainerCardSignature_Init(OverlayManager *manager, s32 *state) {
    SignatureApp *signatureApp;
    BgConfig *pBVar2;
    NARC *narc;
    MessageFormat *pMVar3;
    MsgData *pMVar4;
    SaveData *pSVar5;
    struct SaveTrainerCard *data;
    int *piVar6;
    GameStats *pGVar7;
    Options *pOVar8;

    switch (*state) {
    case 0:
        sub_0200FBF4(PM_LCD_TOP, 0);
        sub_0200FBF4(PM_LCD_BOTTOM, 0);
        Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffffe0ff;
        reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xffffe0ff;
        Heap_Create(HEAP_ID_3, HEAP_ID_39, 0x40000);
        signatureApp = OverlayManager_CreateAndGetData(manager, sizeof(SignatureApp), HEAP_ID_39);
        memset(signatureApp, 0, sizeof(SignatureApp));
        pBVar2 = BgConfig_Alloc(HEAP_ID_39);
        signatureApp->bgConfig = pBVar2;
        narc = NARC_New(NARC_a_0_9_1, HEAP_ID_39);
        pMVar3 = MessageFormat_New(HEAP_ID_39);
        signatureApp->template = pMVar3;
        pMVar4 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xfc, HEAP_ID_39);
        signatureApp->loader = pMVar4;
        TextFlags_SetCanTouchSpeedUpPrint(1);
        TextFlags_SetCanABSpeedUpPrint(1);
        SetKeyRepeatTimers(4, 8);
        ov52_021E83A4();
        ov52_021E83C4(signatureApp->bgConfig);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 0x10, 1, HEAP_ID_39);
        pSVar5 = OverlayManager_GetArgs(manager);
        data = Save_TrainerCard_Get(pSVar5);
        piVar6 = TrainerCard_GetSignature(data);
        signatureApp->savedSignature = piVar6;
        pSVar5 = OverlayManager_GetArgs(manager);
        pGVar7 = Save_GameStats_Get(pSVar5);
        signatureApp->records = pGVar7;
        pSVar5 = OverlayManager_GetArgs(manager);
        pOVar8 = Save_PlayerData_GetOptionsAddr(pSVar5);
        signatureApp->options = pOVar8;
        ov52_021E85DC(signatureApp, narc);
        EnableTouchPad();
        InitializeTouchPad(1);
        Main_SetVBlankIntrCB(ov52_021E837C, signatureApp->bgConfig);
        FontID_Alloc(FONT_SUBSCREEN, HEAP_ID_39);
        ov52_021E84CC(signatureApp);
        ov52_021E86DC();
        ov52_021E870C(signatureApp, narc);
        ov52_021E888C(signatureApp);
        ov52_021E89D4(signatureApp, manager);
        Sound_SetSceneAndPlayBGM(0x38, 0, 0);
        reg_GX_POWCNT = reg_GX_POWCNT & 0xFFFF7FFF;
        NARC_Delete(narc);
        *state = *state + 1;
        break;
    case 1:
        OverlayManager_GetData(manager);
        *state = 0;
        return 1;
    }
    return 0;
}
