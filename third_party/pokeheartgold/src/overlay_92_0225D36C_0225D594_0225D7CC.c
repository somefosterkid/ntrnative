#include "global.h"

#include "constants/heap.h"
#include "overlay_92_unk_struct_ov116_0226139_c.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"
#include "struct_defs/resdat_narc_structs.h"

struct G2dRenderer;

struct G3DPipelineBuffers;

struct SpriteManager;

struct SpriteSystem;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void G2dRenderer_SetSubSurfaceCoords(G2dRenderer *g2dRenderer, fx32 x, fx32 y);

SpriteSystem *SpriteSystem_Alloc(enum HeapID heapID);

SpriteManager *SpriteManager_New(SpriteSystem *spriteSys);

G2dRenderer *SpriteSystem_GetRenderer(SpriteSystem *spriteSys);

BOOL SpriteSystem_Init(SpriteSystem *spriteSys, const OamManagerParam *oamTemplate, const OamCharTransferParam *transferTemplate, int plttCapacity);

BOOL SpriteSystem_InitSprites(SpriteSystem *spriteSys, SpriteManager *spriteMan, int maxSprites);

BOOL SpriteSystem_InitManagerWithCapacities(SpriteSystem *spriteSys, SpriteManager *spriteMan, const SpriteResourceCountsListUnion *capacities);

void ov92_0225D7CC(UnkStruct_ov116_0226139C *param0);

void ov92_0225D7CC(UnkStruct_ov116_0226139C *param0) {
    param0->unk_48.unk_08 = SpriteSystem_Alloc(HEAP_ID_113);
    {
        const OamManagerParam v0 = {
            0,
            128,
            0,
            32,
            0,
            128,
            0,
            32,
        };

        const OamCharTransferParam v1 = {
            64 + 64,
            1024 * 0x40,
            512 * 0x20,
            GX_OBJVRAMMODE_CHAR_1D_64K,
            GX_OBJVRAMMODE_CHAR_1D_64K
        };

        SpriteSystem_Init(param0->unk_48.unk_08, &v0, &v1, 16 + 16);
    }
    {
        BOOL v2;
        const SpriteResourceCountsListUnion v3 = {
            64 + 64,
            16 + 16,
            128,
            128,
            16,
            16,
        };

        param0->unk_48.unk_0C = SpriteManager_New(param0->unk_48.unk_08);

        v2 = SpriteSystem_InitSprites(param0->unk_48.unk_08, param0->unk_48.unk_0C, 255);
        GF_ASSERT(v2);

        v2 = SpriteSystem_InitManagerWithCapacities(param0->unk_48.unk_08, param0->unk_48.unk_0C, &v3);
        GF_ASSERT(v2);
    }
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(param0->unk_48.unk_08), 0, (192 + 64) << FX32_SHIFT);
}
