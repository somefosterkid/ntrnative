#ifndef POKEHEARTGOLD_OVERLAY_01_LAND_DATA_MANAGER_H
#define POKEHEARTGOLD_OVERLAY_01_LAND_DATA_MANAGER_H

// LandDataManager: merged by tools/hg/promote.py from 14 m2c local copies (3 variants, base
// src/overlay_01_021F4704_021F4704.c) and 5 accesses; fields at every accessed offset, pads elsewhere.

struct AreaDataManager;
struct LoadedMap;
struct LoadedMapBuffers;
struct MapLoadFunctions;
struct MapMatrix;
struct MapPropAnimationManager;
struct NARC;
struct SysTask;

typedef struct LandDataManager {
    struct LoadedMapBuffers *loadedMapBufs;
    struct { struct { struct LoadedMap *loadSlots[2]; struct SysTask *mapModelLoadSysTask; struct SysTask *bdhcLoadSysTask; struct { BOOL mapModelTaskRunning; BOOL bdhcTaskRunning; } mapLoadTasksState; int mapMatrixIndexes[2]; u8 quadrants[2]; u8 nextIndexToLoad; u8 unk_24_pad[0x1]; BOOL available; } loader; BOOL loading; u8 loadDirection; u8 unk_2D_tail[0x3]; } mapLazyLoaderMan[2];
    struct { struct LoadedMap *loadSlots[2]; BOOL pending; u8 trackedTargetDirection; u8 unk_10_pad[0x3]; int mapMatrixIndexes[2]; u8 quadrants[2]; u8 unk_1A_tail[0x2]; } lazyLoaderPending;
    int mapQuadrantsToCancelLazyLoad[4];
    struct LoadedMap *loadedMaps[4];
    u8 runningLazyLoaderManCount;
    u8 nextLazyLoaderManIndex;
    u8 activeLazyLoaderManIndex;
    u8 pad_A3[0x1];
    u32 trackedTargetMapMatrixIndex;
    u32 trackedTargetTileIndex;
    u8 trackedTargetLoadedMapsQuadrant;
    u8 trackedTargetMapQuadrant;
    u8 pad_AE[0x2];
    BOOL ending;
    int lazyLoaderSubTask;
    struct AreaDataManager *areaDataMan;
    struct MapMatrix *mapMatrix;
    int mapMatrixWidth;
    int mapMatrixHeight;
    int mapMatrixWidthTiles;
    struct { struct { fx32 x; fx32 y; fx32 z; } prevPosition; void *position; fx32 *higherMovementCoord; fx32 *lowerMovementCoord; BOOL moving; u8 movingDirection; u8 unk_20_pad[0x3]; BOOL tracking; } lazyLoaderTrackedTarget;
    struct MapPropAnimationManager *mapPropAnimMan;
    struct MapLoadFunctions *mapLoadFns;
    struct NARC *landDataNARC;
    void *mapLoadedCb;
    void *mapLoadedCbUserData;
    BOOL inDistortionWorld;
    BOOL skipMapProps;
    int offsetTileX;
    int offsetAltitude;
    int offsetTileZ;
    struct { fx32 x; fx32 y; fx32 z; } offset;
    u8 pad_124[0x4];
} LandDataManager;

// sizeof(LandDataManager) == 0x128

#endif // POKEHEARTGOLD_OVERLAY_01_LAND_DATA_MANAGER_H
