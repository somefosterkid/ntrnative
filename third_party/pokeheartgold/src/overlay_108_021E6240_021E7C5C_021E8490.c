#include "global.h"

#include "battle/battle.h"
#include "library/spl_emitter.h"

#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_07_ball_rotation.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_108_safari_area_customizer.h"
#include "overlay_07_value_lerp_context.h"
#include "overlay_07_palette_fade.h"

typedef struct UnkStruct_ov108_021E84F8 {
    enum HeapID unk_0;
    u8 unk_4[0x140];
    GF_2DGfxResMan *unk_144;
    u8 unk_148[0xC];
    u8 **unk_154;
} UnkStruct_ov108_021E84F8;

void ov108_021E8490(SafariAreaCustomizer *arg0);

void ov108_021E8490(SafariAreaCustomizer *arg0) {
    if (arg0->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
}

s32 ov108_021E867C(void *);

void *ov108_021E84A4(enum HeapID arg0, s16 arg1, s16 arg2, s32 arg3);

void *ov108_021E84A4(enum HeapID arg0, s16 arg1, s16 arg2, s32 arg3) {
    s32 v0;
    void *dest;

    v0 = arg3;
    dest = Heap_Alloc(arg0, 0x164U);
    MI_CpuFill8(dest, 0U, 0x164U);
    *(enum HeapID *)dest = arg0;
    *(s16 *)((u8 *)dest + 0xA) = 4;
    *(s16 *)((u8 *)dest + 0xC) = arg1;
    *(s16 *)((u8 *)dest + 0xE) = arg2;
    *(s32 *)((u8 *)dest + 4) = v0;
    ov108_021E867C(dest);
    return dest;
}

struct PoffinSpriteItem;

struct SpriteResourceCollection;

struct GF_2DGfxResObjList;

typedef struct PoffinSpriteItem PoffinSpriteItem;

typedef struct PoffinSpriteManager {
    enum HeapID heapID;
    int transferType;
    u16 unused;
    u16 numPalettes;
    u16 maxSprites;
    u16 vramType;
    SpriteList *spriteList;
    G2dRenderer renderer;
    u32 unused2;
    SpriteResourceCollection *ownedResources[4];
    GF_2DGfxResObjList *unownedResources[4];
    PoffinSpriteItem *poffinList;
} PoffinSpriteManager;

void ov108_021E8718(PoffinSpriteManager *spriteMan);

void ov108_021E84DC(PoffinSpriteManager *spriteMan);

void ov108_021E84DC(PoffinSpriteManager *spriteMan) {
    ov108_021E8718(spriteMan);
    MI_CpuClear8(spriteMan, sizeof(PoffinSpriteManager));
    Heap_Free(spriteMan);
}

void ov108_021E84F8(UnkStruct_ov108_021E84F8 *arg0, s32 arg1, s32 arg2);

void ov108_021E84F8(UnkStruct_ov108_021E84F8 *arg0, s32 arg1, s32 arg2) {
    SpriteResource *spriteResource;

    spriteResource = *(SpriteResource **)(*arg0->unk_154 + (arg1 * 4));
    ReplaceCharResObjFromNarc(arg0->unk_144, spriteResource, NARC_a_1_6_6, arg2 + 0x12, 0, arg0->unk_0);
    SpriteTransfer_ReplaceCharData(spriteResource);
}

void ov108_021E852C(void *arg0);

void ov108_021E852C(void *arg0) {
    SpriteList *v0;

    v0 = *(SpriteList **)((u8 *)arg0 + 0x10);
    if (v0 != NULL) {
        SpriteList_RenderAndAnimateSprites(v0);
    }
}

struct BallRotation;

struct BattleMonOBJData;

struct PaletteFadeContext;

struct ParticleSystem;

struct UnkStruct_ov12_02236648_t;

typedef struct UnkStruct_ov12_02236648_t UnkStruct_ov12_02236648;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    int unk_04;
    enum HeapID heapID;
    int unk_0C;
    int unk_10;
} UnkStruct_ov12_02236690;

typedef struct BattleMonOBJData BattleMonOBJData;

struct UnkStruct_ov12_02236648_t {
    UnkStruct_ov12_02236690 unk_00;
    SPLEmitter *unk_14;
    ParticleSystem *unk_18;
    int unk_1C;
    int unk_20;
};

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov12_02237C54_sub1;

void ov108_021E853C(BallRotation *param0, int param1);

void ov108_021E853C(BallRotation *param0, int param1) {
    param0->unk_14 = param1;
}
