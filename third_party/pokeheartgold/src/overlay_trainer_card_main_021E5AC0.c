#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "menu_input_state.h"
#include "options.h"
#include "overlay_manager.h"
#include "overlay_trainer_card_main.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_020210A0.h"
#include "unk_02035900.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
#include "overlay_trainer_card.h"
#include "trainer_card_main.h"

struct AsyncWriteManager;

struct Background;

struct BgConfig;

struct G2dRenderer;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResMan;

struct IGT;

struct MenuInputStateMgr;

struct MsgData;

struct NARC;

struct Options;

struct OverlayManager;

struct OverlayManagerTemplate;

struct SaveArrayHeader;

struct SaveData;

struct SaveSlotSpec;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct String;

struct TouchHitboxController;

struct TouchHitboxControllerTemplate;

struct TrainerCardAppArgs;

struct TrainerCardAppSettings;

struct TrainerCardMain;

struct TrainerCardMainCard;

struct TrainerCardMainSprites;

struct Window;

struct YesNoPrompt;

struct YesNoPromptButton;

struct YesNoPromptSubstruct00_18;

struct _MAT;

struct _MAT_ENTRY;

void *memset(void *data, int value, size_t size);

void ov51_021E78F8(TrainerCardMainSprites *sprites);

void ov51_021E7AF4(TrainerCardMainSprites *sprites, const u8 *badgeFlags, int showKantoBadges);

void ov51_021E7BD0(TrainerCardMainSprites *sprites);

void ov51_021E7DA4(u8 *pairs);

void ov51_021E5F64(TrainerCardMain *cardMain);

void ov51_021E60D4(void);

void ov51_021E6238(BgConfig *bgConfig);

void ov51_021E6354(TrainerCardMain *cardMain);

void ov51_021E6734(TrainerCardMain *cardMain);

void ov51_021E6B88(void *data);

void ov51_021E6C00(void);

void ov51_021E6E60(TrainerCardMain *cardMain);

void ov51_021E7664(Window *window, BOOL show, String *string);

void ov51_021E76A4(TrainerCardMain *cardMain, int unused);

BOOL TrainerCardMainApp_Init(OverlayManager *manager, int *state) {
    TrainerCardMain *cardMain;
    Options *options;
    int i;
    u16 mask;

    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    sub_0200FBF4(PM_LCD_TOP, RGB_BLACK);
    sub_0200FBF4(PM_LCD_BOTTOM, RGB_BLACK);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    SetKeyRepeatTimers(4, 8);
    Heap_Create(HEAP_ID_3, HEAP_ID_25, 0x50000);
    cardMain = OverlayManager_CreateAndGetData(manager, sizeof(TrainerCardMain), HEAP_ID_25);
    memset(cardMain, 0, sizeof(TrainerCardMain));
    cardMain->args = OverlayManager_GetArgs(manager);
    cardMain->card = (TrainerCardMainCard *)cardMain->args;
    cardMain->inTouchMode = MenuInputStateMgr_GetState(cardMain->args->settings.menuInputStatePtr);
    options = Save_PlayerData_GetOptionsAddr(cardMain->args->settings.saveData);
    cardMain->textSpeed = Options_GetTextFrameDelay(options);
    cardMain->frame = Options_GetFrame(options);
    if (sub_02037474() == 1) {
        cardMain->isConnected = TRUE;
    }
    if (cardMain->card->clearMonth != 0) {
        cardMain->showKantoBadges = TRUE;
    } else {
        cardMain->showKantoBadges = FALSE;
    }
    if (cardMain->isConnected == TRUE && !cardMain->args->settings.gameCleared) {
        cardMain->showKantoBadges = FALSE;
    }
    if (cardMain->card->version == VERSION_HEARTGOLD || cardMain->card->version == VERSION_SOULSILVER) {
        mask = 1;
        for (i = 0; i < 16; i++) {
            if (cardMain->card->badges & mask) {
                cardMain->badgeFlags[i] = 1;
            } else {
                cardMain->badgeFlags[i] = 0;
            }
            mask <<= 1;
        }
    } else {
        for (i = 0; i < 16; i++) {
            cardMain->badgeFlags[i] = 0;
        }
    }
    cardMain->bgConfig = BgConfig_Alloc(HEAP_ID_25);
    ov51_021E5F64(cardMain);
    ov51_021E60D4();
    ov51_021E6238(cardMain->bgConfig);
    ov51_021E6354(cardMain);
    EnableTouchPad();
    InitializeTouchPad(4);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    Sound_SetSceneAndPlayBGM(0x38, SEQ_NONE, 0);
    ov51_021E7DA4(&cardMain->unk_33A0[0][0]);
    PlaySE(SEQ_SE_DP_CARD3);
    ov51_021E78F8(&cardMain->sprites);
    ov51_021E7AF4(&cardMain->sprites, cardMain->badgeFlags, cardMain->showKantoBadges);
    ov51_021E7BD0(&cardMain->sprites);
    ov51_021E6E60(cardMain);
    ov51_021E7664(&cardMain->windows[5], TRUE, cardMain->colonString);
    if (cardMain->args->settings.showBack) {
        cardMain->showingBack = TRUE;
    } else {
        cardMain->showingBack = FALSE;
    }
    cardMain->args->settings.showBack = 0;
    cardMain->unk_3434 = 0;
    cardMain->unk_3108 = -1;
    cardMain->unk_339C = 0;
    cardMain->unk_3438 = 0;
    ov51_021E6C00();
    ov51_021E6734(cardMain);
    ov51_021E76A4(cardMain, 0);
    Main_SetVBlankIntrCB(ov51_021E6B88, cardMain);
    sub_0203A964();
    GF_SndHandleSetPlayerVolume(1, 0x2a);
    BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, RGB_BLACK, 6, 1, HEAP_ID_25);
    if (cardMain->card->version != VERSION_HEARTGOLD && cardMain->card->version != VERSION_SOULSILVER) {
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    }
    return TRUE;
}
