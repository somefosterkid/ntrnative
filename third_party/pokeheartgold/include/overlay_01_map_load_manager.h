#ifndef POKEHEARTGOLD_OVERLAY_01_MAP_LOAD_MANAGER_H
#define POKEHEARTGOLD_OVERLAY_01_MAP_LOAD_MANAGER_H

#include "field_types_def.h" // typedef struct MapLoadManager MapLoadManager
#include "overlay_01_loaded_map.h"

// MapLoadManager: the full struct (sizeof == 0x110 on the DS). It was promoted by tools/hg/promote.py with anonymous
// nested records; hk74 named them (MapLoadSlot, MapLoadQueued, MapLoadTracking, MapLoadFuncs, MapLoadBuffers) after the
// src-local copies in overlay_01_021F4704_021F5D38_021F5FB8_021F6020.c, so every user can reach funcs/stride/maps
// by field instead of raw DS offsets (0xB8, 0xC0, 0xC4, 0xC8, 0xCC, 0xFC).

struct AreaDataManager;
struct FieldSystemUnkSubCC;
struct Field3dRenderObjManager;
struct LoadedMap;
struct MapLoadManager;
struct MapMatrix;
struct MapPropAnimationManager;
struct NARC;
struct SaveData;
struct SysTask;

enum MapLoadState {
    MAP_LOAD_STATE_START_CHUNK,
    MAP_LOAD_STATE_WAIT_CHUNK,
    MAP_LOAD_STATE_IDLE,
    MAP_LOAD_STATE_END,
};

typedef void (*MapLoadedCallback)(void *arg, int matrixIndex, MapPropManager *propManager);

typedef struct MapLoadBuffers {
    void *primary[4];
    void *secondary[4];
} MapLoadBuffers;

typedef struct MapLoadTasks {
    BOOL modelLoading;
    BOOL bdhcLoading;
} MapLoadTasks;

typedef struct MapLoadSlot {
    struct LoadedMap *maps[2];
    struct SysTask *modelTask;
    struct SysTask *bdhcTask;
    MapLoadTasks tasks;
    int matrixIndices[2];
    u8 mapSlots[2];
    u8 numStarted;
    BOOL finished;
    BOOL active;
    u8 direction;
} MapLoadSlot;

typedef struct MapLoadQueued {
    struct LoadedMap *maps[2];
    BOOL queued;
    u8 direction;
    int matrixIndices[2];
    u8 mapSlots[2];
} MapLoadQueued;

typedef struct MapLoadTracking {
    VecFx32 pos;
    VecFx32 *target;
    fx32 *upper;
    fx32 *lower;
    BOOL moving;
    u8 direction;
    BOOL enabled;
} MapLoadTracking;

typedef struct MapLoadFuncs {
    void (*initialLoad)(struct MapLoadManager *mapLoadManager, int x, int z, u32 stride);
    void (*loadMap)(int index, struct AreaDataManager *areaDataManager, struct MapMatrix *mapMatrix, int width, int height, struct MapLoadManager *mapLoadManager, MapLoadSlot *slot);
} MapLoadFuncs;

struct MapLoadManager {
    MapLoadBuffers *buffers;
    MapLoadSlot slots[2];
    MapLoadQueued queued;
    BOOL mapStale[4];
    struct LoadedMap *maps[4];
    u8 numActiveSlots;
    u8 nextSlot;
    u8 currentSlot;
    u32 currentMatrixIndex;
    u32 currentTile;
    u8 currentMapSlot;
    u8 quadrant;
    BOOL ended;
    enum MapLoadState state;
    struct AreaDataManager *areaDataManager;
    struct Field3dRenderObjManager *renderObjManager;
    struct MapMatrix *mapMatrix;
    int width;
    int height;
    u32 stride;
    MapLoadTracking tracking;
    struct MapPropAnimationManager *mapPropAnimationManager;
    struct FieldSystemUnkSubCC *areaMapAnimation;
    const MapLoadFuncs *funcs;
    struct NARC *narc;
    struct SaveData *saveData;
    MapLoadedCallback mapLoadedCallback;
    void *mapLoadedCallbackArg;
};

#endif // POKEHEARTGOLD_OVERLAY_01_MAP_LOAD_MANAGER_H
