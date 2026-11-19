#include "global.h"

#include "overlay_93_thumb_1.h"
#include "unk_02005D10.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov93_0225D700(void *, s32);

struct UnkStruct_ov117_022626B0_t;
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);
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
int ov93_0225F9AC(UnkStruct_ov117_022626B0 *param0);
void ov93_0225EB38(UnkStruct_ov117_022626B0 *param0);
void ov93_0225EFAC(UnkStruct_ov117_022626B0 *param0);
void ov93_0225EE4C(BOOL param0, BOOL param1);
void ov93_0225EE98(void);
void ov93_0225EB70(UnkStruct_ov117_022626B0 *param0);

void ov93_0225EAE0(UnkStruct_ov117_02261280_t *arg0, void *arg1, s32 arg2, s32 arg3) {
    s32 v0;
    s32 v1;

    if ((arg3 == 1) && ((s32) * (s32 *)((u8 *)arg1 + 0x244) >= 0x62)) {
        ov93_0225D700(arg0, 0);
        PlaySE(0x592U);
        return;
    }
    if ((s32) * (s32 *)((u8 *)arg1 + 0x244) < 0x62) {
        v1 = *(s32 *)((u8 *)arg1 + 0x20);
        v0 = *(s32 *)((u8 *)arg1 + 0x10);
        if (v0 < v1) {
            ov93_0225D700(arg0, 2);
            PlaySE(0x590U);
            return;
        }
        if (v0 > v1) {
            ov93_0225D700(arg0, 1);
            PlaySE(0x590U);
        }
    }
}

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

void ov93_0225EB38(UnkStruct_ov117_022626B0 *param0) {
    if (param0->unk_238 == 0) {
        return;
    }

    param0->unk_238 = 0;
    param0->unk_23C = -1;

    {
        int v0;

        v0 = ov93_0225F9AC(param0);
        param0->unk_244 = param0->unk_240 - v0;

        if (param0->unk_244 < 25) {
            param0->unk_244 = 0;
        }
    }
}

void ov93_0225EB70(UnkStruct_ov117_022626B0 *param0) {
    u32 v0, v1;

    NNS_G3dGeLightVector(GX_LIGHTID_0, FX16_SQRT1_3, -FX16_SQRT1_3, -FX16_SQRT1_3);
    NNS_G3dGeLightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
    NNS_G3dGePushMtx();

    {
        fx16 v2 = FX_SinIdx(param0->unk_08);
        fx16 v3 = FX_CosIdx(param0->unk_08);

        NNS_G3dGeTranslate(0, (9 + 10) * FX32_ONE, -5 * FX32_ONE);
        NNS_G3dGeScale(param0->unk_260 + param0->unk_260 * (param0->unk_268 + param0->unk_26C) / 100, param0->unk_264 + param0->unk_264 * param0->unk_26A / 100, (FX32_ONE));
    }

    {
        NNS_G3dGeMtxMode(GX_MTXMODE_TEXTURE);
        NNS_G3dGeIdentity();
        NNS_G3dGeMtxMode(GX_MTXMODE_POSITION_VECTOR);
    }

    ov93_0225EE4C(1, 1);
    ov93_0225EE98();

    v0 = NNS_GfdGetTexKeyAddr(param0->unk_04->texInfo.vramKey);
    v1 = NNS_GfdGetPlttKeyAddr(param0->unk_04->plttInfo.vramKey);

    NNS_G3dGeTexImageParam(GX_TEXFMT_PLTT256, GX_TEXGEN_TEXCOORD, GX_TEXSIZE_S128, GX_TEXSIZE_T128, GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_TRNS, v0);
    NNS_G3dGeTexPlttBase(v1, GX_TEXFMT_PLTT256);
    NNS_G3dGePolygonAttr(GX_LIGHTMASK_0, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, GX_POLYGON_ATTR_MISC_NONE);

    ov93_0225EFAC(param0);

    NNS_G3dGePopMtx(1);
    NNS_G3dGeFlushBuffer();
}
