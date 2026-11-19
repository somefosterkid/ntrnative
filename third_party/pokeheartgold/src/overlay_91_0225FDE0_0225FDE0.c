#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"

extern s32 ov91_02261BEC;

void ov91_0225FDE0(BgConfig **arg0, NARC *arg1, s32 arg2, enum HeapID arg3);

void ov91_0225FDE0(BgConfig **arg0, NARC *arg1, s32 arg2, enum HeapID arg3) {
    s32 temp_r4;
    u32 temp_r4_2;
    GfGfxLoader_LoadCharDataFromOpenNarc(arg1, 0xC, *arg0, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, arg3);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg1, 0xE, *arg0, GF_BG_LYR_SUB_3, 0U, 0U, 0, arg3);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg1, 0xF, *arg0, GF_BG_LYR_SUB_2, 0U, 0U, 0, arg3);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg1, 0x10, *arg0, GF_BG_LYR_SUB_1, 0U, 0U, 0, arg3);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg1, 0x11, *arg0, GF_BG_LYR_SUB_2_F, 0U, (long long)0U, 0, arg3);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg1, 0xD, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x180U, arg3);
    BgTilemapRectChangePalette(*arg0, 7U, 0U, 0U, 0x20U, 0x20U, (u8)((s32)(*((u8 *)(((u8 *)&ov91_02261BEC) + arg2)))));
    temp_r4 = arg2 * 2;
    temp_r4_2 = temp_r4 + 1;
    BgTilemapRectChangePalette(*arg0, 6U, 0U, 0U, 0x20U, 0x20U, (u8)((u8)(temp_r4 + 2)));
    BgTilemapRectChangePalette(*arg0, 5U, 0U, 0U, 0x20U, 0x20U, (s32)temp_r4_2);
    BgTilemapRectChangePalette(*arg0, 4U, 0U, 0U, 0x20U, 0x20U, (u8)((s32)temp_r4_2));
    BgCommitTilemapBufferToVram(*arg0, 4U);
    BgCommitTilemapBufferToVram(*arg0, 5U);
    BgCommitTilemapBufferToVram(*arg0, 6U);
    BgCommitTilemapBufferToVram(*arg0, 7U);
}
