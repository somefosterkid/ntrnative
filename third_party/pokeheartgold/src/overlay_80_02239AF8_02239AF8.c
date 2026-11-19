#include "global.h"
#include "filesystem.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite_system.h"

struct NARC;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct ManagedSpriteTemplate;

BOOL SpriteSystem_LoadCellResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

extern const ManagedSpriteTemplate ov80_0223DA54[4];

void ov80_02239AF8(SpriteSystem *param0, SpriteManager *param1, NARC *param2, PaletteData *param3, u16 param4);

void ov80_02239AF8(SpriteSystem *param0, SpriteManager *param1, NARC *param2, PaletteData *param3, u16 param4) {
    const ManagedSpriteTemplate *v0;

    GF_ASSERT(param4 < NELEMS(ov80_0223DA54));
    v0 = &ov80_0223DA54[param4];

    SpriteSystem_LoadCharResObjFromOpenNarc(param0, param1, param2, v0->resIdList[GF_GFX_RES_TYPE_CHAR], TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, v0->resIdList[GF_GFX_RES_TYPE_CHAR]);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(param3, PLTTBUF_MAIN_OBJ, param0, param1, param2, v0->resIdList[GF_GFX_RES_TYPE_PLTT], FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, v0->resIdList[GF_GFX_RES_TYPE_PLTT]);
    SpriteSystem_LoadCellResObjFromOpenNarc(param0, param1, param2, v0->resIdList[GF_GFX_RES_TYPE_CELL], TRUE, v0->resIdList[GF_GFX_RES_TYPE_CELL]);
    SpriteSystem_LoadAnimResObjFromOpenNarc(param0, param1, param2, v0->resIdList[GF_GFX_RES_TYPE_ANIM], TRUE, v0->resIdList[GF_GFX_RES_TYPE_ANIM]);
}
