#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "sprite.h"

Sprite *ov96_021EB5B8(u32);

void ov96_021F4BB4(void *arg0, s32 arg1);

void ov96_021F4990(u32 *param_1, BgConfig *param_2);

void ov96_021F4990(u32 *param_1, BgConfig *param_2) {
    VecFx32 vec = { 0, 0, 0 };
    MtxFx22 mtx;
    f32 scale;

    vec.x = 0x38000;
    vec.y = 0x290000;
    Sprite_SetMatrix(ov96_021EB5B8(param_1[0x31]), &vec);
    BG_LoadScreenTilemapData(param_2, 6, (void *)(param_1[0x28] + 0xc), *(u32 *)(param_1[0x2a] + 8));
    BgTilemapRectChangePalette(param_2, 6, 0, 0, 0x20, 0x18, 3);
    ScheduleBgTilemapBufferTransfer(param_2, 6);
    BG_LoadScreenTilemapData(param_2, 7, (void *)(param_1[0x26] + 0xc), *(u32 *)(param_1[0x26] + 8));
    ScheduleBgTilemapBufferTransfer(param_2, 7);
    scale = 0.025f;
    mtx._00 = (fx32)(4096.0f * scale);
    mtx._01 = 0;
    mtx._10 = 0;
    mtx._11 = mtx._00;
    OS_WaitVBlankIntr();
    SetBgAffine(param_2, 7, &mtx, 0x80, 0x60);
    GfGfx_EngineBTogglePlanes(8, 1);
    param_1[5] = (u32)ov96_021F4BB4;
    param_1[7] = 4;
    *param_1 = 1;
}
