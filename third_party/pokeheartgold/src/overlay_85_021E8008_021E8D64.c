#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_85.h"
#include "pm_string.h"
#include "render_window.h"
#include "unk_0200A090.h"
#include "unk_0205B3DC.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

s32 ov85_021EA2C0(UnkStruct_ov109_021D5140 *, NARC *);

void ov85_021E8D64(UnkStruct_ov109_021D5140 *arg0, NARC *arg1) {
    NARC *narc;
    s32 v3;

    narc = arg1;
    v3 = 0;
    do {
        arg0->unk_3C[v3] = String_New(8U, HEAP_ID_102);
        arg0->unk_3D8[v3][0] = NULL;
        arg0->unk_3D8[v3][1] = NULL;
        arg0->unk_400[v3] = 0;
        v3 += 1;
    } while (v3 < 5);
    arg0->unk_54 = String_New(0xB4U, HEAP_ID_102);
    arg0->unk_58 = String_New(0x28U, HEAP_ID_102);
    arg0->unk_3B8 = 0;
    ReadMsgDataIntoString(arg0->unk_38, 0x11, arg0->unk_58);
    ov85_021EA2C0(arg0, narc);
    arg0->unk_41C = sub_0205B4A4(HEAP_ID_102);
    arg0->unk_1C.unk_00 = 0;
    arg0->unk_1C.unk_08 = 0;
    arg0->unk_1C.unk_04 = 0;
    arg0->unk_1C.unk_0C = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 1, &arg0->unk_1C.unk_10, HEAP_ID_102);
    arg0->unk_4AA8 = 0;
    arg0->unk_394 = 0;
    arg0->unk_10->unk_2C = 2;
}

struct JournalEntry;

struct ListMenu2D;

typedef struct JournalEntry JournalEntry;

typedef struct ListMenu2D ListMenu2D;

void ov85_021EA368(UnkStruct_ov109_021D5140 *param0);

void ov85_021E8E00(UnkStruct_ov109_021D5140 *param0);

void ov85_021E8E00(UnkStruct_ov109_021D5140 *param0) {
    int v0;

    ov85_021EA368(param0);
    Heap_Free(param0->unk_1C.unk_0C);
    Heap_Free(param0->unk_41C);

    for (v0 = 0; v0 < 5; v0++) {
        String_Delete(param0->unk_3C[v0]);
    }

    String_Delete(param0->unk_58);
    String_Delete(param0->unk_54);
}

void ov85_021E8E38(BgConfig *param0);

void ov85_021E8E38(BgConfig *param0) {
    FreeBgTilemapBuffer(param0, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(param0, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(param0, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(param0, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(param0, GF_BG_LYR_MAIN_0);
    Heap_Free(param0);
}

void ov85_021E8E6C(UnkStruct_ov109_021D5140 *param_1, NARC *param_2) {
    u32 uVar1;
    BgConfig *bgConfig;

    bgConfig = param_1->unk_14;
    GfGfxLoader_GXLoadPalFromOpenNarc(param_2, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x200, HEAP_ID_102);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_4, 0x47, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_102);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_102);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_102);
    GfGfxLoader_LoadCharData(NARC_a_0_1_4, 10, bgConfig, GF_BG_LYR_SUB_2, 0, 0, 1, HEAP_ID_102);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_4, 0xb, bgConfig, GF_BG_LYR_SUB_2, 0, 0, 1, HEAP_ID_102);
    GfGfxLoader_LoadCharDataFromOpenNarc(param_2, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x2000, 1, HEAP_ID_102);
    GfGfxLoader_LoadScrnDataFromOpenNarc(param_2, 3, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, 1, HEAP_ID_102);
    uVar1 = Options_GetFrame(param_1->unk_0C->unk_14.options);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 0xa, (u8)uVar1, HEAP_ID_102);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1f, 0xb, 0, HEAP_ID_102);
    return;
}
