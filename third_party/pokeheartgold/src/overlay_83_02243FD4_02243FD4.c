#include "global.h"

#include "constants/heap.h"

#include "frontier/overlay_80_022340E8.h"

#include "battle_castle_app_sprite.h"
#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_02035900.h"
#include "unk_0203A3B0.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "touchscreen_list_menu.h"

void ov83_02244394(BattleCastleOpponentApp *arg0);

void ov83_02244408(BattleCastleOpponentApp *arg0);

void ov83_02244488(void *data);

void ov83_02247844(void *arg0);

u8 ov80_02237B58(u8, s32);

s32 ov83_02244DA0(void *, s32 *, s32 *, u8);

s32 ov83_02244DF4(void *, void *, s32 *, void *, s32 *);

s32 ov83_02245C80(BattleCastleOpponentApp *app);

s32 ov83_02245CE8(BattleCastleOpponentApp *app, u32 *x, u32 *y);

s32 ov83_02245D48(BattleCastleOpponentApp *arg0);



void ov83_02245F24(BattleCastleOpponentApp *app);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1);
#else
void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02246988(BattleCastleOpponentApp *param_1);
#else
void ov83_02246988(BattleCastleOpponentApp *param_1);
#endif

BattleCastleAppSprite *ov83_02247454(BattleCastleAppSpriteManager *, s32, s32, s32, s32, s16, s16, s32, void *);

BattleCastleAppSprite *ov83_022474C4(void *, s32, s32, s32, s32, s16, s16, s32, const u8 *);

s32 ov83_0224755C(void *, s32);

s32 ov83_022475EC(void *, Pokemon *);

s32 ov83_022478D4(BgConfig *, void *, u8);

struct GridInputHandler *ov83_02247A7C(void *, u8, s32);

struct TouchscreenListMenuSpawner *ov83_02247CB8(struct SpriteList *, PaletteData *);

void ov83_02243FD4(BattleCastleOpponentApp *app);

#if defined(SDK_PORT) || defined(NONMATCHING)
#define BALLSPR app->ballSprites[var_r6_2]
#define BARSPR app->barSprites[var_r6_2]
#define MONSPR app->monSprites[var_r6_2]
#define FLAGSPR app->flagSprites[sp14][var_r7_2]
#else
// DS: pointer walks over the app base (+4 per mon, +8 per flag row)
#define BALLSPR (*(BattleCastleAppSprite **)((u8 *)var_r4_2 + offsetof(BattleCastleOpponentApp, ballSprites)))
#define BARSPR (*(BattleCastleAppSprite **)((u8 *)var_r4_2 + offsetof(BattleCastleOpponentApp, barSprites)))
#define MONSPR (*(BattleCastleAppSprite **)((u8 *)var_r4_2 + offsetof(BattleCastleOpponentApp, monSprites)))
#define FLAGSPR (*(BattleCastleAppSprite **)((u8 *)var_r4_3 + offsetof(BattleCastleOpponentApp, flagSprites)))
#endif

void ov83_02243FD4(BattleCastleOpponentApp *app) {
    s32 sp34;
    s32 sp30;
    u8 sp2C[4];
    u8 sp28[4];
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    void *sp24;
#endif
    s32 sp20;
    s32 sp1C;
    u32 sp18;
    s32 sp14;
    u32 var_r7;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r7_2;
    s32 var_r6_3;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    void *var_r4_2;
    struct BattleCastleAppSprite **var_r4_3;
#endif
    app->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_107);
    ov83_02244394(app);
    ov83_02244408(app);
    FontID_Alloc(FONT_UNK_4, HEAP_ID_107);
    app->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x21, HEAP_ID_107);
    app->strTemplate = MessageFormat_New(HEAP_ID_107);
    app->displayStr = String_New(0x258U, HEAP_ID_107);
    app->fmtStr = String_New(0x258U, HEAP_ID_107);
    var_r6 = 0;
    do {
        app->simpleMenuStr[var_r6] = String_New(0x20U, HEAP_ID_107);
        var_r6 += 1;
    } while (var_r6 < 3);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_107);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_107);
    app->msgPrinter = MessagePrinter_New(1U, 2U, 0U, HEAP_ID_107);
    ov83_022478D4(app->bgConfig, &app->windows[0], 1);
    ov83_02244DF4(app, &sp2C[2], (s32 *)sp2C, &sp28[2], (s32 *)sp28);
    if (ov80_02237D8C(app->challengeType) == 0) {
        var_r7 = 0x40;
        sp1C = 0x3C;
    } else {
        var_r7 = 0x20;
        sp1C = 0x1C;
    }
    temp_r0 = ov80_02237B58(app->challengeType, 1);
    var_r6_2 = 0;
    sp18 = temp_r0;
    if (temp_r0 > 0) {
        sp14 = sp18;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
        var_r4_2 = app;
#endif
        do {
            BALLSPR = ov83_02247454(&app->spriteMan, 0, 0, 0, 7, sp1C, 0x3E, 2, 0);
            BARSPR = ov83_02247454(&app->spriteMan, 0, 0, 0, 0xF, var_r7, 0x4E, 3, 0);
            MONSPR = ov83_02247454(&app->spriteMan, var_r6_2 + 0xA, 0xA, 5, 1, var_r7, 0x3A, 2, 0);
            ov83_022475EC(MONSPR, Party_GetMonByIndex(app->opponentsParty, var_r6_2));
            if ((*((u8 *)((app->identityUnlocked) + var_r6_2))) == 0) {
                ov83_0224755C(BALLSPR, 1);
                ov83_0224755C(MONSPR, 0);
                ov83_0224755C(BARSPR, 0);
            } else {
                ov83_0224755C(BALLSPR, 0);
                ov83_0224755C(MONSPR, 1);
                ov83_0224755C(BARSPR, 1);
            }
            var_r6_2 += 1;
            sp1C += 0x40;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
            var_r4_2 += 4;
#endif
            var_r7 += 0x40;
        } while (var_r6_2 < sp14);
    }
    ov83_02244DA0(app, &sp34, &sp30, 0);
    app->cursorSprite = ov83_02247454(&app->spriteMan, 0, 0, 0, 1, sp34, sp30, 2, 0);
    app->partnerCursorSprite = ov83_02247454(&app->spriteMan, 0, 0, 0, 2, sp34, sp30, 2, 0);
    if (ov80_02237D8C(app->challengeType) == 0) {
        ov83_0224755C(app->partnerCursorSprite, 0);
    }
    sp14 = 0;
    sp20 = 0;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    sp24 = app;
#endif
    do {
#if !defined(SDK_PORT) && !defined(NONMATCHING)
        var_r4_3 = (struct BattleCastleAppSprite **)sp24;
#endif
        var_r7_2 = 0;
        var_r6_3 = 0;
loop_15:
        ov83_02245CE8(app, &sp34, &sp30);

        FLAGSPR = ov83_02247454(&app->spriteMan, 0, 0, 0, 0xC, sp34 + sp20, sp30 + var_r6_3, 2, 0);
        ov83_0224755C(FLAGSPR, 0);
        var_r7_2 += 1;
        var_r6_3 += 0xC;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
        var_r4_3 += 1;
#endif
        if (var_r7_2 < 2) {
            goto loop_15;
        }
        sp20 += 0x40;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
        sp24 += 8;
#endif
        temp_r0_2 = sp14 + 1;
        sp14 = temp_r0_2;
    } while (temp_r0_2 < 4);
    ov83_02245C80(app);
    app->sparklesSprite = ov83_02247454(&app->spriteMan, 0, 0, 0, 0xB, 0x14, 0x14, 0, 0);
    ov83_0224755C(app->sparklesSprite, 0);
    ov83_02245D48(app);
    ov83_02245F24(app);
    ov83_02246114(app, 1);
    app->selectedMonSprite = ov83_022474C4(&app->spriteMan, 2, 2, 2, 0, 0x30, 0x28, 0, 0);
    ov83_02246988(app);
    app->partyGridInput = ov83_02247A7C(app, 1, app->numSlots);
    app->touchMenuSpawner = ov83_02247CB8(app->spriteMan.spriteList, app->plttData);
    ov83_02247844(&app->yesNoPrompt);
    if (sub_02037474() != 0) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        sub_0203A880();
    }
    G2_SetBlendAlpha(0, 0xE, 6, 0xA);
    Main_SetVBlankIntrCB(ov83_02244488, app);
}
