#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "easy_chat.h"
#include "filesystem.h"
#include "mail_misc.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "overlay_102_easy_chat_app.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "overlay_102_easy_chat_sentence_view.h"

typedef struct EasyChatWordList EasyChatWordList;

typedef struct ColoredArrow ColoredArrow;

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

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

struct EasyChatWordList {
    const EasyChatArgs *easyChatArgs;
    EasyChatManager *wordLoader;
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

typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);

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

u16 ov102_021E8FD0(void *arg0);

void ov102_021E94A4(void *arg0);



void ov102_021EB664(void *arg0);

void ov102_021EB9C0(EasyChatCategoriesContext *ctx, u32 selectedIndex);

BOOL ov102_021EBA5C(EasyChatCategoriesContext *ctx);





s32 ov102_021EB98C(EasyChatCategoriesContext *, s32);

s32 ov102_021EBA44(EasyChatCategoriesContext *);

void ov102_021E9978(s32 arg0, void *arg1);

void ov102_021E9978(s32 arg0, void *arg1) {
    s32 v0;
    s32 v1;
    s32 v3;
    void *v2;

    v0 = *(s32 *)((u8 *)arg1 + 0x14);
    v2 = *(void **)arg1;
    switch (v0) {
    case 0:
        ov102_021EAF44(*(void **)((u8 *)v2 + 0x1E0));
        ov102_021EAF7C(*(void **)((u8 *)v2 + 0x1E0));
        ov102_021EADF8(*(void **)((u8 *)v2 + 0x1E0), 1);
        ov102_021EAE40(*(void **)((u8 *)v2 + 0x1E0), 0);
        ov102_021EB664(*(void **)((u8 *)v2 + 0x1E4));
        ov102_021EBA44(*(EasyChatCategoriesContext **)((u8 *)v2 + 0x1E8));
        ov102_021EA71C(*(void **)((u8 *)v2 + 0x1E0), 0);
        *(s32 *)((u8 *)arg1 + 0x14) = (s32)(*(s32 *)((u8 *)arg1 + 0x14) + 1);
        return;
    case 1:
        v3 = ov102_021EBA5C(*(EasyChatCategoriesContext **)((u8 *)v2 + 0x1E8));
        v1 = ov102_021EA754(*(void **)((u8 *)v2 + 0x1E0));
        if ((v3 != 0) && (v1 != 0)) {
            ov102_021EB9C0(*(EasyChatCategoriesContext **)((u8 *)v2 + 0x1E8), (u32)ov102_021E8FD0(*(void **)((u8 *)v2 + 0x18)));
            ov102_021EB98C(*(EasyChatCategoriesContext **)((u8 *)v2 + 0x1E8), 1);
            ov102_021E94A4(arg1);
        }
        return;
    }
}

void ov102_021EB624(void *arg0);

s32 ov102_021EB654(void *arg0);

void ov102_021EBBD0(void *arg0);

void ov102_021EBBF4(void *arg0);

s32 ov102_021EBC28(void *);

void ov102_021E9A24(s32 arg0, void *arg1);

void ov102_021E9A24(s32 arg0, void *arg1) {
    s32 v0;
    void *v1;

    v0 = *(s32 *)((u8 *)arg1 + 0x14);
    v1 = *(void **)arg1;
    switch (v0) {
    case 0:
        ov102_021EB624(*(void **)((u8 *)v1 + 0x1E4));
        *(s32 *)((u8 *)arg1 + 0x14) = (s32)(*(s32 *)((u8 *)arg1 + 0x14) + 1);
        return;
    case 1:
        ov102_021EBBF4(*(void **)((u8 *)v1 + 0x1E8));
        *(s32 *)((u8 *)arg1 + 0x14) = (s32)(*(s32 *)((u8 *)arg1 + 0x14) + 1);
        return;
    case 2:
        if ((ov102_021EBC28(*(void **)((u8 *)v1 + 0x1E8)) != 0) && (ov102_021EB654(*(void **)((u8 *)v1 + 0x1E4)) != 0)) {
            ov102_021EBBD0(*(void **)((u8 *)v1 + 0x1E8));
            ov102_021E94A4(arg1);
        }
        return;
    }
}

void ov102_021E9A8C(SysTask *param0, void *param1);

void ov102_021E9A8C(SysTask *param0, void *param1) {
    u32 v0;
    void *v1;

    v0 = *(u32 *)((u8 *)param1 + 0x14);
    v1 = *(void **)param1;
    switch (v0) {
    case 0:
        ov102_021EB624(*(void **)((u8 *)v1 + 0x1E4));
        *(u32 *)((u8 *)param1 + 0x14) = (u32)(*(u32 *)((u8 *)param1 + 0x14) + 1);
        return;
    case 1:
        ov102_021EB98C(*(EasyChatCategoriesContext **)((u8 *)v1 + 0x1E8), 0);
        ov102_021EBBF4(*(EasyChatCategoriesContext **)((u8 *)v1 + 0x1E8));
        *(u32 *)((u8 *)param1 + 0x14) = (u32)(*(u32 *)((u8 *)param1 + 0x14) + 1);
        return;
    case 2:
        if (ov102_021EBC28(*(EasyChatCategoriesContext **)((u8 *)v1 + 0x1E8)) != 0) {
            ov102_021EBBD0(*(EasyChatCategoriesContext **)((u8 *)v1 + 0x1E8));
            *(u32 *)((u8 *)param1 + 0x14) = (u32)(*(u32 *)((u8 *)param1 + 0x14) + 1);
            return;
        }
    default:
        return;
    case 3:
        if (ov102_021EB654(*(void **)((u8 *)v1 + 0x1E4)) != 0) {
            ov102_021EB9C0(*(EasyChatCategoriesContext **)((u8 *)v1 + 0x1E8), (u32)ov102_021E8FD0(*(void **)((u8 *)v1 + 0x18)));
            ov102_021EB98C(*(EasyChatCategoriesContext **)((u8 *)v1 + 0x1E8), 1);
            ov102_021E94A4(param1);
        }
        break;
    }
}


void ov102_021EB67C(void *arg0);

void ov102_021EBAAC(void *arg0);


s32 ov102_021EBAD0(void *);

void ov102_021E9B30(s32 arg0, void *arg1);

void ov102_021E9B30(s32 arg0, void *arg1) {
    s32 v0;
    s32 v1;
    s32 v3;
    void *v2;

    v0 = *(s32 *)((u8 *)arg1 + 0x14);
    v2 = *(void **)arg1;
    switch (v0) {
    case 0:
        ov102_021EB67C(*(void **)((u8 *)v2 + 0x1E4));
        ov102_021EB98C(*(void **)((u8 *)v2 + 0x1E8), 0);
        ov102_021EBAAC(*(void **)((u8 *)v2 + 0x1E8));
        ov102_021EADF8(*(void **)((u8 *)v2 + 0x1E0), 0);
        ov102_021EAE40(*(void **)((u8 *)v2 + 0x1E0), 0);
        ov102_021EA71C(*(void **)((u8 *)v2 + 0x1E0), 1);
        *(s32 *)((u8 *)arg1 + 0x14) = (s32)(*(s32 *)((u8 *)arg1 + 0x14) + 1);
        return;
    case 1:
        v3 = ov102_021EBAD0(*(void **)((u8 *)v2 + 0x1E8));
        v1 = ov102_021EA754(*(void **)((u8 *)v2 + 0x1E0));
        if ((v3 != 0) && (v1 != 0)) {
            ov102_021EAFAC(*(void **)((u8 *)v2 + 0x1E0));
            ov102_021EAF50(*(void **)((u8 *)v2 + 0x1E0));
            *(s8 *)((u8 *)v2 + 0x1F4) = 0;
            ov102_021E94A4(arg1);
        }
        return;
    }
}
