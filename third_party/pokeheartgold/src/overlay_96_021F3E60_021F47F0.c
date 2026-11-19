#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"

s32 ov96_021F4700();

s32 ov96_021F4724(void *);

s32 ov96_021F47F0(void *arg0);

s32 ov96_021F47F0(void *arg0) {
    if (ov96_021F4700() != 0) {
        ov96_021F4724(arg0);
        return 1;
    }
    return 0;
}

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021F4EF8(void *, BgConfig *);

extern s32 ov96_021F4A9C;

void ov96_021F480C(void *arg0, BgConfig *arg1);

void ov96_021F480C(void *arg0, BgConfig *bgConfig) {
    u8 v1;
    u8 v2;
    void *v0;

    GfGfx_EngineBTogglePlanes(1U, 1U);
    v1 = 0;
    do {
        ov96_021EB52C(*(void **)((u8 *)(arg0 + (v1 * 4)) + 0xBC), 1, 1);
        v1 += 1;
    } while ((u32)v1 < 2U);
    v2 = 0;
    do {
        ov96_021EB52C(*(void **)((u8 *)(arg0 + (v2 * 4)) + 0xC8), 1, 1);
        v2 += 1;
    } while ((u32)v2 < 4U);
    v0 = *(void **)((u8 *)arg0 + 0xA4);
    BG_LoadScreenTilemapData(bgConfig, 6U, v0 + 0xC, *(u32 *)((u8 *)v0 + 8));
    ov96_021F4EF8(arg0, bgConfig);
    BgTilemapRectChangePalette(bgConfig, 6U, 0U, 0U, 0x20U, 0x18U, 3U);
    ScheduleBgTilemapBufferTransfer(bgConfig, 6U);
    *(s32 **)((u8 *)arg0 + 0x14) = &ov96_021F4A9C;
    *(s32 *)((u8 *)arg0 + 0x1C) = 1;
    *(s32 *)arg0 = 1;
}

s32 ov96_021F4E5C(void *, s32);

extern s32 ov96_021F4AE0;

void ov96_021F48A8(void *arg0, BgConfig *arg1, s32 arg2);

void ov96_021F48A8(void *arg0, BgConfig *bgConfig, s32 arg2) {
    void *v0;

    ov96_021F4E5C(arg0, arg2);
    v0 = *(void **)((u8 *)arg0 + 0xAC);
    BG_LoadScreenTilemapData(bgConfig, 6U, v0 + 0xC, *(u32 *)((u8 *)v0 + 8));
    BgTilemapRectChangePalette(bgConfig, 6U, 0U, 0U, 0x20U, 0x20U, 3U);
    ScheduleBgTilemapBufferTransfer(bgConfig, 6U);
    *(s32 **)((u8 *)arg0 + 0x14) = &ov96_021F4AE0;
    *(s32 *)((u8 *)arg0 + 0x1C) = 2;
    *(s32 *)arg0 = 1;
}

void ov96_021F4CAC(void *param_1);

void ov96_021F4B34(void *arg0);

void ov96_021F48FC(u32 *param_1, BgConfig *param_2);

void ov96_021F48FC(u32 *param_1, BgConfig *param_2) {
    MtxFx22 MStack_1c;

    ov96_021F4CAC((u8 *)param_1);
    MStack_1c._00 = 0x1000;
    MStack_1c._01 = 0;
    MStack_1c._10 = 0;
    MStack_1c._11 = 0x1000;
    OS_WaitVBlankIntr();
    SetBgAffine(param_2, 7, &MStack_1c, 0x80, 0x60);
    BG_LoadScreenTilemapData(param_2, 6, (void *)(param_1[0x2a] + 0xc), *(u32 *)(param_1[0x2a] + 8));
    BgTilemapRectChangePalette(param_2, 6, 0, 0, 0x20, 0x18, 3);
    ScheduleBgTilemapBufferTransfer(param_2, 6);
    BG_LoadScreenTilemapData(param_2, 7, (void *)(param_1[0x27] + 0xc), *(u32 *)(param_1[0x27] + 8));
    ScheduleBgTilemapBufferTransfer(param_2, 7);
    param_1[5] = (u32)(ov96_021F4B34);
    param_1[7] = 3;
    *param_1 = 1;
    return;
}
