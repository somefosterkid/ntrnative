#include "global.h"

#include "bg_window.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_102_easy_chat_app.h"

void ov102_021EBB1C(void *arg0);

struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct TouchScreenActions;
struct UnkStruct_ov20_021D2128_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatArgs EasyChatArgs;
typedef struct EasyChatWordList EasyChatWordList;
typedef struct TouchScreenActions TouchScreenActions;
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
typedef struct {
    UnkStruct_ov20_021D2128 *unk_00;
    const EasyChatApp *easyChatApp;
    const EasyChatWordList *unk_08;
    BgConfig *bgConfig;
    u32 unk_10;
    u32 unk_14;
    int dummy;
    UnkStruct_ov20_021D4FF0 unk_1C;
    UnkStruct_ov20_021D4E8C unk_3C;
    Sprite *cursorSprite;
} EasyChatCategoriesContext;
BOOL ov102_021EC450(UnkStruct_ov20_021D4FF0 *param0);
BOOL ov102_021EBB44(EasyChatCategoriesContext *ctx);

void ov102_021EBB1C(void *arg0) {
    BgSetPosTextAndCommit(*(BgConfig **)((u8 *)arg0 + 0xC), 1U, BG_POS_OP_SET_XSCALE, 0x10);
}

s32 ov102_021EC410(void *, s32, s32, s32, s32);

void ov102_021EBB2C(u8 *arg0);

void ov102_021EBB2C(u8 *arg0) {
    ov102_021EC410(arg0 + 0x1C, 2, 0, -0xA, 0x10);
}

BOOL ov102_021EBB44(EasyChatCategoriesContext *ctx) {
    return ov102_021EC450(&ctx->unk_1C);
}
