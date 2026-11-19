#include "global.h"

#include "constants/heap.h"

#include "frontier/overlay_80_022340E8.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_02035900.h"
#include "unk_0203A3B0.h"
#include "unk_0208805C.h"
#include "overlay_83_battle_castle_self_app.h"
#include "battle_castle_app_sprite.h"
#include "overlay_80_02237A70.h"

void ov83_0223F690(BattleCastleSelfApp *arg0);

void ov83_0223F70C(BattleCastleSelfApp *arg0);

void ov83_02247844(BattleCastleYesNoPrompt *yesNo);


void ov83_0223F7A0(void *);

s32 ov83_02240E70(void *, s32 *, s32 *, u8);

s32 ov83_02240F7C(void *, void *, s32 *, void *, s32 *);

u32 ov83_022411B0(BattleCastleSelfApp *app, u8 barColor);

u32 ov83_022411DC(BattleCastleSelfApp *app, u8 barColor);

void ov83_02241E18(BattleCastleSelfApp *);

void ov83_02241FF0(BattleCastleSelfApp *app);

s32 ov83_022421E0(void *, s32);

s32 ov83_02242894(s32, void *, s32 *);

BattleCastleAppSprite *ov83_02247454(BattleCastleAppSpriteManager *spriteMan, u32 param_2, u32 param_3, u32 param_4, u32 param_5, s16 param_6, s16 param_7, u32 param_8, const u8 *unused);

BattleCastleAppSprite *ov83_022474C4(BattleCastleAppSpriteManager *spriteMan, u32 param_2, u32 param_3, u32 param_4, u32 param_5, s16 param_6, s16 param_7, u32 param_8, const u8 *unused);

void ov83_0224755C(BattleCastleAppSprite *sprite, BOOL flag);

void ov83_022475EC(BattleCastleAppSprite *sprite, Pokemon *mon);

void ov83_02247668(BattleCastleAppSprite *obj, struct BoxPokemon *boxMon, int species, u32 personality);

void ov83_022478D4(BgConfig *bgConfigs, Window *windows, u8 isOpponentApp);

struct GridInputHandler *ov83_02247A24(void *callbackParam, u8 initialPos, s32 arg2);

struct TouchscreenListMenuSpawner *ov83_02247CB8(struct SpriteList *spriteList, PaletteData *plttData);

void ov83_0223F200(BattleCastleSelfApp *arg0);

void ov83_0223F200(BattleCastleSelfApp *arg0) {
    Pokemon *temp_r0_2;
    s32 sp3C;
    s32 sp38;
    u8 sp34[4];
    u8 sp30[4];
    s32 sp28;
    u32 sp24;
    u32 sp20;
    Pokemon *sp1C;
    u8 sp2C[4];
    s32 sp14;
    u32 sp18;
    u32 var_r7;
    s32 temp_r0;
    s32 temp_r0_4;
    s32 temp_r1;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r4_3;
    u32 temp_r0_3;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    u32 *var_r6_3;
#endif

    arg0->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_107);
    ov83_0223F690(arg0);
    ov83_0223F70C(arg0);
    FontID_Alloc(FONT_UNK_4, HEAP_ID_107);
    arg0->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1F, HEAP_ID_107);
    arg0->itemMsgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xDD, HEAP_ID_107);
    arg0->strTemplate = MessageFormat_New(HEAP_ID_107);
    arg0->displayStr = String_New(0x258U, HEAP_ID_107);
    arg0->fmtStr = String_New(0x258U, HEAP_ID_107);
    var_r6 = 0;
    do {
        arg0->yesNoStrs[var_r6] = String_New(0x20U, HEAP_ID_107);
        var_r6 += 1;
    } while (var_r6 < 3);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_107);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_107);
    arg0->msgPrinter = MessagePrinter_New(1U, 2U, 0U, HEAP_ID_107);
    ov83_022478D4(arg0->bgConfig, arg0->windows, 0);
    ov83_02240F7C(arg0, &sp34[2], (s32 *)sp34, &sp30[2], (s32 *)sp30);
    arg0->upArrowSprite = ov83_02247454(&arg0->spriteMan, 0, 0, 0, 4, 0xA0, 0xA, 0, 0);
    arg0->downArrowSprite = ov83_02247454(&arg0->spriteMan, 0, 0, 0, 5, 0xA0, 0x7C, 0, 0);
    ov83_0224755C(arg0->upArrowSprite, 0);
    ov83_0224755C(arg0->downArrowSprite, 0);
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        var_r7 = 0x40;
        sp18 = 0x48;
    } else {
        var_r7 = 0x20;
        sp18 = 0x28;
    }
    temp_r0 = ov80_02237B24(arg0->challengeType, 1);
    var_r6_2 = 0;
    sp14 = temp_r0;
    if (temp_r0 > 0) {
        do {
            arg0->itemIconSprites[var_r6_2] = ov83_02247454(&arg0->spriteMan, 1, 1, 1, 0, sp18, 0x3E, 2, 0);
            temp_r0_2 = Party_GetMonByIndex(arg0->party, var_r6_2);
            sp1C = temp_r0_2;
            if (GetMonData(temp_r0_2, 6, NULL) == 0) {
                ov83_0224755C(arg0->itemIconSprites[var_r6_2], 0);
            }
            temp_r0_3 = GetMonData(sp1C, 0xA3, NULL);
            sp20 = temp_r0_3;
            sp24 = GetMonData(sp1C, 0xA4, NULL);
            temp_r0_4 = ov83_022411B0(arg0, CalculateHpBarColor((u16)temp_r0_3, (u16)sp24, 0x30U));
            sp28 = temp_r0_4;
            arg0->barSprites[var_r6_2] = ov83_02247454(&arg0->spriteMan, 0, 0, 0, ov83_022411DC(arg0, CalculateHpBarColor((u16)temp_r0_3, (u16)sp24, 0x30U)), var_r7, 0x4E, 3, 0);
            arg0->monIconSprites[var_r6_2] = ov83_02247454(&arg0->spriteMan, var_r6_2 + 0xA, 0xA, 5, temp_r0_4, var_r7, 0x3A, 2, 0);
            ov83_022475EC(arg0->monIconSprites[var_r6_2], Party_GetMonByIndex(arg0->party, var_r6_2));
            var_r6_2++;
            var_r7 += 0x40;
            sp18 += 0x40;
        } while (var_r6_2 < sp14);
    }
    arg0->subScreenSprite = ov83_022474C4(&arg0->spriteMan, 3, 3, 3, 0, 0x10, 0xA0, 0, 0);
    ov83_0224755C(arg0->subScreenSprite, 0);
#if defined(SDK_PORT) || defined(NONMATCHING)
    var_r4_3 = 0;
    do {
        ov83_02242894(var_r4_3, &sp2C[2], (s32 *)sp2C);
        temp_r1 = var_r4_3 + 4;
        arg0->listItemSprites[var_r4_3] = ov83_02247454(&arg0->spriteMan, temp_r1, temp_r1, 4, 0, (s32) * (s16 *)((u8 *)((s32 *)sp2C) + 2), (s32) * (s16 *)((s32 *)sp2C), 0, 0);
        ov83_0224755C(arg0->listItemSprites[var_r4_3], 0);
        var_r4_3 += 1;
    } while (var_r4_3 < 6);
#else
    // DS: the walk over the app base (r6 = app + 4 * i); the indexed form above swaps r4/r6
    var_r4_3 = 0;
    var_r6_3 = (u32 *)arg0;
    do {
        ov83_02242894(var_r4_3, &sp2C[2], (s32 *)sp2C);
        temp_r1 = var_r4_3 + 4;
        *(BattleCastleAppSprite **)((u8 *)var_r6_3 + offsetof(BattleCastleSelfApp, listItemSprites)) = ov83_02247454(&arg0->spriteMan, temp_r1, temp_r1, 4, 0, (s32) * (s16 *)((u8 *)((s32 *)sp2C) + 2), (s32) * (s16 *)((s32 *)sp2C), 0, 0);
        ov83_0224755C(*(BattleCastleAppSprite **)((u8 *)var_r6_3 + offsetof(BattleCastleSelfApp, listItemSprites)), 0);
        var_r4_3 += 1;
        var_r6_3 += 1;
    } while (var_r4_3 < 6);
#endif
    ov83_02240E70(arg0, &sp3C, &sp38, 0);
    arg0->cursorSprite = ov83_02247454(&arg0->spriteMan, 0, 0, 0, 1, sp3C, sp38, 2, 0);
    arg0->partnerCursorSprite = ov83_02247454(&arg0->spriteMan, 0, 0, 0, 2, sp3C, sp38, 2, 0);
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        ov83_0224755C(arg0->partnerCursorSprite, 0);
    }
    arg0->bigSparkles = ov83_02247454(&arg0->spriteMan, 0, 0, 0, 0xB, 0x14, 0x14, 0, 0);
    ov83_0224755C(arg0->bigSparkles, 0);
    arg0->itemSelectCursorSprite = ov83_02247454(&arg0->spriteMan, 0, 0, 0, 3, 0x14, 0x14, 1, 0);
    ov83_0224755C(arg0->itemSelectCursorSprite, 0);
    ov83_02241E18(arg0);
    ov83_02241FF0(arg0);
    ov83_022421E0(arg0, 1);
    arg0->selectedMonSprite = ov83_022474C4(&arg0->spriteMan, 2, 2, 2, 0, 0x30, 0x28, 0, 0);
    ov83_02247668(arg0->selectedMonSprite, arg0->boxMon, arg0->species, arg0->personality);
    arg0->partyGridInput = ov83_02247A24(arg0, 1, arg0->numSlots);
    arg0->touchMenuSpawner = ov83_02247CB8(arg0->spriteMan.spriteList, arg0->plttData);
    ov83_02247844(&arg0->yesNoPrompt);
    if (sub_02037474() != 0) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        sub_0203A880();
    }
    G2_SetBlendAlpha(0, 0xE, 6, 0xA);
    Main_SetVBlankIntrCB(ov83_0223F7A0, arg0);
}
