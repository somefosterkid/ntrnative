#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_40.h"
#include "palette.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222DB30(void *, s32);

void ov40_0222C4F8(UnkStruct_0208C06C *arg0) {
    NARC *narc;
    BgConfig *bgConfig;
    PaletteData *data;
    narc = arg0->unk_14.unk_00;
    bgConfig = arg0->unk_14.unk_10;
    data = ((PaletteData *)arg0->unk_14.unk_14);
    if ((arg0->unk_00) == 0) {
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x43, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x43, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x45, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_109);
        PaletteData_LoadNarc(data, NARC_a_1_9_1, ov40_0222DB30(arg0, 3), HEAP_ID_109, PLTTBUF_MAIN_BG, 0x120U, 0U);
        PaletteData_LoadNarc(data, NARC_a_1_9_1, ov40_0222DB30(arg0, 3), HEAP_ID_109, PLTTBUF_SUB_BG, 0x120U, 0U);
        PaletteData_LoadNarc(data, NARC_a_1_9_1, ov40_0222DB30(arg0, 0), HEAP_ID_109, PLTTBUF_MAIN_BG, 0x20U, 0xE0U);
        PaletteData_LoadNarc(data, NARC_a_1_9_1, ov40_0222DB30(arg0, 0), HEAP_ID_109, PLTTBUF_SUB_BG, 0x20U, 0xE0U);
    } else {
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x36, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x36, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x37, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_109);
        PaletteData_LoadNarc(data, NARC_a_1_9_1, 0x35, HEAP_ID_109, PLTTBUF_MAIN_BG, 0x120U, 0U);
        PaletteData_LoadNarc(data, NARC_a_1_9_1, 0x35, HEAP_ID_109, PLTTBUF_SUB_BG, 0x120U, 0U);
        PaletteData_LoadNarc(data, NARC_a_1_9_1, 0x34, HEAP_ID_109, PLTTBUF_MAIN_BG, 0x20U, 0xE0U);
        PaletteData_LoadNarc(data, NARC_a_1_9_1, 0x34, HEAP_ID_109, PLTTBUF_SUB_BG, 0x20U, 0xE0U);
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x46, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_109);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x46, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, HEAP_ID_109);
    PaletteData_BlendPalettes(data, PLTTBUF_SUB_BG, 2U, 0x10U, (u16)((u16)(arg0->unk_14.unk_44)));
}
