// Black's zone data: the zone header's loader and its fields, the tests of the zones the code treats specially.
// pokebw2's src/system/zonedata.c (GPL-3.0, https://github.com/fuddlesworth/pokebw2): InitZoneDataSystem passes
// "zonedata.c" to the heap. pokebw2's names of the func_ functions are in the comments

#include "types.h"
#include "constants/version.h"
#include "field/field.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "nitro/fs.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "constants/arc.h"
#include "constants/zones.h"
#include "system/area_data.h"
#include "system/file_util.h"

struct ArcTool {
    FSFile file;
    u32 fimgOffset;
    u16 fatbOffset;
    u16 fileCount;
};

// The rail or the camera of a zone (pokebw2's ZoneAssignment)
typedef struct {
    u16 zoneId : 11;
    u16 id : 5;
} ZoneAssignment;

static BOOL IsZoneGearStationPlatform(u16 zoneId);

// pokebw2's ABYSSAL_RUINS_ZONES_INSIDE
static const u16 data_0209dfb8[4] = { 245, 246, 247, 248 };

// pokebw2's ZONE_CAMERA_ASSIGNMENTS
static const ZoneAssignment data_0209dfc8[5] = {
    { 62, 0 }, { 418, 4 }, { ZONE_ROUTE_4, 3 }, { ZONE_VILLAGE_BRIDGE, 2 }, { 138, 1 },
};

// pokebw2's NIMBASA_STADIUM_ZONES
static const u16 data_0209dfd2[5] = { 81, 82, 83, 86, 87 };

// pokebw2's ABYSSAL_RUINS_ZONES
static const u16 ABYSSAL_RUINS_ZONES[8] = { 241, 242, 243, 244, 245, 246, 247, 248 };

// pokebw2's BC_WF_ZONES: Black City, its Pokemon Center and houses, White Forest, its Pokemon Center and house
static const u16 data_0209dfec[9] = { ZONE_BLACK_CITY, 1, 2, 3, 4, 5, ZONE_WHITE_FOREST, 425, 426 };

// pokebw2's GEAR_STATION_PLATFORM_ZONES: read by no code Black kept, so it is a global as in pokebw2
const u16 data_0209dffe[9] = { 67, 68, 69, 70, 71, 72, 73, 75, 76 };

// pokebw2's ZONES_WITH_RAILS
static const u16 data_0209e010[11] = { ZONE_CASTELIA_CITY, 249, ZONE_VICTORY_ROAD, 209, 211, 66,
                                       241, 242, 243, 244, ZONE_CASTELIA_CITY_8 };

// pokebw2's ZONE_RAIL_ASSIGNMENTS
static const ZoneAssignment data_0209e026[20] = {
    { ZONE_CASTELIA_CITY, 0 },      { 249, 11 },                 { 30, 1 },                   { ZONE_VICTORY_ROAD, 9 },
    { ZONE_CELESTIAL_TOWER_2, 13 }, { 339, 14 },                 { 340, 15 },                 { 341, 16 },
    { 114, 4 },                     { 209, 7 },                  { 211, 8 },                  { 66, 3 },
    { 121, 5 },                     { 241, 10 },                 { 242, 10 },                 { 243, 10 },
    { 244, 10 },                    { ZONE_CASTELIA_CITY_8, 2 }, { ZONE_VILLAGE_BRIDGE, 12 }, { 137, 6 },
};

// The tables, in the ROM's order. The Abyssal Ruins are zones 241 to 248, the four inside ones 245 to 248
// pokebw2's BC_WF_LOBBY_ZONES: Black City, White Forest and their gates
static const u16 data_0209dfc0[4] = { ZONE_WHITE_FOREST, ZONE_BLACK_CITY, 294, 295 };

ZoneDataSystem *data_02146264;

void func_020139b0(HeapID param_1) {
    ArcTool *pAVar1;
    ZoneValue *pZVar2;
    u32 auStack_18 [1];
    u32 uStack_1c;


    if (data_02146264 == NULL) {
        data_02146264 = GFL_HeapAllocate(param_1, sizeof(ZoneDataSystem), 1, "zonedata.c", 0x60);
        pAVar1 = GFL_ArcSysCreateFileHandle(0xc, param_1);
        data_02146264->arc = pAVar1;
        pZVar2 = GFL_ArcSysReadHeapNewLZGetLen(0x51, 0xd, 0, param_1, auStack_18);
        data_02146264->fogTable = pZVar2;
        data_02146264->fogCount = (u16)(auStack_18[0] >> 2);
        pZVar2 = GFL_ArcSysReadHeapNewLZGetLen(0x52, 1, 0, param_1, auStack_18);
        data_02146264->lightTable = pZVar2;
        data_02146264->lightCount = (u8)(auStack_18[0] >> 2);
        pZVar2 = GFL_ArcSysReadHeapNewLZGetLen(0x95, 0, 0, param_1, &uStack_1c);
        data_02146264->flashTable = pZVar2;
        data_02146264->flashCount = (u8)(uStack_1c >> 2);
        data_02146264->headerZoneId = 0x1ab;
#if GAME_VERSION == VERSION_WHITE
        SetAllowVersionSpecificZone(0, 1);
        SetAllowVersionSpecificZone(1, 1);
#endif
    }
}

void FreeZoneDataSystem(void) {
    if (data_02146264 != NULL) {
        GFL_HeapFree(data_02146264->flashTable);
        GFL_HeapFree(data_02146264->lightTable);
        GFL_HeapFree(data_02146264->fogTable);
        GFL_ArcToolFree(data_02146264->arc);
        GFL_HeapFree(data_02146264);
        data_02146264 = NULL;
    }
}

void SetAllowVersionSpecificZone(u32 side, u32 allow) {
    if (data_02146264 != NULL) {
        data_02146264->allowVersionSpecificZone[side] = (u8)allow;
    }
}

typedef struct {
    u16 from;
    u16 to;
} VersionSpecificZone;

// In .data, as pokebw2's (not const): the zone each allowed side changes to
// pokebw2's VERSION_SPECIFIC_ZONES
static VersionSpecificZone data_020a72a8[VERSION_SPECIFIC_ZONE_COUNT] = { { 0, 424 }, { 294, 295 } };
u16 GetVersionedMapChangeZoneNum(u16 zoneId) {
    int i;

    for (i = 0; i < VERSION_SPECIFIC_ZONE_COUNT; i++) {
        if (data_02146264->allowVersionSpecificZone[i] && zoneId == data_020a72a8[i].from) {
            return data_020a72a8[i].to;
        }
    }
    return zoneId;
}


ZoneData *ZoneData_GetHeader(u16 zoneId) {
    u16 mapZone;

    if (data_02146264 == NULL) {
        return NULL;
    }
    mapZone = GetVersionedMapChangeZoneNum(zoneId);
    if ((u32)mapZone == (u32)data_02146264->headerZoneId) {
        return (void *)&data_02146264->header;
    }
    GFL_ArcToolReadRange(data_02146264->arc, 0, (u32)mapZone * 0x30, 0x30, (void *)&data_02146264->header);
    data_02146264->headerZoneId = mapZone;
    return (void *)&data_02146264->header;
}

BOOL IsZoneIDArrayContainsZoneID(u16 zoneId, const u16 *zones, int count) {
    u16 mapZone;
    s32 i;

    mapZone = GetVersionedMapChangeZoneNum(zoneId);
    i = 0;
    if (0 < count) {
        do {
            if (mapZone == zones[i]) {
                return 1;
            }
            i = i + 1;
        } while (i < count);
    }
    return 0;
}

u16 ZoneData_GetAreaID(u16 zoneId) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(zoneId);
    return pZVar1->areaId;
}

u16 GetZoneMatrixId(u16 zoneId) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(zoneId);
    return pZVar1->matrixId;
}

u8 func_02013b50(u16 param_1) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(param_1);
    return pZVar1->mapType;
}

u16 func_02013b5c(u16 param_1) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(param_1);
    return pZVar1->scriptDatId;
}

u16 func_02013b68(u16 param_1) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(param_1);
    return pZVar1->textDatId;
}

void LoadZoneFlyPosToVecFx32(u16 zoneId, VecFx32 *position) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(zoneId);
    position->x = pZVar1->flyX << 0x10;
    position->y = pZVar1->flyY << 0x10;
    position->z = pZVar1->flyZ << 0x10;
}

void LoadZoneFlyPosRailCoords(u16 zoneId, VecFx32 *position) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(zoneId);
    position->x = pZVar1->flyX;
    position->y = pZVar1->flyY;
    position->z = pZVar1->flyZ;
}

u32 ZoneData_GetObjectProjectionMatrixType(u16 zoneId) {
    return (u32) (u8) ((u32) (*(u16 *)((u8 *)(ZoneData_GetHeader(zoneId)) + 0x1C) << 0x17) >> 0x1D);
}

u16 func_02013bbc(u16 param_1) {
    return (u16) (u8) ((u32) (*(u16 *)((u8 *)(ZoneData_GetHeader(param_1)) + 0x1C) << 0x10) >> 0x19);
}

u16 GetZoneMatrixCamBoundIdx(u16 zoneId) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(zoneId);
    return pZVar1->matrixCamBoundIdx;
}

u16 DecideZoneHeaderBGMID(u16 zoneId, u8 season) {
    ZoneData *header;
    u16 bgm;

    bgm = 0;
    header = ZoneData_GetHeader(zoneId);
    switch(season) {
    case 0:
        bgm = header->bgmSpring;
        break;
    case 1:
        bgm = header->bgmSummer;
        break;
    case 2:
        bgm = header->bgmAutumn;
        break;
    case 3:
        bgm = header->bgmWinter;
    }
#if GAME_VERSION == VERSION_WHITE
    if (bgm == 0x409) {
        bgm = 0x40a;
    }
#endif
    return bgm;
}

u16 GetZoneEnvFlagsWeather(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);

    return header->weather;
}


u32 func_02013c20(u16 param_1) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(param_1);
    return pZVar1->enableCycling;
}

BOOL GetZoneFlagsEnableCycleSurfBGM(u16 zoneId) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(zoneId);
    return pZVar1->enableCycleSurfBGM;
}

BOOL func_02013c40(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);

    return header->enableFlyFrom;
}

BOOL func_02013c50(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);

    return header->enableEscapeRope;
}



u16 func_02013c60(u16 param_1) {
    ZoneData *pZVar1;

    pZVar1 = ZoneData_GetHeader(param_1);
    return pZVar1->enableEntralinkWarp;
}

BOOL IsZonesWithRailsArrayHasZoneID(u16 zoneId) {
    return IsZoneIDArrayContainsZoneID(zoneId, data_0209e010, NELEMS(data_0209e010));
}

// pokebw2's GetZoneHasRailSystem
BOOL func_02013c80(u16 zoneId) {
    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    if (GetRailIDForZone(zoneId) != ZONE_RAIL_NONE) {
        return TRUE;
    }
    return FALSE;
}

u32 GetRailIDForZone(u16 zoneId) {
    u32 i;

    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    for (i = 0; i < NELEMS(data_0209e026); i++) {
        if (zoneId == data_0209e026[i].zoneId) {
            return data_0209e026[i].id;
        }
    }
    return ZONE_RAIL_NONE;
}

BOOL GetZoneIsUnionRoom(u16 zoneId) {
    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    if (zoneId == ZONE_UNION_ROOM) {
        return TRUE;
    }
    return FALSE;
}

BOOL IsZone150Or151(u16 zoneId) {
    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    if (zoneId == 150 || zoneId == 151) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IsZoneLEQ76: the Battle Subway's two zones
BOOL IsZoneLEQ76(u16 zoneId) {
    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    if (zoneId == 75 || zoneId == 76) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IsZoneRoyalUnova
BOOL IsZoneRoyalUnova(u16 zoneId) {
    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    if (zoneId == 52) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IsZoneGameCommDisabled
BOOL func_02013d34(u16 zoneId) {
    if (GetZoneIsUnionRoom(zoneId) || IsZone150Or151(zoneId) || IsZoneRoyalUnova(zoneId)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IsZoneEntralinkHub
BOOL IsZoneEntralinkHub(u16 zoneId) {
    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    if (zoneId == 279) {
        return TRUE;
    }
    return FALSE;
}

// The Entralink's gates are zones 289 to 316, the Entree Forest 280 to 288
BOOL GetZoneIsEntralinkAny(u16 zoneId) {
    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    if ((zoneId >= 289 && zoneId <= 316) || GetZoneIsEntreeForest(zoneId) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GetZoneIsEntreeForest
BOOL GetZoneIsEntreeForest(u16 zoneId) {
    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    if (zoneId >= 280 && zoneId <= 288) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IsZoneEntralinkEdgeColorTable
BOOL IsZoneEntralinkEdgeColorTable(u16 zoneId) {
    if (zoneId >= 289 && zoneId <= 316) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IsZoneBlackCityOrWhiteForestLobby
BOOL func_02013dd0(u16 zoneId) {
    return IsZoneIDArrayContainsZoneID(zoneId, data_0209dfc0, NELEMS(data_0209dfc0));
}

// pokebw2's IsZoneBlackCityOrWhiteForest
BOOL func_02013de0(u16 zoneId) {
    return IsZoneIDArrayContainsZoneID(zoneId, data_0209dfec, NELEMS(data_0209dfec));
}

// pokebw2's GetZoneIsMusicalTheater
BOOL func_02013df0(u16 zoneId) {
    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    if (zoneId == 78) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's func_02018c38
BOOL func_02013e04(u16 zoneId) {
    if (GetZoneIsUnionRoom(zoneId) || IsZone150Or151(zoneId) || IsZoneLEQ76(zoneId) || IsZoneEntralinkHub(zoneId) ||
        GetZoneIsEntralinkAny(zoneId)) {
        return FALSE;
    }
    return TRUE;
}

// pokebw2's GetZoneParentZone
u16 func_02013e40(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);
    return header->parentZone;
}

// The gates of Black City and White Forest (place names 0x70 and 0x71) have this version's name
u16 ZoneData_GetPlaceNameID(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);
    u16 placeNameId = header->placeNameId;

    if (placeNameId == 0x70 || placeNameId == 0x71) {
#if GAME_VERSION == VERSION_WHITE
        placeNameId = 0x71;
#else
        placeNameId = 0x70;
#endif
    }
    return placeNameId;
}

// pokebw2's GetZoneLocNameDispType
u32 func_02013e6c(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);
    return header->locNameDispType;
}

BOOL GetIsZoneMatrix0(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);
    if (header->matrixId == 0) {
        return TRUE;
    }
    return FALSE;
}

// Whether two zones share a matrix; matrix 0 and matrix 175 count as one
BOOL func_02013e90(u16 zoneA, u16 zoneB) {
    u16 matrixA = GetZoneMatrixId(zoneA);
    u16 matrixB = GetZoneMatrixId(zoneB);

    if (matrixA == matrixB) {
        return TRUE;
    }
    if ((matrixA == 0 && matrixB == 175) || (matrixA == 175 && matrixB == 0)) {
        return TRUE;
    }
    return FALSE;
}

u16 GetZoneEncID(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);
    return header->encId;
}

u16 GetZoneEntitiesID(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);
    return header->entitiesId;
}

u32 GetZoneBattleBGID(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);
    return (u8)header->battleBGId;
}

// pokebw2's GetZoneFogIndex
u32 func_02013eec(u16 zoneId) {
    u32 index = ZONE_FOG_NONE;
    int i;

    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    for (i = 0; i < data_02146264->fogCount; i++) {
        if (zoneId == data_02146264->fogTable[i].zoneId) {
            index = data_02146264->fogTable[i].value;
            break;
        }
    }
    return index;
}

u32 GetZoneStaticLightDataIndex(u16 zoneId) {
    u32 index = ZONE_LIGHT_NONE;
    int i;

    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    for (i = 0; i < data_02146264->lightCount; i++) {
        if (zoneId == data_02146264->lightTable[i].zoneId) {
            index = data_02146264->lightTable[i].value;
            break;
        }
    }
    return index;
}

u32 GetZoneFlashFlags(u16 zoneId) {
    u32 flags = 0;
    int i;

    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    for (i = 0; i < data_02146264->flashCount; i++) {
        if (zoneId == data_02146264->flashTable[i].zoneId) {
            flags = data_02146264->flashTable[i].value;
            break;
        }
    }
    return flags;
}

// pokebw2's GetCameraIDForZone
u16 func_02013fa8(u16 zoneId) {
    u32 i;

    zoneId = GetVersionedMapChangeZoneNum(zoneId);
    for (i = 0; i < NELEMS(data_0209dfc8); i++) {
        if (zoneId == data_0209dfc8[i].zoneId) {
            return data_0209dfc8[i].id;
        }
    }
    return ZONE_CAMERA_NONE;
}

// pokebw2's GetMapTransitionEffectType
u32 func_02013fd8(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);
    return (u8)header->mapTransitionEffectType;
}

BOOL IsZoneAbyssalRuinsOutside(u16 zoneId) {
    return IsZoneIDArrayContainsZoneID(zoneId, ABYSSAL_RUINS_ZONES, NELEMS(ABYSSAL_RUINS_ZONES));
}

BOOL IsZoneAbyssalRuinsInside(u16 zoneId) {
    return IsZoneIDArrayContainsZoneID(zoneId, data_0209dfb8, NELEMS(data_0209dfb8));
}

// The Abyssal Ruins' second and third zones
BOOL func_0201400c(u16 zoneId) {
    if (zoneId == 246) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_02014018(u16 zoneId) {
    if (zoneId == 247) {
        return TRUE;
    }
    return FALSE;
}

BOOL IsZoneNimbasaStadium(u16 zoneId) {
    return IsZoneIDArrayContainsZoneID(zoneId, data_0209dfd2, NELEMS(data_0209dfd2));
}

static BOOL IsZoneGearStationPlatform(u16 zoneId) {
    return IsZoneIDArrayContainsZoneID(zoneId, data_0209dffe, NELEMS(data_0209dffe));
}

BOOL IsMetLocationEntralink(u16 location) {
    if (location >= 0x4c && location <= 0x69) {
        return TRUE;
    }
    return FALSE;
}

BOOL GetIsZoneRoute14Or15(u16 zoneId) {
    if (zoneId == ZONE_ROUTE_15 || zoneId == ZONE_ROUTE_14) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GetZoneNpcInfoCacheIdx
u8 GetZoneNPCInfoCacheIdx(u16 zoneId) {
    ZoneData *header = ZoneData_GetHeader(zoneId);
    return header->npcInfoCacheIdx;
}

// Whether the zone's parent is Victory Road
BOOL func_02014068(u16 zoneId) {
    if (func_02013e40(zoneId) == ZONE_VICTORY_ROAD) {
        return TRUE;
    }
    return FALSE;
}
