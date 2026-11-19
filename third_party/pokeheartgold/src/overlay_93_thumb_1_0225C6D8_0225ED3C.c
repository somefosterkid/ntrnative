#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_93_thumb_1.h"
#include "palette.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct UnkStruct_ov117_022626B0_t;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
} UnkStruct_ov117_022626B0_sub1_sub1;

typedef struct {
    UnkStruct_ov117_022626B0_sub1_sub1 unk_00;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_08;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_10;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_18;
} UnkStruct_ov117_022626B0_sub1;

typedef struct {
    UnkStruct_ov117_022626B0_sub1_sub1 unk_00;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_08;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_10;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_18;
} UnkStruct_ov117_022626B0_sub2;

struct UnkStruct_ov117_022626B0_t {
    NNSG3dResFileHeader *unk_00;
    NNSG3dResTex *unk_04;
    u16 unk_08;
    UnkStruct_ov117_022626B0_sub1 unk_0C[8];
    UnkStruct_ov117_022626B0_sub2 unk_10C[8];
    int unk_20C;
    int unk_210;
    int unk_214;
    int unk_218;
    int unk_21C;
    int unk_220;
    int unk_224;
    int unk_228;
    int unk_22C;
    fx32 unk_230;
    ManagedSprite *unk_234;
    int unk_238;
    int unk_23C;
    int unk_240;
    int unk_244;
    fx32 unk_248;
    u16 *unk_24C;
    u16 *unk_250;
    u32 unk_254;
    u32 unk_258;
    s32 unk_25C;
    fx32 unk_260;
    fx32 unk_264;
    s16 unk_268;
    s16 unk_26A;
    s16 unk_26C;
    int unk_270;
    u8 unk_274;
    u8 unk_275;
    u8 unk_276;
};

void ov93_0225ED3C(UnkStruct_ov117_022626B0 *param0);

void ov93_0225ED3C(UnkStruct_ov117_022626B0 *param0) {
    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(param0->unk_250, param0->unk_258, param0->unk_254);
    GX_EndLoadTexPltt();
}

static void ov93_0225ED60(UnkStruct_ov117_022626B0 *param0, int param1);

static void ov93_0225ED60(UnkStruct_ov117_022626B0 *param0, int param1) {
    int v0 = (12 << 8) * param1 / 800;
    v0 = (v0 + 0x80) >> 8;

    if (v0 > 16) {
        v0 = 16;
    }

    BlendPalette(param0->unk_24C, param0->unk_250, param0->unk_254 / 2, v0, 0x1f);
    DC_FlushRange(param0->unk_250, param0->unk_254);

    param0->unk_276 = 1;
}

BOOL ov93_0225EDB8(UnkStruct_ov117_022626B0 *param0, int param1);

BOOL ov93_0225EDB8(UnkStruct_ov117_022626B0 *param0, int param1) {
    BOOL v0 = 0;

    param0->unk_25C += param1;

    if (param0->unk_25C >= 800) {
        param0->unk_25C = 800;
        v0 = 1;
    }

    ov93_0225ED60(param0, param0->unk_25C);

    return v0;
}

void ov93_0225EDE8(UnkStruct_ov117_022626B0 *param0);

void ov93_0225EDE8(UnkStruct_ov117_022626B0 *param0) {
    param0->unk_25C = 0;
    ov93_0225ED60(param0, param0->unk_25C);
}

void ov93_0225EDFC(UnkStruct_ov117_022626B0 *param0);

void ov93_0225EDFC(UnkStruct_ov117_022626B0 *param0) {
    if (param0->unk_268 == 0) {
        return;
    }

    param0->unk_275++;

    if (param0->unk_275 == 4) {
        ov93_0225ED60(param0, param0->unk_25C / 2);
    } else if (param0->unk_275 == 4 * 2) {
        ov93_0225ED60(param0, param0->unk_25C);
        param0->unk_275 = 0;
    }
}

void ov93_0225EE4C(s32 arg0, s32 arg1);

void ov93_0225EE4C(s32 arg0, s32 arg1) {
    u32 v6;
    u32 args;
    s32 v7;
    s32 v8;

    if (arg0 != 0) {
        v7 = 1;
    } else {
        v7 = 0;
    }
    v6 = 0x21085EF7 | (v7 << 0xF);
    NNS_G3dGeBufferOP_N(0x30U, &v6, 1U);
    if (arg1 != 0) {
        v8 = 1;
    } else {
        v8 = 0;
    }
    args = 0x7FFF | (v8 << 0xF);
    NNS_G3dGeBufferOP_N(0x31U, &args, 1U);
}
