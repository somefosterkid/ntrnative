#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"

extern s32 ov65_0222010C;

extern s32 ov65_02220144;

void ov65_0221F8D0(BgConfig *bgConfig, Window *windows, Options *options);

void ov65_0221F8D0(BgConfig *bgConfig, Window *windows, Options *options) {
    s32 sp18;
    BgConfig *sp14;
    s32 var_r7;
    s32 var_r6;
    s32 var_r6_2;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 *var_r4;
    s32 *var_r4_2;

    sp14 = bgConfig;
    AddWindowParameterized(bgConfig, windows, 1U, 2U, 1U, 0xAU, 2U, 8U, 1U);
    FillWindowPixelBuffer(windows, 0U);
    AddWindowParameterized(bgConfig, ((void *)windows) + 0x10, 1U, 0x14U, 1U, 0xAU, 2U, 8U, 0x15U);
    FillWindowPixelBuffer(((void *)windows) + 0x10, 0U);
    AddWindowParameterized(bgConfig, ((void *)windows) + 0x60, 1U, 0x1AU, 0x15U, 5U, 2U, 8U, 0x29U);
    FillWindowPixelBuffer(((void *)windows) + 0x60, 0U);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_FIRST, 0x3D9U, 0xAU, (s32)Options_GetFrame(options), HEAP_ID_26);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_FIRST, 0x3F7U, 0xBU, 0U, HEAP_ID_26);
    AddWindowParameterized(bgConfig, ((void *)windows) + 0x150, 0U, 2U, 0x15U, 0x14U, 2U, 0xDU, 1U);
    FillWindowPixelBuffer(((void *)windows) + 0x150, 0U);
    AddWindowParameterized(bgConfig, ((void *)windows) + 0x160, 0U, 2U, 0x15U, 0x1BU, 2U, 0xDU, 0x28U);
    FillWindowPixelBuffer(((void *)windows) + 0x160, 0U);
    AddWindowParameterized(bgConfig, ((void *)windows) + 0x170, 0U, 2U, 0x13U, 0x1BU, 4U, 0xDU, 0x36U);
    FillWindowPixelBuffer(((void *)windows) + 0x170, 0U);
    AddWindowParameterized(bgConfig, ((void *)windows) + 0x190, 0U, 0x14U, 0x13U, 0xBU, 4U, 0xDU, 0xA2U);
    FillWindowPixelBuffer(((void *)windows) + 0x190, 0U);
    var_r4 = &ov65_0222010C;
    var_r7 = 0;
    var_r6 = 0x33;
    do {
        temp_r0 = var_r7 + 7;
        sp18 = temp_r0 * 0x10;
        AddWindowParameterized(sp14, ((void *)windows) + sp18, 1U, *(u16 *)var_r4, (s32) * (u16 *)((u8 *)var_r4 + 2), 8U, 2U, 8U, (s32)var_r6);
        FillWindowPixelBuffer(&windows[temp_r0], 0U);
        var_r6 += 0x10;
        var_r4 = (void *)((u8 *)var_r4 + (4));
        var_r7 += 1;
    } while (var_r7 < 0xE);
    var_r4_2 = &ov65_02220144;
    var_r6_2 = 0;
    do {
        temp_r0_2 = var_r6_2 + 0x1A;
        AddWindowParameterized(sp14, &windows[temp_r0_2], 4U, *(u16 *)var_r4_2, (s32) * (u16 *)((u8 *)var_r4_2 + 2), (s32) * (u16 *)((u8 *)var_r4_2 + 4), (s32) * (u16 *)((u8 *)var_r4_2 + 6), 8U, (s32) * (u16 *)((u8 *)var_r4_2 + 8));
        FillWindowPixelBuffer(&windows[temp_r0_2], 0U);
        var_r6_2 += 1;
        var_r4_2 = (void *)((u8 *)var_r4_2 + (0xA));
    } while (var_r6_2 < 8);
}
