#include "global.h"

#include "field/map_load_manager.h"
#include "field/map_prop.h"

#include "field_types_def.h"
#include "map_matrix.h"

typedef struct TerrainCollisionHitbox TerrainCollisionHitbox;

struct TerrainCollisionHitbox {
    fx32 startX;
    fx32 startZ;
    fx32 endX;
    fx32 endZ;
};

u32 ov01_021F652C(MapLoadManager *, u32);

s32 sub_02054A9C(MapProp *, TerrainCollisionHitbox *, u8 *);

void sub_02054DC8(u32, const int, u8 *);

BOOL sub_02054B74(FieldSystem *fieldSystem, s32 *mapPropModelIDs, u8 mapPropModelIDsCount, TerrainCollisionHitbox *hitbox, MapProp **mapPropOut, s32 *mapPropModelIDOut);

BOOL sub_02054B74(FieldSystem *fieldSystem, s32 *mapPropModelIDs, u8 mapPropModelIDsCount, TerrainCollisionHitbox *hitbox, MapProp **mapPropOut, s32 *mapPropModelIDOut) {
    u8 uVar1;
    u32 uVar2;
    MapProp *mapProp;
    s32 iVar3;
    u32 uVar4;
    u8 index;
    u32 uStack_28;
    MapPropManager *pMStack_24;
    u8 auStack_20[12];

    uStack_28 = 0;
    do {
        ov01_021F630C(uStack_28, fieldSystem->mapLoadManager, &pMStack_24);
        if (pMStack_24 != NULL) {
            uVar2 = ov01_021F652C(fieldSystem->mapLoadManager, uStack_28);
            uVar1 = MapMatrix_GetWidth(fieldSystem->mapMatrix);
            sub_02054DC8(uVar2, uVar1, auStack_20);
            index = 0;
            do {
                mapProp = MapPropManager_GetMapPropByIndex(pMStack_24, index);
                iVar3 = sub_02054A9C(mapProp, hitbox, auStack_20);
                if (iVar3 != 0) {
                    iVar3 = MapProp_GetBuildModel(mapProp);
                    uVar4 = 0;
                    while (uVar4 < mapPropModelIDsCount) {
                        if (iVar3 == mapPropModelIDs[uVar4]) {
                            if (mapPropOut != NULL) {
                                *mapPropOut = mapProp;
                            }
                            if (mapPropModelIDOut != NULL) {
                                *mapPropModelIDOut = iVar3;
                            }
                            return 1;
                        }
                        uVar4 = uVar4 + 1 & 0xff;
                    }
                }
                index = index + 1;
            } while (index < 0x20);
        }
        uStack_28 = uStack_28 + 1 & 0xff;
    } while (uStack_28 < 4);
    return 0;
}
