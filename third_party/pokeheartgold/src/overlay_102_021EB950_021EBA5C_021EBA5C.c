#include "global.h"

#include "bg_window.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_102_easy_chat_app.h"

struct EasyChatApp;

struct EasyChatArgs;

struct EasyChatWordList;

struct TouchScreenActions;

struct UnkStruct_ov20_021D2128_t;

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatArgs EasyChatArgs;

typedef struct EasyChatWordList EasyChatWordList;

typedef struct TouchScreenActions TouchScreenActions;

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

void ov102_021EC298(UnkStruct_ov20_021D4E8C *param0, BgConfig *param1, u32 param2, int param3, int param4, int param5);

BOOL ov102_021EC2EC(UnkStruct_ov20_021D4E8C *param0);

BOOL ov102_021EC450(UnkStruct_ov20_021D4FF0 *param0);

BOOL ov102_021EBA5C(EasyChatCategoriesContext *ctx);

BOOL ov102_021EBA5C(EasyChatCategoriesContext *ctx) {
    switch (ctx->unk_14) {
    case 0:
        if (ov102_021EC450(&ctx->unk_1C)) {
            ov102_021EC298(&(ctx->unk_3C), ctx->bgConfig, 1, 1, 0, 6);
            ctx->unk_14++;
        }
        break;
    case 1:
        return ov102_021EC2EC(&(ctx->unk_3C));
    }

    return 0;
}
