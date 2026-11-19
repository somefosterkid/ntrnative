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

void ov93_0225F268(fx32 param0, fx32 param1, fx16 *param2, fx16 *param3);

void ov93_0225EFAC(Ov93Ctx *ctx);

void ov93_0225EFAC(Ov93Ctx *ctx) {
    s16 x;
    s16 y;
    s32 i;

    for (i = 0; i < 8; i++) {
        u32 begin = GX_BEGIN_QUADS;

        NNS_G3dGeBufferOP_N(0x40, &begin, 1);
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            ov93_0225F268(ctx->unk_00C[i].unk_00, ctx->unk_00C[i].unk_04, &x, &y);
            texcoord = ((u16)(fx16)((ctx->unk_10C[i].unk_00) >> (12 - 4)) | ((u32)(u16)(fx16)((ctx->unk_10C[i].unk_04) >> (12 - 4)) << 16));
            NNS_G3dGeBufferOP_N(0x22, &texcoord, 1);
            normal = 0x1FF00000;
            NNS_G3dGeBufferOP_N(0x21, &normal, 1);
            vtx[0] = ((u32)(u16)(x) | ((u32)(u16)(y) << 16));
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(0x23, vtx, 2);
        }
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            ov93_0225F268(ctx->unk_00C[i].unk_08, ctx->unk_00C[i].unk_0C, &x, &y);
            texcoord = ((u16)(fx16)((ctx->unk_10C[i].unk_08) >> (12 - 4)) | ((u32)(u16)(fx16)((ctx->unk_10C[i].unk_0C) >> (12 - 4)) << 16));
            NNS_G3dGeBufferOP_N(0x22, &texcoord, 1);
            normal = 0x1FF00000;
            NNS_G3dGeBufferOP_N(0x21, &normal, 1);
            vtx[0] = ((u32)(u16)(x) | ((u32)(u16)(y) << 16));
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(0x23, vtx, 2);
        }
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            ov93_0225F268(ctx->unk_00C[i].unk_18, ctx->unk_00C[i].unk_1C, &x, &y);
            texcoord = ((u16)(fx16)((ctx->unk_10C[i].unk_18) >> (12 - 4)) | ((u32)(u16)(fx16)((ctx->unk_10C[i].unk_1C) >> (12 - 4)) << 16));
            NNS_G3dGeBufferOP_N(0x22, &texcoord, 1);
            normal = 0x1FF00000;
            NNS_G3dGeBufferOP_N(0x21, &normal, 1);
            vtx[0] = ((u32)(u16)(x) | ((u32)(u16)(y) << 16));
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(0x23, vtx, 2);
        }
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            ov93_0225F268(ctx->unk_00C[i].unk_10, ctx->unk_00C[i].unk_14, &x, &y);
            texcoord = ((u16)(fx16)((ctx->unk_10C[i].unk_10) >> (12 - 4)) | ((u32)(u16)(fx16)((ctx->unk_10C[i].unk_14) >> (12 - 4)) << 16));
            NNS_G3dGeBufferOP_N(0x22, &texcoord, 1);
            normal = 0x1FF00000;
            NNS_G3dGeBufferOP_N(0x21, &normal, 1);
            vtx[0] = ((u32)(u16)(x) | ((u32)(u16)(y) << 16));
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(0x23, vtx, 2);
        }
        NNS_G3dGeBufferOP_N(0x41, 0L, 0);
    }
}
