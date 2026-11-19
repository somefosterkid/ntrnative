#include "global.h"

#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"

struct Ov93Ctx;

struct Ov93Node;

typedef struct Ov93Node {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
} Ov93Node;

typedef struct Ov93Ctx {
    u8 padding_000[0xC];
    Ov93Node unk_00C[8];
    Ov93Node unk_10C[8];
    s32 unk_20C;
    s32 unk_210;
    u8 padding_214[0x4];
    s32 unk_218;
    s32 unk_21C;
    u8 padding_220[0x4];
    s32 unk_224;
    s32 unk_228;
    s32 unk_22C;
    fx32 unk_230;
    u8 padding_234[0x4];
    s32 unk_238;
    s32 unk_23C;
    s32 unk_240;
    u8 padding_244[0x4];
    s32 unk_248;
    u8 padding_24C[0x24];
    s32 unk_270;
    u8 unk_274;
} Ov93Ctx;

int ov93_0225F548(Ov93Ctx *ctx, int a1, fx32 a2);

void ov93_0225EB38(Ov93Ctx *ctx);

extern const u8 ov93_02262C04[13][4];

BOOL ov93_0225F44C(Ov93Ctx *ctx);

BOOL ov93_0225F44C(Ov93Ctx *ctx) {
    s32 arg1 = 0;
    fx32 delta;
    s32 v;
    int r;

    if (ctx->unk_238 == 1) {
        s32 prev = ctx->unk_23C;
        s32 cur = ctx->unk_210;

        if (cur < prev) {
            ctx->unk_274 = FALSE;
            return FALSE;
        }

        if (cur == prev) {
            ctx->unk_274 = FALSE;
            return TRUE;
        }

        if (prev != -1 && cur > prev && !IsSEPlaying(0x58F)) {
            PlaySE(0x58F);
            ctx->unk_274 = TRUE;
        }

        v = ctx->unk_210;
        delta = ((v - ctx->unk_22C) << FX32_SHIFT) - (ctx->unk_230 + (ov93_02262C04[ctx->unk_270][3] << FX32_SHIFT));
        ctx->unk_23C = v;
    } else {
        arg1 = 1;
        delta = -ctx->unk_248;
    }

    r = ov93_0225F548(ctx, arg1, delta);
    switch (r) {
    case 0:
        break;
    case 1:
    case 2:
        ov93_0225EB38(ctx);
        break;
    }

    ctx->unk_230 = -(ctx->unk_00C[0].unk_14 + (ctx->unk_00C[0].unk_04 - ctx->unk_00C[0].unk_14) / 2);
    return TRUE;
}
