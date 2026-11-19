#ifndef POKEHEARTGOLD_UNK_02054648_H
#define POKEHEARTGOLD_UNK_02054648_H

#include "field/field_control.h"
#include "field/terrain_collision_manager.h"

#include "script.h"

SoundplateStruct *sub_02054874(FieldSystem *fieldSystem, int x, int z);
void sub_0205489C(const TerrainCollisionManager **a0, int a1);
u8 sub_020548EC(const FieldSystem *fieldSystem, int tileX, int tileZ);
BOOL sub_020548C0(FieldSystem *fieldSystem, int x, int z);
u8 GetMetatileBehavior(FieldSystem *fieldSystem, int x, int z);
BOOL sub_020549A8(FieldSystem *fieldSystem, VecFx32 *playerPos, int xInFront, int yInFront, s8 *verticalDirection);
u32 sub_020549F4(FieldSystem *fieldSystem, VecFx32 *playerPos, u32 x, u32 y, u32 *a4);
BOOL sub_02060BFC(LocalMapObject *playerObj, int xInFront, int playerElev, int yInFront);
void sub_020659CC(LocalMapObject *object);

#endif // POKEHEARTGOLD_UNK_02054648_H
