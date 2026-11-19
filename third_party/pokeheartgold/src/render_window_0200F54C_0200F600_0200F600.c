#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "wild_encounter.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_resource_manager.h"

typedef enum HeapID HeapID;

struct BallSeal;

struct BgConfig;

struct ManagedSprite;

struct Pokemon;

struct PokemonPreview;

struct SpriteResourceManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef union SpriteResourceCapacities {
    int asArray[GF_GFX_RES_TYPE_MAX];
    struct {
        int charCapacity;
        int plttCapacity;
        int cellCapacity;
        int animCapacity;
        int mcellCapacity;
        int manimCapacity;
    } asStruct;
} SpriteResourceCapacities;

void FieldSpriteManager_InitEmptyResLists(SpriteResourceManager *spriteManager, SpriteResourceCapacities *capacities, u32 maxElements, enum HeapID heapID);

typedef struct PokemonPreview {
    SpriteResourceManager spriteManager;
    ManagedSprite *managedSprite;
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 x;
    u8 y;
    u8 state;
} PokemonPreview;

u8 *DrawPokemonPreview(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, u16 species, u8 gender, enum HeapID heapID);

u8 *DrawPokemonPreviewFromStruct(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, Pokemon *mon, enum HeapID heapID);

void sub_0200F600(PokemonPreview *preview, u32 heapID);

void sub_0200F600(PokemonPreview *preview, u32 heapID) {
    SpriteResourceCapacities v0 = { 1, 1, 1, 1, 0, 0 };
    FieldSpriteManager_InitEmptyResLists(&preview->spriteManager, (union SpriteResourceCapacities *)(&v0), (HeapID)(1), (HeapID)(heapID));
}
