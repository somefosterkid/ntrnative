#include "global.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "bg_window.h"
#include "palette.h"
#include "sprite_resources_header.h"
#include "frontier_graphics.h"
#include "overlay_45_unk_struct_ov66_02234798.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"

struct BattleFrontier;

struct FrontierParticleSystem;

struct G3DPipelineBuffers;

struct Sprite;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

typedef struct BattleFrontier BattleFrontier;

typedef struct Sprite Sprite;

typedef struct SpriteResourcesHeaderList SpriteResourcesHeaderList;

typedef struct ManagedSprite {
    Sprite *sprite;
    SpriteResourcesHeader *resourceHeader;
    SpriteResourcesHeaderList *resourceHeaderList;
    BOOL vramTransfer;
} ManagedSprite;

void Sprite_SetPalOffsetRespectVramOffset(Sprite *sprite, u8 paletteOffset);

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

typedef struct SpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animIdx;
    int priority;
    int plttIdx;
    enum NNS_G2D_VRAM_TYPE vramType;
    int resources[GF_GFX_RES_TYPE_MAX];
    int bgPriority;
    BOOL vramTransfer;
} SpriteTemplate;

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const SpriteTemplate *template);

void ManagedSprite_TickFrame(ManagedSprite *managedSprite);

BOOL SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, enum NNS_G2D_VRAM_TYPE vramType, int resourceID);

typedef struct FrontierParticleSystem FrontierParticleSystem;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

u32 Pokemon_GetIconNaix(Pokemon *mon);

const u8 Pokemon_GetIconPalette(Pokemon *mon);

extern const SpriteTemplate ov80_0223BD80;

ManagedSprite *ov80_0222F29C(FrontierGraphics *graphics, Pokemon *mon, int resourceID, int x, int y);

ManagedSprite *ov80_0222F29C(FrontierGraphics *graphics, Pokemon *mon, int resourceID, int x, int y) {
    ManagedSprite *sprite;
    SpriteTemplate template;

    ((resourceID < 2009 - 1 - 2000) ? (void)0 : GF_AssertFail());

    SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(
        graphics->spriteSystem, graphics->spriteMan, (enum NarcId)20, Pokemon_GetIconNaix(mon), 0, NNS_G2D_VRAM_TYPE_2DMAIN, 2000 + resourceID);

    template = ov80_0223BD80;

    template.resources[0] += resourceID;
    template.x = x;
    template.y = y;
    template.priority = 200;

    sprite = SpriteSystem_NewSprite(graphics->spriteSystem, graphics->spriteMan, &template);

    Sprite_SetPalOffsetRespectVramOffset(sprite->sprite, Pokemon_GetIconPalette(mon));
    ManagedSprite_TickFrame(sprite);

    return sprite;
}
