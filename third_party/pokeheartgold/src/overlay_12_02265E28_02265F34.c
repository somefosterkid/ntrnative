#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"

struct BattleSystem;

struct ManagedSprite;

struct Sprite;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct ManagedSpriteTemplate;

struct Terrain;

typedef struct BattleSystem BattleSystem;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void Sprite_TickFrame(Sprite *sprite);

typedef struct Terrain {
    ManagedSprite *managedSprite;
    BattleSystem *battleSys;
    u8 side;
    u8 terrainType;
    s16 unused_0A;
    s16 unused_0C;
} Terrain;

SpriteSystem *BattleSystem_GetSpriteSystem(BattleSystem *battleSys);

SpriteManager *BattleSystem_GetSpriteManager(BattleSystem *battleSys);

extern const ManagedSpriteTemplate ov12_0226E100[2];

void ov12_02265F34(Terrain *terrain);

void ov12_02265F34(Terrain *terrain) {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    const ManagedSpriteTemplate *spriteTemplate;

    spriteSys = BattleSystem_GetSpriteSystem(terrain->battleSys);
    spriteMan = BattleSystem_GetSpriteManager(terrain->battleSys);
    spriteTemplate = &ov12_0226E100[terrain->side];

    terrain->managedSprite = SpriteSystem_NewSprite(spriteSys, spriteMan, spriteTemplate);
    Sprite_TickFrame(terrain->managedSprite->sprite);
}
