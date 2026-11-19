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

void ov83_02244394(void *arg0);

void ov83_02244408(void *arg0);

void ov83_02244488(void *data);

void ov83_02247844(void *arg0);

u8 ov80_02237B58(u8, s32);

s32 ov83_02244DA0(void *, s32 *, s32 *, u8);

s32 ov83_02244DF4(void *, void *, s32 *, void *, s32 *);

s32 ov83_02245C80(void *);

s32 ov83_02245CE8(void *, s32 *, s32 *);

s32 ov83_02245D48(void *);

struct Ov83App_02246114;

void ov83_02245F24(struct Ov83App_02246114 *);

s32 ov83_02246114(void *, s32);

s32 ov83_02246988(void *);

BattleCastleAppSprite *ov83_02247454(void *, s32, s32, s32, s32, s16, s16, s32, void *);

BattleCastleAppSprite *ov83_022474C4(void *, s32, s32, s32, s32, s16, s16, s32, s32);

s32 ov83_0224755C(void *, s32);

s32 ov83_022475EC(void *, Pokemon *);

s32 ov83_022478D4(BgConfig *, void *, u8);

s32 ov83_02247A7C(void *, u8, s32);

s32 ov83_02247CB8(s32, PaletteData *);

void ov83_02243FD4(void *arg0);

void ov83_02243FD4(void *arg0) {
    s32 sp34;
    s32 sp30;
    u8 sp2C[4];
    u8 sp28[4];
    void *sp24;
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
    void *var_r4_2;
    struct BattleCastleAppSprite **var_r4_3;
    ((BattleCastleOpponentApp *)arg0)->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_107);
    ov83_02244394(arg0);
    ov83_02244408(arg0);
    FontID_Alloc(FONT_UNK_4, HEAP_ID_107);
    *((MsgData **)&((BattleCastleOpponentApp *)arg0)->msgLoader) = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x21, HEAP_ID_107);
    *((MessageFormat **)&((BattleCastleOpponentApp *)arg0)->strTemplate) = MessageFormat_New(HEAP_ID_107);
    ((BattleCastleOpponentApp *)arg0)->displayStr = String_New(0x258U, HEAP_ID_107);
    ((BattleCastleOpponentApp *)arg0)->fmtStr = String_New(0x258U, HEAP_ID_107);
    var_r6 = 0;
    do {
        ((BattleCastleOpponentApp *)arg0)->simpleMenuStr[var_r6] = String_New(0x20U, HEAP_ID_107);
        var_r6 += 1;
    } while (var_r6 < 3);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_107);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_107);
    *((MessagePrinter **)&((BattleCastleOpponentApp *)arg0)->specialChars) = MessagePrinter_New(1U, 2U, 0U, HEAP_ID_107);
    ov83_022478D4(*((void **)&((BattleCastleOpponentApp *)arg0)->bgConfig), arg0 + 0x50, 1);
    ov83_02244DF4(arg0, &sp2C[2], (s32 *)sp2C, &sp28[2], (s32 *)sp28);
    if (ov80_02237D8C(((BattleCastleOpponentApp *)arg0)->challengeType) == 0) {
        var_r7 = 0x40;
        sp1C = 0x3C;
    } else {
        var_r7 = 0x20;
        sp1C = 0x1C;
    }
    temp_r0 = ov80_02237B58(((BattleCastleOpponentApp *)arg0)->challengeType, 1);
    var_r6_2 = 0;
    sp18 = temp_r0;
    if (temp_r0 > 0) {
        sp14 = sp18;
        var_r4_2 = arg0;
        do {
            *((void **)(((u8 *)var_r4_2) + 0x4F4)) = ov83_02247454(arg0 + 0x2C8, 0, 0, 0, 7, sp1C, 0x3E, 2, 0);
            *((void **)(((u8 *)var_r4_2) + 0x510)) = ov83_02247454(arg0 + 0x2C8, 0, 0, 0, 0xF, var_r7, 0x4E, 3, 0);
            *((void **)(((u8 *)var_r4_2) + 0x4E4)) = ov83_02247454(arg0 + 0x2C8, var_r6_2 + 0xA, 0xA, 5, 1, var_r7, 0x3A, 2, 0);
            ov83_022475EC(*((void **)(((u8 *)var_r4_2) + 0x4E4)), Party_GetMonByIndex(((BattleCastleOpponentApp *)arg0)->opponentsParty, var_r6_2));
            if ((*((u8 *)((((BattleCastleOpponentApp *)arg0)->identityUnlocked) + var_r6_2))) == 0) {
                ov83_0224755C(*((void **)(((u8 *)var_r4_2) + 0x4F4)), 1);
                ov83_0224755C(*((void **)(((u8 *)var_r4_2) + 0x4E4)), 0);
                ov83_0224755C(*((void **)(((u8 *)var_r4_2) + 0x510)), 0);
            } else {
                ov83_0224755C(*((void **)(((u8 *)var_r4_2) + 0x4F4)), 0);
                ov83_0224755C(*((void **)(((u8 *)var_r4_2) + 0x4E4)), 1);
                ov83_0224755C(*((void **)(((u8 *)var_r4_2) + 0x510)), 1);
            }
            var_r6_2 += 1;
            sp1C += 0x40;
            var_r4_2 += 4;
            var_r7 += 0x40;
        } while (var_r6_2 < sp14);
    }
    ov83_02244DA0(arg0, &sp34, &sp30, 0);
    *((void **)&((BattleCastleOpponentApp *)arg0)->cursorSprite) = ov83_02247454(arg0 + 0x2C8, 0, 0, 0, 1, sp34, sp30, 2, 0);
    *((void **)&((BattleCastleOpponentApp *)arg0)->partnerCursorSprite) = ov83_02247454(arg0 + 0x2C8, 0, 0, 0, 2, sp34, sp30, 2, 0);
    if (ov80_02237D8C(((BattleCastleOpponentApp *)arg0)->challengeType) == 0) {
        ov83_0224755C(*((void **)&((BattleCastleOpponentApp *)arg0)->partnerCursorSprite), 0);
    }
    sp14 = 0;
    sp20 = 0;
    sp24 = arg0;
    do {
        var_r7_2 = 0;
        var_r4_3 = (struct BattleCastleAppSprite **)sp24;
        var_r6_3 = 0;
loop_15:
        ov83_02245CE8(arg0, &sp34, &sp30);

        *(void **)((u8 *)var_r4_3 + offsetof(BattleCastleOpponentApp, flagSprites)) = ov83_02247454(arg0 + 0x2C8, 0, 0, 0, 0xC, sp34 + sp20, sp30 + var_r6_3, 2, 0);
        ov83_0224755C(*(void **)((u8 *)var_r4_3 + offsetof(BattleCastleOpponentApp, flagSprites)), 0);
        var_r7_2 += 1;
        var_r6_3 += 0xC;
        var_r4_3 += 1;
        if (var_r7_2 < 2) {
            goto loop_15;
        }
        sp20 += 0x40;
        sp24 += 8;
        temp_r0_2 = sp14 + 1;
        sp14 = temp_r0_2;
    } while (temp_r0_2 < 4);
    ov83_02245C80(arg0);
    *((void **)&((BattleCastleOpponentApp *)arg0)->sparklesSprite) = ov83_02247454(arg0 + 0x2C8, 0, 0, 0, 0xB, 0x14, 0x14, 0, 0);
    ov83_0224755C(*((void **)&((BattleCastleOpponentApp *)arg0)->sparklesSprite), 0);
    ov83_02245D48(arg0);
    ov83_02245F24(arg0);
    ov83_02246114(arg0, 1);
    ((BattleCastleOpponentApp *)arg0)->selectedMonSprite = ov83_022474C4(arg0 + 0x2C8, 2, 2, 2, 0, 0x30, 0x28, 0, 0);
    ov83_02246988(arg0);
    *((s32 *)&((BattleCastleOpponentApp *)arg0)->unk_5F0) = ov83_02247A7C(arg0, 1, ((BattleCastleOpponentApp *)arg0)->numSlots);
    *((s32 *)&((BattleCastleOpponentApp *)arg0)->unk_5F4) = ov83_02247CB8(*((s32 *)&((BattleCastleOpponentApp *)arg0)->spriteMan.spriteList), *((void **)&((BattleCastleOpponentApp *)arg0)->unk_2B0));
    ov83_02247844(arg0 + 0x604);
    if (sub_02037474() != 0) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        sub_0203A880();
    }
    G2_SetBlendAlpha(0, 0xE, 6, 0xA);
    Main_SetVBlankIntrCB(ov83_02244488, arg0);
}
