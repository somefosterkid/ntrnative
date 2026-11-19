#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "touch_hitbox_controller.h"

struct EasyChatApp;

struct EasyChatArgs;

struct EasyChatWordList;

struct TouchHitboxController;

struct UnkStruct_ov20_021D2128_t;

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatWordList EasyChatWordList;

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

void ov102_021EB6C8(EasyChatCategoriesContext *ctx);

void ov102_021EB6C8(EasyChatCategoriesContext *ctx) {
    if (ctx->cursorSprite) {
        Sprite_Delete(ctx->cursorSprite);
    }

    Heap_Free(ctx);
}

BgConfig *ov102_021EA268(s32);

s32 ov102_021EB77C(s32 *, BgConfig *, u32);

s32 ov102_021EB880(s32 *, BgConfig *, s32);

s32 ov102_021EB94C(s32 *, BgConfig *, s32);

s32 ov102_021EB950(s32 *);

void ov102_021EB6E0(s32 *arg0, NARC *arg1);

void ov102_021EB6E0(s32 *arg0, NARC *narc) {
    BgConfig *v0;

    v0 = ov102_021EA268(*arg0);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, v0, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, HEAP_ID_35);
    ov102_021EB94C(arg0, v0, ov102_021EB880(arg0, v0, ov102_021EB77C(arg0, v0, GfGfxLoader_LoadCharDataFromOpenNarc(narc, 3, v0, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, HEAP_ID_35) >> 5)));
    BgSetPosTextAndCommit(v0, 1U, BG_POS_OP_SET_ROT, -4);
    BgSetPosTextAndCommit(v0, 1U, BG_POS_OP_SET_XSCALE, 0x10);
    ov102_021EB950(arg0);
    G2_SetBlendBrightness(2, -0xA);
    BgCommitTilemapBufferToVram(v0, 1U);
}
