#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_96_app_021f4724.h"

typedef struct UnkStruct_ov96_021F4700 {
    u8 unk_0[0x1C];
    s32 unk_1C;
} UnkStruct_ov96_021F4700;

s32 ov96_021F4700(UnkStruct_ov96_021F4700 *arg0, s32 arg1);

s32 ov96_021F4700(UnkStruct_ov96_021F4700 *arg0, s32 arg1) {
    s32 v0;

    v0 = arg0->unk_1C;
    if (v0 == arg1) {
        return 0;
    }
    if (arg1 == 1) {
        return 1;
    }
    if ((arg1 == 2) && (v0 != 1)) {
        return 1;
    }
    return 0;
}

s32 ov96_021EB52C(void *, s32, s32);

static void ov96_021F4790(void *arg0);

void ov96_021F4724(void *arg0);

void ov96_021F4724(void *arg0) {
    void *v0;
    s32 v1;
    s32 *v2;

    v1 = 0;
    *(s32 *)arg0 = 0;
    v0 = arg0;
    *(s16 *)((u8 *)arg0 + 0x1A) = 0;
    v2 = (s32 *)arg0;
    do {
        ov96_021EB52C(*(void **)((u8 *)v2 + offsetof(Ov96App_021F4724, unk_90)), 1, 0);
        v1 += 1;
        v2 += 1;
    } while (v1 < 2);
    reg_G2S_DB_BLDCNT = 0;
    ov96_021F4790(v0);
    GfGfx_EngineBTogglePlanes(8U, 0U);
    GfGfx_EngineBTogglePlanes(1U, 0U);
    *(s32 *)((u8 *)v0 + 0x14) = 0;
    *(u8 *)((u8 *)v0 + 0xEA) = 0U;
    reg_G2S_DB_BG2OFS = (*(u8 *)((u8 *)v0 + 0xEA) << 0x10) & 0x01FF0000;
    v0 += 0xEA;
}

static void ov96_021F4790(void *arg0) {
    s32 v0;
    u8 v1;
    u8 v2;

    v1 = 0;
    do {
        ov96_021EB52C(*(void **)((u8 *)(arg0 + (v1 * 4)) + 0xBC), 1, 0);
        v1 += 1;
    } while ((u32)v1 < 2U);
    v2 = 0;
    do {
        v0 = v2 * 4;
        ov96_021EB52C(*(void **)((u8 *)(arg0 + v0) + 0xC8), 1, 0);
        ov96_021EB52C(*(void **)((u8 *)(arg0 + v0) + 0xD8), 1, 0);
        v2 += 1;
    } while ((u32)v2 < 4U);
    ov96_021EB52C(*(void **)((u8 *)arg0 + 0xC4), 1, 0);
}
