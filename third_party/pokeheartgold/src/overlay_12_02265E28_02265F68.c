#include "global.h"

#include "field/map_prop_animation.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct BattleSystem;
struct ManagedSprite;
struct SpriteList;
struct SpriteResourceCollection;
struct GF_2DGfxResObjList;
struct GF_2DGfxResHeader;
struct SpriteResourcesHeaderList;
void Sprite_DeleteAndFreeResources(void *ptr);
typedef struct BattleSystem BattleSystem;
typedef struct SpriteResourcesHeaderList SpriteResourcesHeaderList;
typedef struct SpriteManager {
    SpriteList *sprites;
    SpriteResourcesHeaderList *resourceHeaders;
    GF_2DGfxResHeader *resourcePaths;
    SpriteResourceCollection *ownedResources[GF_GFX_RES_TYPE_MAX];
    GF_2DGfxResObjList *unownedResources[GF_GFX_RES_TYPE_MAX];
    int loadedResourceCount[GF_GFX_RES_TYPE_MAX];
    int numResourceTypes;
} SpriteManager;
BOOL SpriteManager_UnloadCharObjById(SpriteManager *spriteMan, int resourceID);
BOOL SpriteManager_UnloadPlttObjById(SpriteManager *spriteMan, int resourceID);
BOOL SpriteManager_UnloadCellObjById(SpriteManager *spriteMan, int resourceID);
BOOL SpriteManager_UnloadAnimObjById(SpriteManager *spriteMan, int resourceID);
typedef struct Terrain {
    ManagedSprite *managedSprite;
    BattleSystem *battleSys;
    u8 side;
    u8 terrainType;
    s16 unused_0A;
    s16 unused_0C;
} Terrain;
SpriteManager *BattleSystem_GetSpriteManager(BattleSystem *battleSys);

void ov12_02265F68(MapPropOneShotAnimationManager **oneShotAnimMan);
void ov12_02265F7C(Terrain *terrain);

void ov12_02265F68(MapPropOneShotAnimationManager **oneShotAnimMan) {
    if (*oneShotAnimMan == 0L) {
        return;
    }

    Sprite_DeleteAndFreeResources(*oneShotAnimMan);
    *oneShotAnimMan = 0L;
}

void ov12_02265F7C(Terrain *terrain) {
    SpriteManager *spriteMan;
    int charResourceID, cellResourceID, animResourceID;

    spriteMan = BattleSystem_GetSpriteManager(terrain->battleSys);

    if (terrain->side == 0) {
        charResourceID = 20013;
        cellResourceID = 20005;
        animResourceID = 20005;
    } else {
        charResourceID = 20014;
        cellResourceID = 20006;
        animResourceID = 20006;
    }

    SpriteManager_UnloadCharObjById(spriteMan, charResourceID);
    SpriteManager_UnloadPlttObjById(spriteMan, 20009);
    SpriteManager_UnloadCellObjById(spriteMan, cellResourceID);
    SpriteManager_UnloadAnimObjById(spriteMan, animResourceID);
}
