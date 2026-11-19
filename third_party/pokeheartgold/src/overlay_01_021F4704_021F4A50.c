#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "map_matrix.h"
#include "field/area_data.h"
#include "field/map_prop.h"
#include "player_data.h"
#include "safari_zone.h"
#include "overlay_01_021F4704.h"
#include "overlay_01_021FB04C.h"


u16 ov01_021F4A50(u32 *header, MapLoadManager *mapLoadManager, MapMatrix *map_matrix, LoadedMap *map, s32 map_no, s32 arg5, s32 arg6) {
    u16 v0;

    if (arg6 != 0) {
        if (map->renderObj.anmMat != NULL) {
            GF_AssertFail();
        }
        map->renderObj.anmMat = NULL;
    }
    if ((map_no < 0) || (map_no >= arg5)) {
        return 0xFFFFU;
    }
    v0 = GetMapModelNo((u32)map_no, map_matrix);
    if (v0 != 0xFFFF) {
        ov01_021F49FC(mapLoadManager->narc, v0, header);
    }
    return v0;
}

void ov01_021F4AAC(MapLoadManager *mapLoadManager, LoadedMap *map) {
    u16 size;

    NARC_ReadFile(mapLoadManager->narc, 4U, &map->soundplates.header);
    size = (u16)((s32)map->soundplates.header >> 0x10);
    if (size != 0) {
        NARC_ReadFile(mapLoadManager->narc, (u32)size, map->soundplates.data);
    }
}

void ov01_021F4AE4(MapLoadManager *mapLoadManager, LoadedMap *map, u32 file_id) {
    u16 uVar1;
    s32 iVar2;
    u16 *puVar3;

    NARC_ReadFile(mapLoadManager->narc, file_id, map->permissions);
    if (map->permissionBytes != NULL) {
        iVar2 = 0;
        puVar3 = map->permissions;
        do {
            uVar1 = *puVar3;
            puVar3 = puVar3 + 1;
            map->permissionBytes[iVar2] = (s8)uVar1;
            iVar2 = iVar2 + 1;
        } while (iVar2 < 0x400);
    }
    return;
}

void ov01_021F4B1C(MapLoadManager *mapLoadManager, LoadedMap *map, u32 *header, s32 arg3) {
    s32 area_set_no;
    SafariZone *safari_zone;
    SafariZoneAreaSet *safariZoneAreaSet;
    s32 v1;
    SafariZone *safariZone;
    s32 v2;
    u16 v4;
    u16 v3;
    u8 gender;

    v1 = mapLoadManager->width;
    v4 = (u16)(arg3 % v1);
    v3 = (u16)(arg3 / v1);
    if (((u32)v4 < 1U) || ((u32)v4 > 3U) || ((u32)v3 < 1U) || ((u32)v3 > 2U)) {
        MapPropManager_LoadFromNARC(mapLoadManager->narc, header[3], map->propManager, mapLoadManager->mapPropAnimationManager);
        return;
    }
    safariZone = Save_SafariZone_Get(mapLoadManager->saveData);
    safari_zone = safariZone;
    v2 = (s32)sub_0202F620(safariZone);
    area_set_no = v2;
    safariZoneAreaSet = SafariZone_GetAreaSet(safariZone, area_set_no);
    if (v2 == 0) {
        gender = (u8)PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(mapLoadManager->saveData));
    } else {
        gender = SafariZone_GetLinkLeaderGender(safari_zone);
    }
    MapPropManager_LoadFromSafariZone(mapLoadManager->narc, map->propManager, mapLoadManager->mapPropAnimationManager, &safariZoneAreaSet->areas[(u16)(v4 - 1) + ((u16)(v3 - 1) * 3)], map->permissions, gender);
}

void ov01_021F4BE8(MapLoadManager *mapLoadManager, LoadedMap *map, u32 *header, AreaDataManager *areaDataManager, MapLoadSlot *slot, u32 modelNo, BOOL loadBdhc) {
    NNSG3dResTex *texture;

    slot->tasks.modelLoading = slot->tasks.modelLoading + 1;
    texture = AreaDataManager_GetMapTexture(areaDataManager);
    slot->modelTask = ov01_021F676C(mapLoadManager->narc, modelNo, header[1], &map->renderObj, &map->modelFile, texture, mapLoadManager->areaMapAnimation, &map->modelLoaded, &slot->tasks.modelLoading);
    if (loadBdhc != 0) {
        slot->tasks.bdhcLoading = slot->tasks.bdhcLoading + 1;
        slot->bdhcTask = ov01_021FB308(mapLoadManager->narc, header[2], map->bdhc, &slot->tasks.bdhcLoading, &map->bdhcBuffer, &slot->tasks.modelLoading);
    }
}
