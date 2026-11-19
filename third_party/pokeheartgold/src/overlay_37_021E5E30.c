#include "global.h"

#include "unk_02095EAC.h"
#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "unk_02032844.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "render_window.h"

u16 sub_0203769C(void);
void ov37_021E7844(UnkStruct_02095EAC *param0, int param1);

void ov37_021E5E30(UnkStruct_02095EAC *param0);
void ov37_021E5F20(UnkStruct_02095EAC *param0);
void ov37_021E5F5C(BgConfig *param0);

void ov37_021E5F98(UnkStruct_02095EAC *arg0, NARC *arg1);

void ov37_021E5E30(UnkStruct_02095EAC *param0) {
    int v0;

    for (v0 = 0; v0 < 5; v0++) {
        param0->unk_14[v0] = String_New(7 + 1, HEAP_ID_39);
        param0->unk_43E6[v0].unk_08_3 = 0;
        param0->unk_4418[v0].unk_02 = 0;
        param0->unk_398[v0][0] = 0L;
        param0->unk_398[v0][1] = 0L;
    }

    param0->unk_28 = String_New(10, HEAP_ID_39);
    param0->unk_2C = String_New(40 * 2, HEAP_ID_39);
    param0->unk_43DA = 0;
    param0->unk_43DB = 1;
    param0->unk_9458 = 1;
    param0->unk_945C = 0;
    param0->unk_37C = 2;
    param0->unk_380 = sub_02033250();
    param0->unk_9460 = 0;
    param0->unk_9464 = 0;
    param0->unk_368 = 0;
    param0->unk_9468 = 0;
    param0->unk_946C = 0;

    if (sub_0203769C() == 0) {
        ov37_021E7844(param0, 4);
    } else {
        ov37_021E7844(param0, 4);
    }

    ReadMsgDataIntoString(param0->unk_10, 7, param0->unk_28);

    param0->unk_442C = Heap_Alloc(HEAP_ID_39, 30 * 16 * 32);
    param0->unk_9454 = YesNoPrompt_Create(HEAP_ID_39);
    param0->unk_378 = 0;
}

void ov37_021E5F20(UnkStruct_02095EAC *param0) {
    int v0;

    Heap_Free(param0->unk_442C);
    YesNoPrompt_Destroy(param0->unk_9454);

    for (v0 = 0; v0 < 5; v0++) {
        String_Delete(param0->unk_14[v0]);
    }

    String_Delete(param0->unk_2C);
    String_Delete(param0->unk_28);
}

void ov37_021E5F5C(BgConfig *param0) {
    FreeBgTilemapBuffer(param0, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(param0, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(param0, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(param0, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(param0, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(param0, GF_BG_LYR_MAIN_0);
    Heap_Free(param0);
}

void ov37_021E5F98(UnkStruct_02095EAC *arg0, NARC *narc) {
    BgConfig *bgConfig;
    bgConfig = arg0->unk_00;
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, HEAP_ID_39);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 1, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, HEAP_ID_39);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_39);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_39);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0x2000U, 1, HEAP_ID_39);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0x600U, 1, HEAP_ID_39);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 3, bgConfig, GF_BG_LYR_SUB_1, 0U, 0x2000U, 1, HEAP_ID_39);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 5, bgConfig, GF_BG_LYR_SUB_1, 0U, 0x600U, 1, HEAP_ID_39);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_FIRST, 1U, 0xAU, (u8)((s32)Options_GetFrame(arg0->unk_08->options)), HEAP_ID_39);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_FIRST, 0x1FU, 0xBU, 0U, HEAP_ID_39);
}
