#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "render_window.h"
#include "yes_no_prompt.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E5C00(void *arg0) {
    FreeBgTilemapBuffer(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_14, 6U);
    FreeBgTilemapBuffer(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_14, 5U);
    FreeBgTilemapBuffer(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_14, 4U);
    FreeBgTilemapBuffer(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_14, 3U);
    FreeBgTilemapBuffer(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_14, 2U);
    FreeBgTilemapBuffer(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_14, 1U);
    FreeBgTilemapBuffer(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_14, 0U);
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_14);
}

void ov14_021E5C54(UnkStruct_ov14_boxapp *arg0) {
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 0xE, arg0->sys->unk_14, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 0xE, arg0->sys->unk_14, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 3, arg0->sys->unk_14, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, HEAP_ID_10);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_9, 2, arg0->sys->unk_14, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 1, arg0->sys->unk_14, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_10);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_9, 0, arg0->sys->unk_14, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_10);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_9, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x80U, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 6, arg0->sys->unk_14, GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_10);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_9, 5, arg0->sys->unk_14, GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_10);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_9, 7, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 6, arg0->sys->unk_14, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_10);
}

void ov14_021E5D78(UnkStruct_ov14_boxapp *arg0) {
    arg0->sys->unk_20 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x18, HEAP_ID_10);
    arg0->sys->unk_1C = MessagePrinter_New(1U, 2U, 0U, HEAP_ID_10);
    arg0->sys->unk_24 = MessageFormat_New(HEAP_ID_10);
    arg0->sys->unk_28 = String_New(0x400U, HEAP_ID_10);
}

void ov14_021E5DB8(void *arg0) {
    String_Delete(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_28);
    MessageFormat_Delete(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_24);
    MessagePrinter_Delete(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_1C);
    DestroyMsgData(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_20);
}

void ov14_021E5DE0(UnkStruct_ov14_boxapp *param_1) {
    u32 uVar1;
    UnkStruct_ov14_boxsys *sys = param_1->sys;

    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 0x40, sys->unk_14, GF_BG_LYR_MAIN_0, 1000, 0x300, 1, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 0x40, sys->unk_14, GF_BG_LYR_MAIN_1, 1000, 0x300, 1, HEAP_ID_10);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_9, 0x41, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, 0x40, HEAP_ID_10);
    uVar1 = Options_GetFrame(param_1->options);
    LoadUserFrameGfx2(sys->unk_14, GF_BG_LYR_MAIN_0, 0x38e, 0xa, (u8)uVar1, HEAP_ID_10);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_11_OFFSET, HEAP_ID_10);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_10);
    return;
}

void ov14_021E5E74(UnkStruct_ov14_boxapp *arg0) {
    arg0->sys->unk_18 = PaletteData_Init(HEAP_ID_10);
    PaletteData_AllocBuffers(arg0->sys->unk_18, PLTTBUF_MAIN_BG, 0x200U, HEAP_ID_10);
}

void ov14_021E5E94(void *arg0) {
    PaletteData_FreeBuffers(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_18, PLTTBUF_MAIN_BG);
    PaletteData_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_18);
}

void ov14_021E5EAC(s32 arg0) {
    if (arg0 == 1) {
        G2_SetBlendAlpha(0, 0x2A, 6, 0xA);
        return;
    }
    *(s16 *)REG_BLDCNT_ADDR = 0;
}

void ov14_021E5ED0(UnkStruct_ov14_boxapp *arg0) {
    arg0->sys->unk_434 = YesNoPrompt_Create(HEAP_ID_10);
}

void ov14_021E5EE8(u8 *param_1) {
    YesNoPrompt_Destroy(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_434);
    return;
}
