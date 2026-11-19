#include "global.h"

#include "overlay_102_app_021e937c.h"

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

u16 ov102_021E8FC0(void *arg0);

u16 ov102_021E8FC8(void *arg0);

void ov102_021E94A4(void *arg0);

void ov102_021EB530(UnkStruct_ov20_021D3980 *param0, int param1);



#if defined(SDK_PORT) || defined(NONMATCHING)
void ov102_021EB524(void *cursor, BOOL draw);
#else
void ov102_021EB524();
#endif

void ov102_021E9F38(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021E9F38(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    u16 uVar1;
    Ov102App_021E937C *iVar2;

    iVar2 = param_2->unk_0;
    uVar1 = ov102_021E8FC8(iVar2->unk_18);
    if ((uVar1 & 0xff) == 0) {
        iVar2->unk_1F4 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
        ov102_021EB524(iVar2->unk_1E4, FALSE);
#else
        ov102_021EB524(iVar2->unk_1E4);
#endif
        ov102_021EAF5C(iVar2->unk_1E0, 1);
        uVar1 = ov102_021E8FC0(iVar2->unk_18);
        ov102_021EAFF0(iVar2->unk_1E0, uVar1);
    } else {
        iVar2->unk_1F4 = 1;
        ov102_021EAF5C(iVar2->unk_1E0, 0);
#if defined(SDK_PORT) || defined(NONMATCHING)
        ov102_021EB524(iVar2->unk_1E4, TRUE);
#else
        ov102_021EB524(iVar2->unk_1E4, 1);
#endif
        ov102_021EB530(iVar2->unk_1E4, uVar1 & 0xff);
    }
    ov102_021E94A4(param_2);
    return;
}
