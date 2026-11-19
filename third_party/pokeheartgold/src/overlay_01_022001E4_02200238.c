#include "global.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_022001E4.h"

s32 ov01_021F1758(FieldEffectManager *fieldEffMan, s32, s32, s32, s32, s32, s32 *);
void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);
s32 ov01_021F1908(FieldEffectManager *fieldEffMan, s32, s32);
s32 ov01_021F1930(FieldEffectManager *fieldEffMan, s32, s32, s32);
extern s32 ov01_022092F0;
void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id);
s32 ov01_021F1924(FieldEffectManager *fieldEffMan, s32);
s32 ov01_021F1970(FieldEffectManager *fieldEffMan, s32);

void ov01_02200238(UnkOv01_022001E4 *ctx, int emoteKind) {
    if (emoteKind == 0) {
        ov01_02200208(ctx);
        return;
    }
    ov01_02200220(ctx);
    return;
}

void ov01_0220024C(UnkOv01_022001E4 *ctx, int emoteKind) {
    if (emoteKind == 0) {
        ov01_02200210(ctx);
        return;
    }
    ov01_02200228(ctx);
    return;
}

void ov01_02200260(UnkOv01_022001E4 *ctx) {
    if (ctx->unk_08 == 0) {
        ctx->unk_08 = 1;
        ov01_021F18D4(ctx->fieldEffMan, 1, 0x76);
        ov01_021F1908(ctx->fieldEffMan, 1, 0x8C);
        ov01_021F1930(ctx->fieldEffMan, 1, 0x11, 1);
        ov01_021F1758(ctx->fieldEffMan, 1, 1, 1, 1, 0, &ov01_022092F0);
    }
}

void ov01_022002AC(UnkOv01_022001E4 *ctx) {
    if (ctx->unk_0C == 0) {
        ctx->unk_0C = 1;
        ov01_021F18D4(ctx->fieldEffMan, 0xA, 0x7D);
        ov01_021F1908(ctx->fieldEffMan, 0xA, 0x8C);
        ov01_021F1930(ctx->fieldEffMan, 0xB, 0x18, 1);
        ov01_021F1758(ctx->fieldEffMan, 0xC, 0xA, 0xA, 0xB, 0, &ov01_022092F0);
    }
}

void ov01_022002FC(UnkOv01_022001E4 *ctx) {
    if (ctx->unk_08 == 1) {
        ctx->unk_08 = 0;
        ov01_021F18FC(ctx->fieldEffMan, 1);
        ov01_021F1924(ctx->fieldEffMan, 1);
        ov01_021F1970(ctx->fieldEffMan, 1);
        ov01_021F18C8(ctx->fieldEffMan, 1);
    }
}

void ov01_0220032C(UnkOv01_022001E4 *ctx) {
    if (ctx->unk_0C == 1) {
        ctx->unk_0C = 0;
        ov01_021F18FC(ctx->fieldEffMan, 0xA);
        ov01_021F1924(ctx->fieldEffMan, 0xA);
        ov01_021F1970(ctx->fieldEffMan, 0xB);
        ov01_021F18C8(ctx->fieldEffMan, 0xC);
    }
}
