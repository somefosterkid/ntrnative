#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "sys_task.h"

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct SelectedPaletteData;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

typedef struct {
    int unk_00;
    enum HeapID heapID;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
} UnkStruct_0208B284;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    PaletteData *unk_0C;
} UnkStruct_0208B878_sub1;

typedef struct {
    ManagedSprite *unk_00;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
} UnkStruct_0208B878_sub2;

typedef struct {
    UnkStruct_0208B878_sub1 unk_00;
    UnkStruct_0208B284 unk_10;
    UnkStruct_0208B878_sub2 unk_24[12];
    int unk_E4;
    int unk_E8;
    int unk_EC;
    BOOL unk_F0;
    SysTask *unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
} UnkStruct_0208B878;

extern const int _02102814[7];

void sub_0208763C(UnkStruct_0208B878 *param0, int param1);

void sub_0208763C(UnkStruct_0208B878 *param0, int param1) {
    NARC *v0;
    SpriteSystem *v1 = param0->unk_00.unk_04;
    SpriteManager *v2 = param0->unk_00.unk_08;
    PaletteData *v3 = param0->unk_00.unk_0C;
    v0 = param0->unk_00.unk_00;

    SpriteManager_UnloadPlttObjById(v2, 22222 + 1);
    SpriteManager_UnloadPlttObjById(v2, 22222 + 2);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(v3, (enum PaletteBufferId)(2), v1, v2, v0, _02102814[param1], 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 22222 + 1);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(v3, (enum PaletteBufferId)(3), v1, v2, v0, _02102814[param1], 0, 1, NNS_G2D_VRAM_TYPE_2DSUB, 22222 + 2);
}

BOOL SpriteSystem_LoadCellResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

void sub_020876B0(UnkStruct_0208B878 *param0, int param1);

void sub_020876B0(UnkStruct_0208B878 *param0, int param1) {
    NARC *v0;
    SpriteSystem *v1;
    SpriteManager *v2;
    PaletteData *v3;
    int v4 = 22222 + param0->unk_10.unk_08;

    v1 = param0->unk_00.unk_04;
    v2 = param0->unk_00.unk_08;
    v3 = param0->unk_00.unk_0C;
    v0 = param0->unk_00.unk_00;

    if (param0->unk_10.unk_08 == 1) {
        if (param0->unk_10.unk_0C == 0) {
            SpriteSystem_LoadPaletteBufferFromOpenNarc(v3, (enum PaletteBufferId)(2), v1, v2, v0, _02102814[param1], 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, v4);
        } else {
            SpriteSystem_LoadPaletteBufferFromOpenNarc(v3, (enum PaletteBufferId)(2), v1, v2, v0, 96, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, v4);
        }

        SpriteSystem_LoadCharResObjFromOpenNarc(v1, v2, v0, 95, 0, NNS_G2D_VRAM_TYPE_2DMAIN, v4);
    } else {
        if (param0->unk_10.unk_0C == 0) {
            SpriteSystem_LoadPaletteBufferFromOpenNarc(v3, PLTTBUF_SUB_OBJ, v1, v2, v0, _02102814[param1], 0, 1, NNS_G2D_VRAM_TYPE_2DSUB, v4);
        } else {
            SpriteSystem_LoadPaletteBufferFromOpenNarc(v3, PLTTBUF_SUB_OBJ, v1, v2, v0, 96, 0, 1, NNS_G2D_VRAM_TYPE_2DSUB, v4);
        }

        SpriteSystem_LoadCharResObjFromOpenNarc(v1, v2, v0, 95, 0, NNS_G2D_VRAM_TYPE_2DSUB, v4);
    }

    SpriteSystem_LoadCellResObjFromOpenNarc(v1, v2, v0, 93, 0, v4);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v1, v2, v0, 94, 0, v4);
}
