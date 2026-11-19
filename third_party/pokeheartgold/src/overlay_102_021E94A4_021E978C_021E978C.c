#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "overlay_102_app_021e937c.h"

extern u32 ov102_021EC6A8;

extern u32 ov102_021EC6B8;

extern u32 ov102_021EC6D4;

extern u32 ov102_021EC6F0;

extern u32 ov102_021EC70C;

extern u32 ov102_021EC728;

extern u32 ov102_021EC744;

extern u32 ov102_021EC760;

void ov102_021E978C(UnkStruct_ov102_021E978C *param_1);

void ov102_021E978C(UnkStruct_ov102_021E978C *param_1) {
    s32 iVar1;
    Ov102App_021E937C *iVar2;

    iVar2 = param_1->unk_0;
    reg_GX_POWCNT = reg_GX_POWCNT & ~0x8000;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GfGfx_SetBanks((GraphicsBanks *)&ov102_021EC760);
    SetBothScreensModesAndDisable((GraphicsModes *)&ov102_021EC6A8);
    InitBgFromTemplate(iVar2->unk_20, 0, (BgTemplate *)&ov102_021EC728, 0);
    InitBgFromTemplate(iVar2->unk_20, 1, (BgTemplate *)&ov102_021EC6D4, 0);
    InitBgFromTemplate(iVar2->unk_20, 2, (BgTemplate *)&ov102_021EC6B8, 0);
    InitBgFromTemplate(iVar2->unk_20, 3, (BgTemplate *)&ov102_021EC70C, 0);
    InitBgFromTemplate(iVar2->unk_20, 4, (BgTemplate *)&ov102_021EC744, 0);
    InitBgFromTemplate(iVar2->unk_20, 5, (BgTemplate *)&ov102_021EC6F0, 0);
    iVar1 = 0;
    do {
        BgClearTilemapBufferAndCommit(iVar2->unk_20, (u8)iVar1);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 6);
    FontID_Alloc(FONT_SUBSCREEN, HEAP_ID_35);
    return;
}
