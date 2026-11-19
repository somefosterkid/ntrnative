#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "camera.h"
#include "heap.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"
#include "gf_gfx_planes.h"

struct JournalEntry;

struct UnkStruct_ov109_021D0F70_t;

typedef struct JournalEntry JournalEntry;

void *ov85_021E8588(UnkStruct_ov109_021D0F70 *param0, u32 param1, BOOL param2);

void ov85_021E6B68(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E6B68(UnkStruct_ov109_021D0F70 *param0) {
    void *v0 = ov85_021E8588(param0, 17, 0);
    NNS_G2dGetUnpackedPaletteData(v0, &param0->unk_D90);
    PaletteData_LoadPalette(param0->unk_D9C, param0->unk_D90->pRawData, PLTTBUF_MAIN_BG, 0, ((16) * sizeof(u16)) * 2);
    Heap_Free(v0);

    v0 = ov85_021E8588(param0, 20, 0);
    NNS_G2dGetUnpackedPaletteData(v0, &param0->unk_D90);
    PaletteData_LoadPalette(param0->unk_D9C, param0->unk_D90->pRawData, PLTTBUF_SUB_BG, 0, ((16) * sizeof(u16)) * 2);
    Heap_Free(v0);

    PaletteData_BlendPalettes(param0->unk_D9C, PLTTBUF_SUB_BG, 0xffff, 8, 0);

    v0 = ov85_021E8588(param0, 16, 0);
    NNS_G2dGetUnpackedCharacterData(v0, &param0->unk_D8C);
    BG_LoadCharTilesData(param0->unk_D84, 3, param0->unk_D8C->pRawData, param0->unk_D8C->szByte, 32 * 0);
    Heap_Free(v0);

    v0 = ov85_021E8588(param0, 22, 0);
    NNS_G2dGetUnpackedCharacterData(v0, &param0->unk_D8C);
    BG_LoadCharTilesData(param0->unk_D84, 6, param0->unk_D8C->pRawData, param0->unk_D8C->szByte, 32 * 0);
    Heap_Free(v0);

    v0 = ov85_021E8588(param0, 21, 0);
    NNS_G2dGetUnpackedCharacterData(v0, &param0->unk_D8C);
    BG_LoadCharTilesData(param0->unk_D84, 7, param0->unk_D8C->pRawData, param0->unk_D8C->szByte, 32 * 0);
    Heap_Free(v0);

    v0 = ov85_021E8588(param0, 19, 0);
    NNS_G2dGetUnpackedScreenData(v0, &param0->unk_D88);
    BG_LoadScreenTilemapData(param0->unk_D84, 2, (void *)param0->unk_D88->rawData, param0->unk_D88->szByte);
    BgCommitTilemapBufferToVram(param0->unk_D84, 2);
    Heap_Free(v0);

    v0 = ov85_021E8588(param0, 18, 0);
    NNS_G2dGetUnpackedScreenData(v0, &param0->unk_D88);
    BG_LoadScreenTilemapData(param0->unk_D84, 3, (void *)param0->unk_D88->rawData, param0->unk_D88->szByte);
    BgCommitTilemapBufferToVram(param0->unk_D84, 3);
    Heap_Free(v0);

    v0 = ov85_021E8588(param0, 24, 0);
    NNS_G2dGetUnpackedScreenData(v0, &param0->unk_D88);
    BG_LoadScreenTilemapData(param0->unk_D84, 6, (void *)param0->unk_D88->rawData, param0->unk_D88->szByte);
    BgCommitTilemapBufferToVram(param0->unk_D84, 6);
    Heap_Free(v0);

    v0 = ov85_021E8588(param0, 23, 0);
    NNS_G2dGetUnpackedScreenData(v0, &param0->unk_D88);
    BG_LoadScreenTilemapData(param0->unk_D84, 7, (void *)param0->unk_D88->rawData, param0->unk_D88->szByte);
    BgCommitTilemapBufferToVram(param0->unk_D84, 7);
    Heap_Free(v0);
}

void ov85_021E6D68(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E6D68(UnkStruct_ov109_021D0F70 *param0) {
    FreeBgTilemapBuffer(param0->unk_D84, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(param0->unk_D84, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(param0->unk_D84, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(param0->unk_D84, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(param0->unk_D84, GF_BG_LYR_SUB_3);
    Heap_Free(param0->unk_D84);
}

static void ov85_021E6DAC(UnkStruct_ov109_021D0F70 *param0, u32 param1);

static void ov85_021E6DAC(UnkStruct_ov109_021D0F70 *param0, u32 param1) {
    void *v0 = ov85_021E8588(param0, param1, 0);
    NNS_G2dGetUnpackedScreenData(v0, &param0->unk_D88);

    BG_LoadScreenTilemapData(param0->unk_D84, 6, (void *)param0->unk_D88->rawData, param0->unk_D88->szByte);
    BgCommitTilemapBufferToVram(param0->unk_D84, 6);
    Heap_Free(v0);
}

void ov85_021E6DF0(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E6DF0(UnkStruct_ov109_021D0F70 *param0) {
    ov85_021E6DAC(param0, 26);
}

void ov85_021E6DFC(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E6DFC(UnkStruct_ov109_021D0F70 *param0) {
    ov85_021E6DAC(param0, 24);
}

void ov85_021E6E08(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E6E08(UnkStruct_ov109_021D0F70 *param0) {
    ov85_021E6DAC(param0, 25);
}

void ov85_021E6E14(struct UnkStruct_ov109_021D0F70_t *arg0);

void ov85_021E6E14(struct UnkStruct_ov109_021D0F70_t *arg0) {
    arg0->unk_D9C = PaletteData_Init(HEAP_ID_102);
    PaletteData_SetAutoTransparent(arg0->unk_D9C, 1);
    PaletteData_AllocBuffers(arg0->unk_D9C, PLTTBUF_MAIN_BG, 0x200U, HEAP_ID_102);
    PaletteData_AllocBuffers(arg0->unk_D9C, PLTTBUF_MAIN_OBJ, 0x200U, HEAP_ID_102);
    PaletteData_AllocBuffers(arg0->unk_D9C, PLTTBUF_SUB_BG, 0x200U, HEAP_ID_102);
    PaletteData_AllocBuffers(arg0->unk_D9C, PLTTBUF_SUB_OBJ, 0x200U, HEAP_ID_102);
}

void ov85_021E6E6C(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E6E6C(UnkStruct_ov109_021D0F70 *param0) {
    PaletteData_FreeBuffers(param0->unk_D9C, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(param0->unk_D9C, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(param0->unk_D9C, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(param0->unk_D9C, PLTTBUF_SUB_OBJ);
    PaletteData_Free(param0->unk_D9C);
}

void ov85_021E6EA8(UnkStruct_ov109_021D0F70 *param0, u32 param1);

void ov85_021E6EA8(UnkStruct_ov109_021D0F70 *param0, u32 param1) {
    PaletteData_BlendPalettes(param0->unk_D9C, PLTTBUF_SUB_BG, 0xffff, param1, 0);
}
