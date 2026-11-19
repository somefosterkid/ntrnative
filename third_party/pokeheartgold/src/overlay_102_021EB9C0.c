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

enum EasyChatMode {
    GROUP_MODE = 0,
    ABC_MODE,
};

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

u8 ov102_021E8F7C(const EasyChatApp *easyChatApp);

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

void ov102_021EC4CC(u32 index, u32 *outX, u32 *outY);

void ov102_021EB9C0(EasyChatCategoriesContext *ctx, u32 selectedIndex);

void ov102_021EC410(UnkStruct_ov20_021D4FF0 *param0, int param1, int param2, int param3, int param4);
void ov102_021EBA44(EasyChatCategoriesContext *ctx);

void ov102_021EB9C0(EasyChatCategoriesContext *ctx, u32 selectedIndex) {
    u32 mode = ov102_021E8F7C(ctx->easyChatApp);

    VecFx32 position;
    position.z = 0;

    u32 animID;

    if (selectedIndex == 0xFE) {
        position.x = 224;
        position.y = 176;
        animID = 16;
    } else {
        if (mode == GROUP_MODE) {
            position.x = 48 + 80 * (selectedIndex % 3);
            position.y = 64 + 24 * (selectedIndex / 3);
            animID = 4;
        } else {
            u32 x, y;
            ov102_021EC4CC(selectedIndex, &x, &y);

            position.x = 26 + x;
            position.y = 72 + y;
            animID = 6;
        }
    }

    position.x *= ((fx32)0x0000000000001000L);
    position.y *= ((fx32)0x0000000000001000L);

    Sprite_SetMatrix(ctx->cursorSprite, &position);
    Sprite_SetAnimCtrlSeq(ctx->cursorSprite, animID);
}

void ov102_021EBA44(EasyChatCategoriesContext *ctx) {
    ctx->unk_14 = 0;
    ov102_021EC410(&(ctx->unk_1C), GX_BLEND_PLANEMASK_BG1, -10, 0, 16);
}
