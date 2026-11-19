#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"

typedef struct UnkStruct_ov96_021F3CBC {
    enum HeapID unk_0;
    u8 unk_4[0x4];
    BgConfig *unk_8;
    u8 unk_C[0x5C];
    u8 unk_68;
    u8 unk_69[0x97];
    void *unk_100;
    u8 unk_104;
    u8 unk_105[0x3];
    void *unk_108;
    void *unk_10C;
    void *unk_110;
    void *unk_114;
    void *unk_118;
    u8 unk_11C[0x38];
    void *unk_154;
    void *unk_158;
    void *unk_15C;
    void *unk_160;
    void *unk_164;
    void *unk_168;
    void *unk_16C;
} UnkStruct_ov96_021F3CBC;

s32 ov96_021F4EF8(void *, BgConfig *);

s32 ov96_021F4FD8(s32, void *);

void ov96_021F3CBC(UnkStruct_ov96_021F3CBC *arg0);

void ov96_021F3CBC(UnkStruct_ov96_021F3CBC *arg0) {
    NNSG2dScreenData *ppScrnData;
    void *v0;
    void *v1;
    void *ptr;

    ptr = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 0xD, 0, &ppScrnData, arg0->unk_0);
    BG_LoadScreenTilemapData(arg0->unk_8, 5U, ppScrnData->rawData, ppScrnData->szByte);
    BgTilemapRectChangePalette(arg0->unk_8, 5U, 0U, 0U, 0x20U, 0x18U, 2U);
    ScheduleBgTilemapBufferTransfer(arg0->unk_8, 5U);
    Heap_Free(ptr);
    arg0->unk_168 = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 0x17, 0, (void *)&arg0->unk_100, arg0->unk_0);
    arg0->unk_16C = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 0x18, 0, (void *)&arg0->unk_104, arg0->unk_0);
    v0 = arg0->unk_100;
    BG_LoadScreenTilemapData(arg0->unk_8, 7U, v0 + 0xC, *(u32 *)((u8 *)v0 + 8));
    GfGfx_EngineBTogglePlanes(8U, 0U);
    arg0->unk_154 = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 0x12, 0, (void *)&arg0->unk_10C, arg0->unk_0);
    arg0->unk_158 = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 0x13, 0, (void *)&arg0->unk_110, arg0->unk_0);
    arg0->unk_15C = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 0x11, 0, (void *)&arg0->unk_108, arg0->unk_0);
    arg0->unk_160 = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 0x14, 0, (void *)&arg0->unk_114, arg0->unk_0);
    arg0->unk_164 = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 0x10, 0, (void *)&arg0->unk_118, arg0->unk_0);
    ov96_021F4FD8(0x80, arg0->unk_10C);
    ov96_021F4FD8(0x80, arg0->unk_110);
    ov96_021F4FD8(0x80, arg0->unk_114);
    ov96_021F4FD8(0x80, arg0->unk_108);
    ov96_021F4FD8(0x80, arg0->unk_118);
    v1 = arg0->unk_10C;
    BG_LoadScreenTilemapData(arg0->unk_8, 6U, v1 + 0xC, *(u32 *)((u8 *)v1 + 8));
    ov96_021F4EF8((void *)&arg0->unk_68, arg0->unk_8);
    BgTilemapRectChangePalette(arg0->unk_8, 6U, 0U, 0U, 0x20U, 0x18U, 3U);
    ScheduleBgTilemapBufferTransfer(arg0->unk_8, 6U);
}

void ov96_021F4484(void *arg0);

void ov96_021F3E58(void *arg0);

void ov96_021F3E58(void *arg0) {
    ov96_021F4484(arg0);
}
