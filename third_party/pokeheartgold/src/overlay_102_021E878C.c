#include "global.h"
#include "constants/heap.h"
#include "overlay_42.h"
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
#include "overlay_49_unk_struct_ov70_02263344_t.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "overlay_102_easy_chat_sentence_view.h"

s32 ov102_021E878C(void *arg0);

struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_02263344_t;
typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;
typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);
typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;
typedef struct {
    const UnkStruct_ov70_0226DA18 *unk_00;
    void *unk_04;
    void *unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0226308C;
void *ov102_021E87A8(UnkStruct_ov70_02263344 *param0);
u16 ov102_021E87AC(const UnkStruct_ov42_02227F68 *param0);
u16 ov102_021E87B0(const UnkStruct_ov42_02227F68 *param0);

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
void ov102_021E940C(UnkStruct_ov20_021D2128 *param0, int param1);
s32 ov102_021E8B18(void *, u32 *);
s32 ov102_021E9464(UnkStruct_ov20_021D2128 *);
s32 ov102_021E947C(UnkStruct_ov20_021D2128 *, s32);
s32 ov102_021E87B4(EasyChatApp *arg0, u32 *arg1);

s32 ov102_021E878C(void *arg0) {
    return *(u8 *)((u8 *)arg0 + 6) + (*(u8 *)((u8 *)arg0 + 7) * 2);
}

s32 ov102_021E8798(u16 *arg0);

s32 ov102_021E8798(u16 *arg0) {
    return (*arg0 * 2) + ov102_021E878C(arg0);
}

void *ov102_021E87A8(UnkStruct_ov70_02263344 *param0) {
    return param0->unk_04.unk_04;
}

u16 ov102_021E87AC(const UnkStruct_ov42_02227F68 *param0) {
    return param0->unk0;
}

u16 ov102_021E87B0(const UnkStruct_ov42_02227F68 *param0) {
    return param0->unk2;
}

s32 ov102_021E87B4(EasyChatApp *arg0, u32 *arg1) {
    u32 v0;

    v0 = *arg1;
    switch (v0) {
    case 0:
        ov102_021E8B18(arg0, arg1);
        break;
    case 1:
        if (ov102_021E947C(arg0->unk_14, 0x13) != 0) {
            *arg1 = 0;
        }
        break;
    case 2:
        if (ov102_021E947C(arg0->unk_14, 0x13) != 0) {
            ov102_021E940C(arg0->unk_14, 0x12);
            *arg1 = 0;
        }
        break;
    case 3:
        if (ov102_021E9464(arg0->unk_14) != 0) {
            ov102_021E7A4C(arg0, arg0->nextMainFunc);
            *arg1 = 0;
        }
        break;
    }
    return 0;
}
