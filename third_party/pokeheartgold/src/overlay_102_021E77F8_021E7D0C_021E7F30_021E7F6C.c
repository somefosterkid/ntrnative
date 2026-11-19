#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "mail_misc.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "system.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "overlay_102_easy_chat_app.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "overlay_102_easy_chat_sentence_view.h"
#include "msgdata.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct EasyChatWordLoader EasyChatWordLoader;

typedef struct EasyChatWordList EasyChatWordList;

typedef struct ColoredArrow ColoredArrow;

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatSentence EasyChatSentence;

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

typedef struct TouchScreenActions TouchScreenActions;

typedef struct UnkStruct_ov20_021D3980_t UnkStruct_ov20_021D3980;

typedef struct UnkStruct_ov20_021D4210_t UnkStruct_ov20_021D4210;

typedef struct UnkStruct_ov20_021D4AD4_t UnkStruct_ov20_021D4AD4;

enum EasyChatGroup {
    GROUP_POKEMON = 0,
    GROUP_POKEMON_2,
    GROUP_MOVE,
    GROUP_MOVE_2,
    GROUP_STATUS,
    GROUP_TRAINER,
    GROUP_PEOPLE,
    GROUP_GREETINGS,
    GROUP_LIFESTYLE,
    GROUP_FEELINGS,
    GROUP_TOUGH_WORDS,
    GROUP_UNION,
    EASY_CHAT_GROUP_COUNT,
    GROUP_MODE_CANCEL_INDEX = EASY_CHAT_GROUP_COUNT,
};

enum ABCModeChars {
    A = 0,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    EXCLAMATION,
    ABC_MODE_CHAR_COUNT,
};

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


enum EasyChatWordTextBank {
    EASY_CHAT_WORD_TEXT_BANK_SPECIES_NAME = 0,
    EASY_CHAT_WORD_TEXT_BANK_MOVE_NAMES_UPPERCASE,
    EASY_CHAT_WORD_TEXT_BANK_POKEMON_TYPE_NAMES,
    EASY_CHAT_WORD_TEXT_BANK_ABILITY_NAMES_UPPERCASE,
    EASY_CHAT_WORD_TEXT_BANK_TRAINER_WORDS,
    EASY_CHAT_WORD_TEXT_BANK_PEOPLE_WORDS,
    EASY_CHAT_WORD_TEXT_BANK_GREETINGS,
    EASY_CHAT_WORD_TEXT_BANK_LIFESTYLE_WORDS,
    EASY_CHAT_WORD_TEXT_BANK_FEELINGS,
    EASY_CHAT_WORD_TEXT_BANK_TOUGH_WORDS,
    EASY_CHAT_WORD_TEXT_BANK_UNION_WORDS,
    EASY_CHAT_WORD_TEXT_BANK_COUNT,
};

struct EasyChatWordLoader {
    enum HeapID heapID;
    MsgData *loaders[EASY_CHAT_WORD_TEXT_BANK_COUNT];
};

struct EasyChatWordList {
    const EasyChatArgs *easyChatArgs;
    EasyChatWordLoader *wordLoader;
    u32 groupWordCounts[EASY_CHAT_GROUP_COUNT];
    u32 groupStartIndices[EASY_CHAT_GROUP_COUNT];
    u16 wordsByGroup[((((((((((496 + 468) + 18) + 124) + 38) + 38) + 107) + 104) + 47) + 32) + 23)];
    u32 letterWordCounts[ABC_MODE_CHAR_COUNT];
    u32 letterStartIndices[ABC_MODE_CHAR_COUNT];
    u16 wordsByLetter[((((((((((496 + 468) + 18) + 124) + 38) + 38) + 107) + 104) + 47) + 32) + 23)];
    u8 addedWordBits[((((((((((((496 + 468) + 18) + 124) + 38) + 38) + 107) + 104) + 47) + 32) + 23) >> 3) + 1)];
};

typedef u32 TextColor;

struct ColoredArrow {
    TextColor color;
    String *string;
};

struct EasyChatSentence {
    u16 type;
    u16 id;
    u16 words[2];
};

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchScreenHitTable;

typedef void (*TouchScreenHitTableAction)(u32, enum TouchScreenButtonState, void *);

struct TouchScreenActions {
    const TouchScreenHitTable *hitTables;
    u32 hitTablesLength;
    TouchScreenHitTableAction action;
    void *hitTableContext;
    TouchScreenHitTableActionStateInternal *actionStates;
};

typedef int (*EasyChatMainFunc)(EasyChatApp *easyChatApp, int *state);

typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);

typedef struct {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_ov20_021D1F34;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    int unk_08;
} UnkStruct_ov20_021D18BC;

typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
} UnkStruct_ov20_021D0F64;

typedef struct {
    BgConfig *unk_00;
    u32 unk_04;
    int unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    u16 unk_18;
    u16 unk_1A;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4E8C;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4FF0;

struct UnkStruct_ov20_021D2128_t {
    SysTask *unk_00;
    SysTask *unk_04;
    SysTask *unk_08[4];
    const EasyChatApp *unk_18;
    const EasyChatWordList *unk_1C;
    BgConfig *unk_20;
    SpriteList *unk_24;
    G2dRenderer unk_28;
    NNSG2dImageProxy unk_1B4[2];
    NNSG2dImagePaletteProxy unk_1FC[2];
    NNSG2dCellDataBank *unk_224[2];
    NNSG2dCellAnimBankData *unk_22C[2];
    void *unk_234[2];
    void *unk_23C[2];
    EasyChatSentenceView *unk_244;
    UnkStruct_ov20_021D3980 *unk_248;
    EasyChatCategoriesContext *unk_24C;
    UnkStruct_ov20_021D4210 *unk_250;
    UnkStruct_ov20_021D4AD4 *unk_254;
};

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov20_021D34CC;

typedef struct {
    String *unk_00;
    const u16 *unk_04;
    int unk_08;
} UnkStruct_ov20_021D33C8;

struct UnkStruct_ov20_021D3980_t {
    UnkStruct_ov20_021D2128 *unk_00;
    const EasyChatApp *unk_04;
    const EasyChatWordList *unk_08;
    Sprite *unk_0C;
};

struct UnkStruct_ov20_021D4210_t {
    UnkStruct_ov20_021D2128 *unk_00;
    const EasyChatApp *unk_04;
    const EasyChatWordList *unk_08;
    BgConfig *unk_0C;
    Window unk_10;
    Window unk_20;
    Sprite *unk_30;
    Sprite *unk_34;
    Sprite *unk_38;
    String *unk_3C;
    UnkStruct_ov20_021D4FF0 unk_40;
    UnkStruct_ov20_021D4E8C unk_60;
    u32 unk_80;
    GXWndPlane unk_84;
    int unk_88;
    int unk_8C;
    int unk_90;
};

void ov102_021E940C(UnkStruct_ov20_021D2128 *param0, int param1);

s32 ov102_021E79DC(void *, void *, u32);

s32 ov102_021E7EEC(void *, s32, s32);

s32 ov102_021E7F30(void *, s32, s32);

Sprite *ov102_021EA228(UnkStruct_ov20_021D2128 *, s32, s32, s32);

s32 ov102_021EA238(UnkStruct_ov20_021D2128 *);

s32 ov102_021EA248(UnkStruct_ov20_021D2128 *, TouchscreenHitbox *, s32);

extern TouchscreenHitbox ov102_021EC634;

s32 ov102_021E7F6C(void *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov102_021E7F6C(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    TouchscreenHitbox sp4;
    s16 sp0[2];
    Sprite *temp_r0;
    s16 var_r4_2;
    s32 temp_r0_2;
    s32 var_r0;
    s32 var_r0_2;
    u32 temp_r4;
    u32 var_r4;

    temp_r0 = ov102_021EA228(((UnkStruct_ov20_021D2128 *)((EasyChatApp *)arg0)->unk_14), arg1, arg2, arg3);
    Sprite_GetPositionXY(temp_r0, &sp0[1], &sp0[0]);
    if (((EasyChatApp *)arg0)->unk_66.unk_00 != 0) {
        if (ov102_021E7F30(arg0, *(s16 *)((u8 *)(&sp0) + 2), *(s16 *)(&sp0)) == 0) {
            ((EasyChatApp *)arg0)->unk_66.unk_00 = 0U;
            return -1;
        }
        var_r4_2 = (s16)gSystem.touchX;
        if ((s32)var_r4_2 < 0x1C) {
            var_r4_2 = 0x1C;
        } else if ((s32)var_r4_2 > 0xE2) {
            var_r4_2 = 0xE2;
        }
        var_r0_2 = var_r4_2 - *((s16 *)&((EasyChatApp *)arg0)->pad_6E[0]);
        if (var_r0_2 < 0) {
            var_r0_2 = 0 - var_r0_2;
        }
        if (var_r0_2 < 2) {
            return -1;
        }
        Sprite_SetPositionXY(temp_r0, var_r4_2, *(s16 *)(&sp0));
        *((s16 *)&((EasyChatApp *)arg0)->pad_6E[0]) = var_r4_2;
        temp_r4 = var_r4_2 - 0x1C;
        *((s16 *)&((EasyChatApp *)arg0)->currentWordSlot) = 0;
        ov102_021E79DC(arg0 + 0x64, arg0 + 8, (u32)((temp_r4 + (temp_r4 >> 0x1F)) << 0x17) >> 0x18);
        PlaySE(0x5DCU);
        ov102_021E940C(((UnkStruct_ov20_021D2128 *)((EasyChatApp *)arg0)->unk_14), 3);
        return -1;
    }
    if (System_GetTouchNew() == 0) {
        return -1;
    }
    if (ov102_021E7EEC(arg0, *(s16 *)((u8 *)(&sp0) + 2), *(s16 *)(&sp0)) != 0) {
        *((s16 *)&((EasyChatApp *)arg0)->pad_6E[0]) = (s16)gSystem.touchX;
        ((EasyChatApp *)arg0)->unk_66.unk_00 = 1U;
        PlaySE(0x5DCU);
        return 6;
    }
    var_r0 = TouchscreenHitbox_FindRectAtTouchNew(&ov102_021EC634);
    if (var_r0 == -1) {
        temp_r0_2 = ov102_021EA238(((UnkStruct_ov20_021D2128 *)((EasyChatApp *)arg0)->unk_14));
        var_r4 = 0;
        if (temp_r0_2 > 0) {
loop_19:
            ov102_021EA248(((UnkStruct_ov20_021D2128 *)((EasyChatApp *)arg0)->unk_14), &sp4, (u8)var_r4);
            if (TouchscreenHitbox_PointIsIn(&sp4, (u32)gSystem.touchX, (u32)gSystem.touchY) != 0) {
                return var_r4 + 4;
            }
            var_r4 += 1;
            if ((s32)var_r4 >= temp_r0_2) {
                goto block_22;
            }
            goto loop_19;
        }
block_22:
        var_r0 = -1;
        return var_r0;
    }
    return var_r0;
}
