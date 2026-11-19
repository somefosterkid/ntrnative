#ifndef POKEHEARTGOLD_FIELD_TERRAIN_COLLISION_MANAGER_H
#define POKEHEARTGOLD_FIELD_TERRAIN_COLLISION_MANAGER_H

#include "global.h"

#include "field_types_def.h"

// What FieldSystem::unk60 points to: the two terrain queries of the loaded map's collision mode (the full model-based pair
// _020FC604 = sub_02054774 / sub_020547D8, or the simple pair _020FC614 = sub_020547A4 / sub_02054824, chosen by
// MapLoadMode::useSimpleTerrainCollisions in sub_0205489C). The tables hold code pointers: they are typed so the PC build
// re-emits them with 8-byte pointers (asm .rodata keeps 4-byte ones).
typedef const fx32 (*GetHeightFunc)(const FieldSystem *, const fx32, const fx32, const fx32, u8 *);

typedef BOOL (*GetTileAttributesFunc)(const FieldSystem *, const int, const int, u16 *);

typedef struct TerrainCollisionManager {
    GetHeightFunc getHeight;
    GetTileAttributesFunc getTileAttributes;
} TerrainCollisionManager;

#endif // POKEHEARTGOLD_FIELD_TERRAIN_COLLISION_MANAGER_H
