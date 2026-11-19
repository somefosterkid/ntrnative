#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "options.h"
#include "overlay_75.h"
#include "palette.h"
#include "render_window.h"
#include "sys_task_api.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_75_unk_struct_ov98_02247704.h"

void ov75_02247450(UnkStruct_ov98_02247704 *param0);

void ov75_02247450(UnkStruct_ov98_02247704 *param0) {
    BgConfig *v0 = param0->unk_04;
    NARC *v1 = NARC_New(NARC_a_0_8_8, HEAP_ID_116);

    GfGfxLoader_GXLoadPalFromOpenNarc(v1, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_116);
    GfGfxLoader_GXLoadPalFromOpenNarc(v1, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_116);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_116);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_116);
    LoadUserFrameGfx2(v0, GF_BG_LYR_MAIN_0, 1, 10, Options_GetFrame(param0->unk_00->options), HEAP_ID_116);
    LoadUserFrameGfx1(v0, GF_BG_LYR_MAIN_0, 1 + (18 + 12), 11, 0, HEAP_ID_116);
    LoadUserFrameGfx1(v0, GF_BG_LYR_MAIN_2, 1 + (18 + 12), 11, 0, HEAP_ID_116);
    GfGfxLoader_LoadCharDataFromOpenNarc(v1, 2, v0, GF_BG_LYR_MAIN_1, 0, 0, 0, HEAP_ID_116);
    GfGfxLoader_LoadScrnDataFromOpenNarc(v1, 6, v0, GF_BG_LYR_MAIN_1, 0, 32 * 24 * 2, 0, HEAP_ID_116);
    GfGfxLoader_LoadCharDataFromOpenNarc(v1, 11, v0, GF_BG_LYR_SUB_1, 0, 0, 0, HEAP_ID_116);
    GfGfxLoader_LoadScrnDataFromOpenNarc(v1, 12, v0, GF_BG_LYR_SUB_1, 0, 32 * 24 * 2, 0, HEAP_ID_116);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);

    {
        NNSG2dPaletteData *v3;
        void *v4;
        int v6, v10, v8, v9, v7, v11;

        MI_CpuFill8(&param0->unk_EC, 0, sizeof(param0->unk_EC));
        v4 = GfGfxLoader_GetPlttDataFromOpenNarc(v1, 5, &v3, HEAP_ID_116);
        MI_CpuCopy16(&((u16 *)(v3->pRawData))[0 * 16], param0->unk_EC.unk_08, 4 * 0x20);
        MI_CpuCopy16(&((u16 *)(v3->pRawData))[0 * 16], param0->unk_EC.unk_88, 4 * 0x20);
        Heap_Free(v4);

        v10 = 0;
        for (v6 = 0; v6 < 4 - 1; v6++) {
            v7 = v6 + 1;
            v8 = 0;
            v9 = 0;
            do {
                ((v10 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
                for (v11 = 1; v11 < 1 + 15; v11++) {
                    BlendPalette(&param0->unk_EC.unk_08[v6][v11], &param0->unk_EC.unk_88[v10][v11], 1, v8 >> 8, param0->unk_EC.unk_08[v7][v11]);
                }
                v10++;
                if (v9 == 1) {
                    break;
                }
                v8 += 0x300;
                if (v8 >= (16 << 8)) {
                    v8 = (16 << 8);
                    v9 = 1;
                }
            } while (1);
        }
        DC_FlushRange(param0->unk_EC.unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));

        param0->unk_EC.unk_04 = 1;
        param0->unk_EC.unk_32C = 0;
        param0->unk_EC.unk_00 = SysTask_CreateOnVBlankQueue(ov75_022476E8, &param0->unk_EC, 20);
    }

    NARC_Delete(v1);

    {
        v1 = NARC_New(NARC_a_1_9_7, HEAP_ID_116);

        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);

        GfGfxLoader_LoadCharDataFromOpenNarc(v1, 11, v0, GF_BG_LYR_MAIN_3, 0, 0, 0, HEAP_ID_116);
        GfGfxLoader_LoadScrnDataFromOpenNarc(v1, 10, v0, GF_BG_LYR_MAIN_3, 0, 0, 0, HEAP_ID_116);
        GfGfxLoader_GXLoadPalFromOpenNarc(v1, 12, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_9_OFFSET, 0x20, HEAP_ID_116);

        NARC_Delete(v1);
    }
}
