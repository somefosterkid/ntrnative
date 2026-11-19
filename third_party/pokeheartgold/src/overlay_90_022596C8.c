#include "global.h"
#include "sprite_resources_header.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "overlay_90.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "heap.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_90_unk_struct_ov114_0225_e1_a4.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"

struct UnkStruct_ov90_0225C178 {
    u8 unk_0[0x4];
    u16 unk_4;
    u8 unk_6[0x2A];
    BgConfig *unk_30;
    u8 unk_34[0x8];
    u8 unk_3C;
    u8 unk_3D[0x173];
    u8 unk_1B0;
    u8 unk_1B1[0x17];
    void *unk_1C8;
    u8 unk_1CC;
    u8 unk_1CD[0x2F];
    void *unk_1FC;
    u8 unk_200;
};

void *ov90_02258E54(void *, NARC *, s32, s32, s32, s32, s32, s32, enum HeapID);

s32 ov90_0225A2B0(void *, void *, void *, s32, NARC *, enum HeapID);

s32 ov90_0225A65C(NARC *, s32, enum HeapID);

struct BufferManager;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_ov114_0225D678_t;
struct UnkStruct_ov66_0222DFF8_t;
typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;
struct UnkStruct_ov114_0225D678_t {
    u8 unk_00;
    u8 unk_01;
    u16 heapID;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    UnkStruct_ov114_0225C76C unk_08;
    UnkStruct_ov114_0225E0F8 unk_18;
    UnkStruct_ov114_0225CEF0 unk_30;
    UnkStruct_ov114_0225D084 unk_3C;
    UnkStruct_ov114_0225D338 unk_4C;
    UnkStruct_ov114_0225CFCC unk_84;
    UnkStruct_ov114_0225CCD0 unk_214;
    UnkStruct_ov114_0225CDB4 *unk_22C;
    UnkStruct_ov114_0225E1A4 unk_230;
    void *unk_260;
    NNSG2dScreenData *unk_264;
    UnkStruct_ov114_0225DAC0 unk_268[4];
    UnkStruct_ov114_0225DFFC unk_348;
    SysTask *unk_650;
    SysTask *unk_654;
};
void ov90_02259784(UnkStruct_ov114_0225D678 *param0);

void ov90_022596C8(UnkStruct_ov90_0225C178 *arg0, enum HeapID heapID) {
    NARC *narc;

    narc = NARC_New(NARC_a_1_9_8, heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x120U, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, arg0->unk_30, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, heapID);
    arg0->unk_1FC = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 9, 0, (void *)&arg0->unk_200, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, arg0->unk_30, GF_BG_LYR_MAIN_3, 0U, 0U, 0, heapID);
    ov90_0225A65C(narc, arg0->unk_4, heapID);
    arg0->unk_1C8 = ov90_02258E54((void *)&arg0->unk_1B0, narc, 0xB, 5, 0xC, 0xD, 0xE, 0x1388, heapID);
    ov90_0225A2B0((void *)&arg0->unk_1CC, (void *)&arg0->unk_30, (void *)&arg0->unk_3C, arg0->unk_4, narc, heapID);
    NARC_Delete(narc);
}

void ov90_02259784(UnkStruct_ov114_0225D678 *param0) {
    Heap_Free(param0->unk_260);
}
