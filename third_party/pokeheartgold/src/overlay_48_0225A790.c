#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "filesystem.h"
#include "font_types_def.h"
#include "text.h"
#include "unk_0200A090.h"
#include "sprite.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"
#include "msgdata.h"

struct MsgData;

struct SpriteList;

struct MessageFormat;

typedef u32 TextColor;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} UnkStruct_ov69_0225ECF0;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_ov69_0225E0C0;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    void *unk_08[2];
    NNSG2dScreenData *unk_10[2];
    u16 unk_18;
    u16 unk_1A;
    void *unk_1C;
    NNSG2dPaletteData *unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
} UnkStruct_ov69_0225E7DC;

typedef struct {
    u8 unk_00;
    s8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov69_0225E084_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    const UnkStruct_ov69_0225ECF0 *unk_0C;
} UnkStruct_ov69_0225ED90;

typedef struct {
    UnkStruct_ov69_0225ED90 unk_00[3];
    s16 unk_30;
    s16 unk_32;
    u8 unk_34[4];
    u8 unk_38[4];
    Sprite *unk_3C[12];
    SpriteResource *unk_6C[12][4];
} UnkStruct_ov69_0225EB60;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225E0C0 unk_04[4];
    u16 unk_34;
    u16 unk_36;
    u32 unk_38;
    u16 unk_3C;
    u16 unk_3E;
    UnkStruct_ov69_0225E084_sub1 unk_40[18];
    u32 unk_88[3];
    void *unk_94;
    NNSG2dScreenData *unk_98;
    u16 unk_9C;
    u16 unk_9E;
    UnkStruct_ov69_0225E7DC unk_A0[5];
    Window unk_168[3];
    Window unk_198;
    Window unk_1A8;
    UnkStruct_ov69_0225EB60 unk_1B8;
    UnkStruct_ov69_0225E478 unk_2E4;
} UnkStruct_ov69_0225E084;

u16 ov48_0225B038(u32 param0);

String *ov48_0225B0C4(UnkStruct_ov69_0225EF54 *param0, u32 param1);

String *ov48_0225B0E0(UnkStruct_ov69_0225EF54 *param0, u32 param1);

String *ov48_0225B108(UnkStruct_ov69_0225EF54 *param0, u32 param1, u32 param2);

extern const TextColor ov48_0225B1D8[5];

void ov48_0225A790(UnkStruct_ov69_0225E084 *param0, u32 param1, const UnkStruct_ov69_0225E0C0 *param2, UnkStruct_ov69_0225EF54 *param3);

s32 ov48_0225A834(u8 *arg0, u32 arg1, s32 arg2);
s32 ov48_0225A858(UnkStruct_ov69_0225E084 *arg0, s32 arg1);

void ov48_0225A790(UnkStruct_ov69_0225E084 *param0, u32 param1, const UnkStruct_ov69_0225E0C0 *param2, UnkStruct_ov69_0225EF54 *param3) {
    String *v0;
    Window *v1;
    u32 v2;

    ((param1 < 3) ? (void)0 : GF_AssertFail());

    v1 = &param0->unk_168[param1];
    FillWindowPixelBuffer(v1, 0);
    v2 = ov48_0225B038(param2->unk_00);

    {
        v0 = ov48_0225B0C4(param3, 2);
        AddTextPrinterParameterizedWithColor(v1, FONT_MESSAGE, v0, 0, 0, 0xFF, ov48_0225B1D8[v2], 0L);
    }

    {
        v0 = ov48_0225B0E0(param3, param2->unk_04);
        AddTextPrinterParameterizedWithColor(v1, FONT_MESSAGE, v0, 0, 16, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    }

    {
        v0 = ov48_0225B108(param3, param2->unk_04, param2->unk_08);
        AddTextPrinterParameterizedWithColor(v1, FONT_MESSAGE, v0, 0, 32, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    }
}

s32 ov48_0225A834(u8 *arg0, u32 arg1, s32 arg2) {
    if (arg1 >= 3U) {
        GF_AssertFail();
    }
    return *(s32 *)((u8 *)(arg0 + (arg1 * 0x10)) + 0x174) + (0x360 * arg2);
}

s32 ov48_0225A858(UnkStruct_ov69_0225E084 *arg0, s32 arg1) {
    return *((s32 *)&arg0->unk_198.pixelBuffer) + (0x360 * arg1);
}
