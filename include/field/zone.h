// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/zone.h, the lines in use so far.
#ifndef BWPORT_FIELD_ZONE_H
#define BWPORT_FIELD_ZONE_H

// Names, layouts and constants from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

struct RailPosition {
    u16 componentId;
    // Black copies the two bytes as one halfword
    union {
        struct {
            u8 componentIsLine;
            u8 railDirection;
        };
        u16 lineAndDirection;
    };
    s16 posSide;
    u16 posFront;
};

typedef union {
    VecFx32 vec;
    RailPosition rail;
} FieldPosition;

struct ZoneSpawnInfo {
    u32 changeType;
    s16 zoneId;
    s16 warpId;
    s16 warpDir;
    u16 posWeightBits;
    BOOL isRail;
    FieldPosition pos;
};

// Black's zone header: one record of the zone data archive (0x30 bytes). pokebw2's ZoneHeader (src/system/zonedata.c),
// whose layout Black shares; matching code confirms npcInfoCacheIdx, matrixId, encId, entitiesId, parentZone,
// placeNameId, locNameDispType, mapTransitionEffectType and battleBGId
typedef struct ZoneData ZoneData;
struct ZoneData {
    u8 mapType;
    u8 npcInfoCacheIdx;
    u16 areaId;
    u16 matrixId;
    u16 scriptDatId;
    u16 unk08;
    u16 textDatId;
    u16 bgmSpring;
    u16 bgmSummer;
    u16 bgmAutumn;
    u16 bgmWinter;
    u16 encId;
    u16 entitiesId;
    u16 parentZone;
    u16 placeNameId : 10;
    u16 locNameDispType : 6;
    u16 weather : 6;
    u16 objectProjectionMatrixType : 3;
    u16 defaultCameraIndex : 7;
    u16 mapTransitionEffectType : 5;
    u16 battleBGId : 5;
    u16 enableCycling : 1;
    u16 unk1E_11 : 1;
    u16 enableEscapeRope : 1;
    u16 enableFlyFrom : 1;
    u16 enableCycleSurfBGM : 1;
    u16 enableEntralinkWarp : 1;
    u16 matrixCamBoundIdx;
    u16 nameIcon : 13;
    u16 unk22_13 : 3;
    s32 flyX;
    s32 flyY;
    s32 flyZ;
};

// An entry of the fog, light and flash tables (pokebw2's ZoneValue)
typedef struct {
    u16 zoneId;
    u16 value;
} ZoneValue;

// Black has two version-specific zones (Black City and its gate), Black 2 three
#define VERSION_SPECIFIC_ZONE_COUNT 2

// The zone data system (pokebw2's ZoneDataSystem): the loaded zone header and the tables of the zones' fog, static
// light and flash
typedef struct ZoneDataSystem {
    ArcTool *arc;
    ZoneData header;
    u16 headerZoneId;
    ZoneValue *fogTable;
    ZoneValue *lightTable;
    u16 fogCount;
    u8 lightCount;
    u8 flashCount;
    ZoneValue *flashTable;
    u8 allowVersionSpecificZone[VERSION_SPECIFIC_ZONE_COUNT];
    u8 allowVersionSpecificArea[VERSION_SPECIFIC_ZONE_COUNT];
} ZoneDataSystem;

// pokebw2's g_ZoneDataSystem
extern ZoneDataSystem *data_02146264;

// No fog or static light for the zone
#define ZONE_FOG_NONE 0x0fffffff
#define ZONE_LIGHT_NONE 0x0fffffff
// No rail or camera for the zone
#define ZONE_RAIL_NONE 0xffffffff
#define ZONE_CAMERA_NONE 0xffff

// The zone that stands for zoneId in this version (pokebw2's GetVersionedMapChangeZoneNum)
u16 GetVersionedMapChangeZoneNum(u16 zoneId);
// The zone's header, loaded if it isn't the one kept (pokebw2's ZoneData_GetHeader)
ZoneData *ZoneData_GetHeader(u16 zoneId);
// Whether the versioned zone is in the list (pokebw2's IsZoneIDArrayContainsZoneID)
BOOL IsZoneIDArrayContainsZoneID(u16 zoneId, const u16 *zones, int count);

// pokebw2's GetZoneHasRailSystem
BOOL func_02013c80(u16 zoneId);
// pokebw2's IsZoneLEQ76: the Battle Subway's two zones
BOOL IsZoneLEQ76(u16 zoneId);
// pokebw2's IsZoneRoyalUnova
BOOL IsZoneRoyalUnova(u16 zoneId);
// pokebw2's IsZoneEntralinkHub
BOOL IsZoneEntralinkHub(u16 zoneId);
// pokebw2's GetZoneIsEntreeForest
BOOL GetZoneIsEntreeForest(u16 zoneId);
// pokebw2's IsZoneEntralinkEdgeColorTable
BOOL IsZoneEntralinkEdgeColorTable(u16 zoneId);
// pokebw2's IsZoneBlackCityOrWhiteForestLobby
BOOL func_02013dd0(u16 zoneId);
// pokebw2's IsZoneBlackCityOrWhiteForest
BOOL func_02013de0(u16 zoneId);
// pokebw2's GetZoneIsMusicalTheater
BOOL func_02013df0(u16 zoneId);
// pokebw2's func_02018c38: whether the zone allows the field's communication and item features
BOOL func_02013e04(u16 zoneId);
// pokebw2's GetZoneParentZone
u16 func_02013e40(u16 zoneId);
// pokebw2's GetZoneLocNameDispType
u32 func_02013e6c(u16 zoneId);
// Whether two zones share a matrix
BOOL func_02013e90(u16 zoneA, u16 zoneB);
// pokebw2's GetZoneFogIndex
u32 func_02013eec(u16 zoneId);
// pokebw2's GetCameraIDForZone
u16 func_02013fa8(u16 zoneId);
// pokebw2's GetMapTransitionEffectType
u32 func_02013fd8(u16 zoneId);
BOOL func_0201400c(u16 zoneId);
BOOL func_02014018(u16 zoneId);
// pokebw2's GetZoneFlagsEnableFlyFrom and GetZoneFlagsEnableEscapeRope
BOOL func_02013c40(u16 zoneId);
BOOL func_02013c50(u16 zoneId);
u8 GetZoneNPCInfoCacheIdx(u16 zoneId);
// Whether the zone's parent is Victory Road
BOOL func_02014068(u16 zoneId);

u16 ZoneData_GetPlaceNameID(u16 zoneId);
void LoadZoneSpawnInfoCheckRail(ZoneSpawnInfo *spawn, u16 zoneId);
BOOL IsReturnLocationNonLeaguePokeCen(GameData *gameData);
BOOL IsZone150Or151(u16 zoneId);
u16 GetZoneEncID(u16 zoneId);

s16 GetDirectionVectorCompX(u32 direction);
u16 GetInverseDirection(u32 direction);
u16 GetDirFromPosToPos(s32 x1, s32 z1, s32 x2, s32 z2);
BOOL CheckTriggerPositionMatchRail(const ZoneTrigger *trigger, const RailPosition *position);
BOOL CheckTriggerPositionMatchXYZ(const ZoneTrigger *trigger, const VecFx32 *position);
u32 ZoneWarp_GetDirection(const ZoneWarp *warp);
u32 GetActualRespawnZoneIdx(u32 index);
void CreateRespawnZoneChangeData(ZoneSpawnInfo *spawn, u16 zoneId, u32 unused, u16 x, u16 z);
ZoneWarp *GetZoneWarpByID(EventData *eventData, u16 warpId);
void SetupTeleportZoneChange(u16 returnLocation, ZoneSpawnInfo *spawn);

// Black 2's; matching code confirms the offsets of discoveryFlagId, canReturnHere, discoverOnVisit, zoneId, x, z and
// mainZoneId, and its size in Black
struct RespawnZoneInfo {
    u16 discoveryFlagId;
    u16 canReturnHere : 1;
    u16 discoverOnVisit : 1;
    u16 unkFlags : 14;
    u16 zoneId;
    u16 x : 8;
    u16 z : 8;
    u16 mainZoneId;
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    u16 unk10;
    u16 unk12;
};

u16 GetRespawnZoneMainZone(u16 index);

BOOL GetIsZoneMatrix0(u16 zoneId);

u32 GetZoneStaticLightDataIndex(u16 zoneId);

void FreeZoneDataSystem(void);
// pokebw2's IsZoneGameCommDisabled
BOOL func_02013d34(u16 zoneId);

// Spawns at a position instead of a warp, warpId is -1
#define ZONE_SPAWN_CHANGE_TYPE_POSITION 1

#define WARP_DIR_UP 1
#define WARP_DIR_DOWN 2
#define WARP_DIR_LEFT 3
#define WARP_DIR_RIGHT 4

u16 ConvDirToWarpDir(u16 dir);
void InitZoneSpawnInfo(ZoneSpawnInfo *spawn);
void SetupZoneWarpArrivalGrid(ZoneSpawnInfo *spawn, u16 zoneId, s16 warpId, s16 warpDir, u16 posWeightBits, s32 x,
                              s32 y, s32 z);
void SetupZoneWarpArrivalRail(ZoneSpawnInfo *spawn, u16 zoneId, s16 warpId, s16 warpDir, u16 posWeightBits,
                              u16 componentId, u16 posFront, s16 posSide);
// The position Fly lands at
void LoadZoneFlyPosToVecFx32(u16 zoneId, VecFx32 *position);
void LoadZoneFlyPosRailCoords(u16 zoneId, VecFx32 *position);
void GetPlayerZoneStateWPos(const ZoneSpawnInfo *spawn, VecFx32 *pos);
void SetPlayerZoneStateWPos(ZoneSpawnInfo *spawn, const VecFx32 *pos);
// pokebw2's CreateZoneChangeDataRail
void func_020141a8(ZoneSpawnInfo *spawn, u16 zoneId, s16 warpDir, u16 componentId, u16 posFront,
                              s16 posSide);

void VecGPosToWPos(s32 x, s32 y, s32 z, VecFx32 *position);
u8 func_ov010_021642d4(u32 direction, u16 angle);

// Black 2's, not checked against Black
#define ZONE_SPAWN_CHANGE_TYPE_3 3
// With a map replace event set, each version allows its own area
#define VERSION_AREA_BLACK2 0

// Black 2's, not checked against Black
#define VERSION_AREA_WHITE2 1
#define VERSION_AREA_2 2

void SetupWarpParamByWarp(ZoneWarp *warp, ZoneSpawnInfo *spawn, u32 direction);
BOOL IsWarpDestId256(ZoneWarp *warp);

void GetGridWarpOutPos(ZoneWarp *warp, u32 direction, VecFx32 *position);
void GetRailWarpOutPos(const ZoneWarp *warp, u32 direction, RailPosition *position);

// Black 2's; twinned code confirms the offsets of x, y, z, width and height in Black
struct ZoneWarpGridPosition {
    s16 x;
    s16 y;
    s16 z;
    u16 width;
    u16 height;
    u16 unk0a;
};

struct ZoneWarpRailPosition {
    u16 componentId;
    u16 posFront;
    s16 posSide;
    u16 frontSpan;
    u16 sideSpan;
    u16 param;
};

// Black 2's; twinned code confirms the offsets of isRail and pos, and its size in Black
// Field names from swan's ZoneFurniture.
struct ZoneBGEntity {
    u16 scrId;
    u16 condition;
    u16 direction;
    u16 isRail;
    union {
        struct {
            s32 x;
            s32 y;
            s32 z;
        } grid;
        struct {
            u16 componentId;
            u16 posFront;
            s16 posSide;
        } rail;
    } pos;
};

// Black 2's; twinned code confirms the offsets of scrId, workValue, workId, type, isRail and pos in Black
// Field names from swan.
struct ZoneTrigger {
    u16 scrId;
    u16 workValue;
    u16 workId;
    u16 type;
    u16 isRail;
    union {
        struct {
            u16 x;
            u16 z;
            u16 width;
            u16 height;
            s16 y;
        } grid;
        struct {
            u16 componentId;
            u16 posFront;
            s16 posSide;
            u16 frontSpan;
            u16 sideSpan;
        } rail;
    } pos;
    u16 unk14;
};

// Black 2's; twinned code confirms the offsets of unk0, destId, unk4, isRail and pos, and its size in Black
struct ZoneWarp {
    u16 unk0;
    u16 destId;
    u8 unk4;
    u8 transitionType;
    u16 isRail;
    union {
        ZoneWarpGridPosition grid;
        ZoneWarpRailPosition rail;
    } pos;
};

u32 ConvDirToTriggerDir(u32 dir);
BOOL func_ov010_02162d2c(const ZoneWarp *warp, const VecFx32 *position);
BOOL func_ov010_02162e00(const ZoneWarp *warp, const RailPosition *position);
BOOL func_ov010_02162e9c(const ZoneBGEntity *entity, const RailPosition *position);
BOOL func_ov010_02162e48(const ZoneBGEntity *entity, const VecFx32 *position);
void SetBGEntityLocation(EventData *data, u32 index, u16 x, u16 z, u16 y);
u16 ZoneWarp_CalcPosWeightBitsGrid_(ZoneWarp *warp, const VecFx32 *position);
void func_ov010_02162ecc(const ZoneTrigger *trigger, VecFx32 *position);
BOOL CheckTriggerPositionMatchXZ(const ZoneTrigger *trigger, const VecFx32 *position);
BOOL CheckWarpDirectionMatch(const ZoneWarp *warp, u16 direction);
BOOL IsWarpZoneOrWarpID0xFFFF(const ZoneWarp *warp);
void SetZoneWarpLocation(EventData *eventData, u16 warpId, u16 x, u16 y, u16 z);
ZoneTrigger *FindCollidingZoneTriggerAtLocation(EventData *eventData, EventWork *eventWork, const VecFx32 *position);
ZoneTrigger *FindTriggerAtPosRail(EventData *eventData, EventWork *eventWork, const RailPosition *position);
u16 func_ov010_02162bcc(ZoneWarp *warp, const RailPosition *position);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 ConvDirToRailDir(u32 direction);
s16 GetDirectionVectorCompZ();
void GetTriggerCenterPos(const ZoneTrigger *trigger, VecFx32 *position);
u16 CalcWarpTransferAddend(u32 posWeightBits, u32 warpDirection, BOOL isRail, u32 railParam, u16 span);
void CreateZoneChangeData(ZoneSpawnInfo *spawn, u32 zoneId, s16 warpDir, s32 x, s32 y, s32 z);
void SetupZoneSpawnInfoWarp(ZoneSpawnInfo *spawn, u16 zoneId, u16 warpId, u32 direction);
u32 GetOutTransitionTypeBetweenZones(u16 fromZone, u16 toZone);
u32 GetLeaguePokeCenReturnLocationIdx(void);
BOOL RangeCheckTeleportZone(s32 index);
u32 GetWarpTransitionType(ZoneWarp *warp);
BOOL GetZoneIsEntralinkAny(u16 zoneId);
ZoneTrigger *FindTriggerAtPosGrid(EventData *eventData, EventWork *eventWork, const VecFx32 *position, u32 direction);
u32 GetZoneFlashFlags(u16 zoneId);
BOOL GetZoneIsUnionRoom(u16 zoneId);
u16 GetZoneMatrixId(u16 zoneId);
BOOL IsZoneAbyssalRuinsOutside(u16 zoneId);
BOOL IsZoneNimbasaStadium(u16 zoneId);
// Whether a place name is one of the Entralink's
BOOL IsMetLocationEntralink(u16 location);
BOOL GetIsZoneRoute14Or15(u16 zoneId);
BOOL SetupZoneWarpArrival(const EventData *eventData, ZoneSpawnInfo *spawn, u16 warpId, u16 posWeightBits);
u16 ZoneData_GetAreaID(u16 zoneId);
u16 GetZoneEntitiesID(u16 zoneId);
u32 GetZoneBattleBGID(u16 zoneId);
u16 GetZoneMatrixCamBoundIdx(u16 zoneId);
// The zone's BGM for the season
u16 DecideZoneHeaderBGMID(u16 zoneId, u8 season);
// Whether the cycling and surfing BGM play in the zone
BOOL GetZoneFlagsEnableCycleSurfBGM(u16 zoneId);
u16 GetZoneEnvFlagsWeather(u16 zoneId);
u32 GetRespawnLocationIndexForRespawnZone(s32 zoneId);
void SetTeleportZoneDiscover(GameData *gameData, s32 zoneId);
u32 GetRailIDForZone(u16 zoneId);
BOOL GetZoneSpawnInfoIsRail(ZoneSpawnInfo *spawn);
void LoadAspertiaCitySpawnInfo(ZoneSpawnInfo *spawn);
// Whether Fly lands on a rail in the zone
BOOL IsZonesWithRailsArrayHasZoneID(u16 zoneId);
BOOL IsZoneAbyssalRuinsInside(u16 zoneId);
u16 ZoneWarp_CalcPosWeightBitsGrid(ZoneWarp *warp, const VecFx32 *position);
u16 func_ov010_021626b0(ZoneWarp *warp, const RailPosition *position);

#endif // BWPORT_FIELD_ZONE_H
