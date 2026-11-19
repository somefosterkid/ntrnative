#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "gf_gfx_loader.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "vram_transfer_manager.h"
#include "heap.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct G3DPipelineBuffers;

struct SpriteList;

struct SpriteResourceCollection;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} UnkStruct_ov69_0225ECF0;

typedef struct {
    u16 unk_00[2];
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
} UnkStruct_ov69_0225F118;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_ov69_0225E0C0;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    void *unk_08[2];
    NNSG2dScreenData *unk_10[2];
    u16 unk_18;
    u16 unk_1A;
    void *unk_1C;
    NNSG2dPaletteData *unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
} UnkStruct_ov69_0225E7DC;

typedef struct {
    u8 unk_00;
    s8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov69_0225E084_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    const UnkStruct_ov69_0225ECF0 *unk_0C;
} UnkStruct_ov69_0225ED90;

typedef struct {
    UnkStruct_ov69_0225ED90 unk_00[3];
    s16 unk_30;
    s16 unk_32;
    u8 unk_34[4];
    u8 unk_38[4];
    Sprite *unk_3C[12];
    SpriteResource *unk_6C[12][4];
} UnkStruct_ov69_0225EB60;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225E0C0 unk_04[4];
    u16 unk_34;
    u16 unk_36;
    u32 unk_38;
    u16 unk_3C;
    u16 unk_3E;
    UnkStruct_ov69_0225E084_sub1 unk_40[18];
    u32 unk_88[3];
    void *unk_94;
    NNSG2dScreenData *unk_98;
    u16 unk_9C;
    u16 unk_9E;
    UnkStruct_ov69_0225E7DC unk_A0[5];
    Window unk_168[3];
    Window unk_198;
    Window unk_1A8;
    UnkStruct_ov69_0225EB60 unk_1B8;
    UnkStruct_ov69_0225E478 unk_2E4;
} UnkStruct_ov69_0225E084;

void ov48_0225A894(UnkStruct_ov69_0225E7DC *param0, UnkStruct_ov69_0225D35C *param1, const UnkStruct_ov69_0225F118 *param2, u16 param3, enum HeapID heapID);

void ov48_0225A868(UnkStruct_ov69_0225E084 *param0, u32 param1, u32 param2, const u8 *param3);

void ov48_0225A928(void *arg0);

void ov48_0225A868(UnkStruct_ov69_0225E084 *param0, u32 param1, u32 param2, const u8 *param3) {
    u16 v0 = GetWindowBaseTile(&param0->unk_168[param1]);
    v0 += (27 * param2);

    GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG1_CHAR_SUB, v0 * 32, (void *)param3, 27 * 32);
}

void ov48_0225A894(UnkStruct_ov69_0225E7DC *param0, UnkStruct_ov69_0225D35C *param1, const UnkStruct_ov69_0225F118 *param2, u16 param3, enum HeapID heapID) {
    int v0;

    memset(param0, 0, sizeof(UnkStruct_ov69_0225E7DC));
    param0->unk_02 = param3;

    if (param2->unk_04 > 0) {
        param0->unk_18 = param2->unk_04;

        for (v0 = 0; v0 < param0->unk_18; v0++) {
            param0->unk_08[v0] = GfGfxLoader_GetScrnDataFromOpenNarc(
                param1->unk_1A8, param2->unk_00[v0], 0, &param0->unk_10[v0], heapID);
        }
    }

    if (param2->unk_08 > 0) {
        param0->unk_24 = param2->unk_08;
        param0->unk_26 = param2->unk_0A;
        param0->unk_1C = GfGfxLoader_GetPlttDataFromOpenNarc(param1->unk_1A8, param2->unk_06, &param0->unk_20, heapID);
    }
}

void ov48_0225A928(void *arg0) {
    s32 v1;
    u16 v0;
    void *v2;

    v0 = *(u16 *)((u8 *)arg0 + 0x18);
    if (v0 != 0) {
        v1 = 0;
        if ((s32)v0 > 0) {
            v2 = arg0;
            do {
                Heap_Free(*(void **)((u8 *)v2 + 8));
                v1 += 1;
                v2 += 4;
            } while (v1 < (s32) * (u16 *)((u8 *)arg0 + 0x18));
        }
    }
    if (*(u8 *)((u8 *)arg0 + 0x24) != 0) {
        Heap_Free(*(void **)((u8 *)arg0 + 0x1C));
    }
}
