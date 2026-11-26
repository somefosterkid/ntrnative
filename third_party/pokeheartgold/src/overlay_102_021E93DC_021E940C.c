#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "easy_chat.h"
#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "mail_misc.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "overlay_102_easy_chat_app.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "overlay_102_easy_chat_sentence_view.h"
#include "overlay_102_app_021e937c.h"

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
    EasyChatApp *unk_18;
    EasyChatWordList *unk_1C;
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

extern u32 ov102_021EC788;

void ov102_021E940C(UnkStruct_ov20_021D2128 *param0, s32 param1);

void ov102_021E940C(UnkStruct_ov20_021D2128 *param0, s32 param1) {
    UnkStruct_ov102_021E978C *data;
    SysTaskFunc func;
    SysTask *pSVar1;
    UnkStruct_ov20_021D2128 *pUVar2;
    s32 iVar3;

    if ((u32)param1 < 0x20) {
        data = Heap_Alloc(HEAP_ID_35, sizeof(UnkStruct_ov102_021E978C));
        if (data != NULL) {
            iVar3 = 0;
            data->unk_14 = 0;
            data->unk_C = param1;
            data->unk_0 = param0;
            data->unk_4 = param0->unk_18;
            data->unk_8 = param0->unk_1C;
            pUVar2 = param0;
            do {
                if (pUVar2->unk_08[0] == NULL) {
                    func = *(SysTaskFunc *)(&ov102_021EC788 + param1);
                    data->unk_10 = iVar3;
                    pSVar1 = SysTask_CreateOnMainQueue(func, data, 1);
                    param0->unk_08[iVar3] = pSVar1;
                    return;
                }
                iVar3 = iVar3 + 1;
                pUVar2 = (UnkStruct_ov20_021D2128 *)&pUVar2->unk_04;
            } while (iVar3 < 4);
            return;
        }
    } else {
        GF_AssertFail();
    }
    return;
}
