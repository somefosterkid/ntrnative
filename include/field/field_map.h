// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_map.h, the lines in use so far.
#ifndef BWPORT_FIELD_FIELD_MAP_H
#define BWPORT_FIELD_FIELD_MAP_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// The terrain of a layer at a position. Layout from swan, which names the normal's components SlopeX, HeightDiv and
// SlopeZ
struct MapTerrainBuf {
    VecFx16 normal;
    u16 pad06;
    u32 tileType;
    fx32 height;
};

// The terrain at a position in a chunk, a layer for each height. Layout from swan
struct MapTerrainSamplerOutput {
    MapTerrainBuf layers[16];
    u32 layerCount;
};

u32 GetTileFlags(u32 tileType);
// The state of the zone's gimmick that the save keeps. The ID is not checked
void *GimmickState_GetUserData(GimmickState *gimmick, u32 gimmickId);
u16 GetAbyssalRuinsDiveZoneID(Field *field, u16 *zoneId);
u16 GetTileClass(u32 tileType);
BOOL MapTile_IsSurfEdge(u32 tileClass);
BOOL IsTileSurfWater(u16 tileClass);

void MapMatrix_Load(MapMatrix *matrix, u16 matrixId, u16 zoneId, HeapID heapId);
u32 *GetMapMatrixChunkIDs(MapMatrix *matrix);
MapReplace *MapReplace_Create(HeapID heapId, GameSystem *gsys);
s32 MapReplace_LoadEntry(MapReplace *replace, u32 index);
void MapReplace_Free(MapReplace *replace);
void MapReplace_Patch(MapMatrix *matrix, MapReplace *replace, HeapID heapId);
u32 MapReplace_ResolvePatch(MapReplace *replace, u32 *oldValue, u32 *newValue);
int MapReplace_GetEventByCond(u8 condition);
const MapReplaceEvent *MapReplace_GetEventByUID(GameData *gameData, u16 uid);
void GameData_SetEventMapReplace(GameData *gameData, u16 uid, BOOL set);
void MapMatrix_Patch(MapMatrix *matrix, GameSystem *gsys, HeapID heapId);
u32 GetTileTypeAtPos(FieldG3DMapper *mapper, const VecFx32 *position);
BOOL MapTile_IsValid(u32 tileType);
BOOL MapTile_BlocksCollision(u32 tileType);
BOOL MapTile_IsNormalTallGrassDoubleBtl(u32 tileClass);
BOOL MapTile_IsReallyTallGrassSingleBtl(u32 tileClass);
BOOL MapTile_IsReallyTallGrassDoubleBtl(u32 tileClass);
BOOL MapTile_IsSnowNoCycling(u32 tileClass);
BOOL MapTile_IsElectricField(u32 tileClass);
BOOL MapTile_IsDeepSand(u32 tileClass);
// The rest of map_attr.c
BOOL MapTile_IsLedgeU(u32 tileClass);
BOOL MapTile_IsLedgeD(u32 tileClass);
BOOL MapTile_IsLedgeL(u32 tileClass);
BOOL MapTile_IsLedgeR(u32 tileClass);
BOOL MapTile_IsStrengthHole(u32 tileClass);
BOOL IsTileWaterfall(u32 tileClass);
BOOL IsTileSlidingIce(u32 tileClass);
BOOL IsTileSand(u32 tileClass);

// An entry of the map replacements, file 0 of archive 10
typedef struct {
    u16 id;
    u8 kind;
    // What picks the value: the season, the version, both, or an event of EVENT_MAP_REPLACE_TABLE
    u8 condition;
    // The original value, then the replacements
    u16 values[6];
} MapReplaceEntry;

// The values that MapReplaceEntry.condition picks from
struct MapReplaceVariables {
    u8 season;
    // 0 in Black and Black 2, 1 in White and White 2
    u8 version;
    // 0 in Black and Black 2, the season plus 1 in White and White 2
    u8 versionSeason;
    // Whether each event of EVENT_MAP_REPLACE_TABLE happened
    u8 events[10];
};

struct MapReplace {
    u16 heapId;
    ArcTool *arc;
    u32 entryCount;
    MapReplaceVariables variables;
    MapReplaceEntry entry;
};

// Black 2's; twinned code confirms the offsets of workId and expectedValue in Black
struct MapReplaceEvent {
    u8 condition;
    u8 uid;
    u16 workId;
    u16 expectedValue;
};

BOOL GameData_IsMapReplaceEventSet(GameData *gameData, u16 uid);

BOOL MapTile_IsElectricRock(u32 tileClass);

BOOL MapTile_IsReallyTallGrass(u16 tileClass);
BOOL MapTile_IsSwamp(u32 tileClass);
// Whether a tile class keeps an actor from leaving it in a direction
BOOL TileExitBlockCheck_Up(u16 tileClass);
BOOL TileExitBlockCheck_Down(u16 tileClass);
BOOL TileExitBlockCheck_Left(u16 tileClass);
BOOL TileExitBlockCheck_Right(u16 tileClass);
BOOL MapTile_IsQuicksand(u32 tileClass);
BOOL MapTile_IsReceptionCounter(u32 tileClass);
BOOL MapTile_IsPC(u32 tileClass);
BOOL MapTile_IsUnused0xD7(u32 tileClass);
BOOL MapTile_IsTV(u32 tileClass);
BOOL MapTile_IsVendingMachine(u32 tileClass);
BOOL MapTile_IsBookcaseA(u32 tileClass);
BOOL MapTile_IsBookcaseB(u32 tileClass);
BOOL MapTile_IsBookcaseC(u32 tileClass);
BOOL MapTile_IsBookcaseD(u32 tileClass);
BOOL MapTile_IsTrashCanA(u32 tileClass);
BOOL MapTile_IsTrashCanB(u32 tileClass);
BOOL MapTile_IsAssortmentA(u32 tileClass);
BOOL MapTile_IsAssortmentB(u32 tileClass);
BOOL MapTile_IsAssortmentC(u32 tileClass);
BOOL MapTile_IsSnow_(u32 tileClass);
BOOL MapTile_IsCatwalkBody_(u32 tileClass);
BOOL MapTile_IsCatwalkEntryPoint(u32 tileClass);
BOOL CheckSurfBeginTiles(u32 tileType, u32 frontTileType);
BOOL func_ov021_021ab304(u32 tileClass);
BOOL func_ov021_021ab358(u32 tileClass);
BOOL func_ov021_021ab390(u32 tileClass);
BOOL func_ov021_021ab3d4(u32 tileClass);
BOOL func_ov021_021ab3e0(u32 tileClass);
BOOL func_ov021_021ab428(u32 tileClass);
BOOL func_ov021_021ab434(u32 tileClass);
BOOL func_ov021_021ab440(u32 tileClass);
BOOL func_ov021_021ab4ec(u32 tileClass);
BOOL func_ov021_021ab4f8(u32 tileClass);
u32 func_ov021_021ab504(u32 tileType);

u32 GetZoneIDAtMatrixXZWorld(MapMatrix *matrix, s32 x, s32 z);

// Black 2's; twinned code confirms the offsets of zoneId, matrixId, width, height, chunkIdCount and chunkIds in Black
struct MapMatrix {
    u16 heapId;
    u16 zoneId;
    u32 matrixId;
    u16 width;
    u16 height;
    u16 format;
    u16 unk0E;
    u32 chunkIdCount;
    u16 zoneIds[900];
    u32 chunkIds[900];
};

// Black 2's, not checked against Black
struct MapMatrixFileHeader {
    u16 format;
    u16 unk02;
    u16 width;
    u16 height;
    // The chunks, then the zones if the format is 1
    u32 chunkIds[];
};

u16 GetZoneIDAtMatrixXZ(MapMatrix *matrix, s32 x, s32 z);
BOOL RangeCheckChunkCoordinate(MapMatrix *matrix, s32 x, s32 z);
u32 GetChunkCoordOfWorld(s32 coordinate);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 GimmickState_GetID(GimmickState *gimmick);
void ProcessMapMatrix(MapMatrix *matrix, const MapMatrixFileHeader *data, u32 matrixId, u16 zoneId);
u16 GetMapMatrixWidth(MapMatrix *matrix);
u16 GetMapMatrixHeight(MapMatrix *matrix);
u32 GetMapMatrixChunkIDCount(MapMatrix *matrix);
s32 MapReplace_GetEntryCount(MapReplace *replace);
void MapReplace_LoadVariables(MapReplaceVariables *variables, GameSystem *gsys);
// The battle terrain of the tile class
u32 GetTileEncountType(u32 tileClass);
BOOL MapTile_IsSnow(u32 tileClass);
BOOL func_ov021_021bb1a0(u32 tileClass);
BOOL MapTile_IsBlocksCycling(u16 tileClass);
BOOL MapTile_IsTallGrassDoubleBtl(u32 tileClass);
BOOL MapTile_IsTallGrass(u32 tileClass);
BOOL func_ov021_021ab2f8(u32 tileClass);
int RangeCheckChunkCoordinateWorld(MapMatrix *matrix, s32 x, s32 z);
MapMatrix *InitMapMatrix(HeapID heapId);
void FreeMapMatrix(void *ptr);

// Patches of map land data (land_data_patch.c and ov021_021c93d0.c): a file of archive 0x9c that starts with the
// offsets of its cell grid and its buildings, of which the White Forest and Black City are built. pokebw2's types
// (its src/ov036/land_data_patch.c, which holds both of Black's files)

// A map cell
typedef struct LandPatchCell {
    u8 data[0x18];
} LandPatchCell;

// A grid of cells
typedef struct LandPatchGrid {
    u16 width;
    u16 height;
    LandPatchCell cells[1];
} LandPatchGrid;

typedef struct LandDataPatch {
    u8 *data;
} LandDataPatch;

// pokebw2's ReadLandDataPatchA154Data, FreeLandDataPatch, LandDataPatch_OverwriteAttr (copies height rows of width
// cells, from srcX and srcY in the patch to destX and destY in the map) and LoadLandDataPatchBuildings (adds the
// patch's buildings at x and y, after the count already added, and returns the new count)
LandDataPatch *func_ov021_021c939c(u32 fileId, HeapID heapId);
void func_ov021_021c93d0(LandDataPatch *patch);
void func_ov021_021c93e4(LandDataPatch *patch, LandPatchGrid *map, u32 srcX, u32 srcY, u32 destX, u32 destY,
                         u32 width, u32 height);
s32 func_ov021_021c9450(LandDataPatch *patch, FieldPropSystem *propSystem, FieldChunk *chunk, s32 count, s32 x,
                        s32 y);

#endif // BWPORT_FIELD_FIELD_MAP_H
