#ifndef POKEHEARTGOLD_OVERLAY_01_LOADED_MAP_H
#define POKEHEARTGOLD_OVERLAY_01_LOADED_MAP_H

#include "field/map_prop.h" // MapPropManager (and the NitroSystem 3D types)

// LoadedMap: one map chunk as MapLoadManager holds it (maps[4], and the slot / queued maps[2] arrays point at these).
// Heap_Alloc'd by ov01_021F488C with sizeof(LoadedMap) == 0xA74 on the DS (asm: Heap_Alloc(HEAP_ID_FIELD1, 0xa74), then
// 0x854 modelFile, 0x858 bdhcBuffer, 0x85C bdhc, 0x860 matrixIndex, 0x864 modelLoaded, 0x868 propManager, 0x86C soundplates,
// 0xA70 permissionBytes). One header for what five src files carried as local copies (hk79); the asm decided where
// the copies disagreed (the LandDataManager-era copies stopped at propManager, 0x86C).

typedef struct BdhcPoint {
    fx32 x;
    fx32 z;
} BdhcPoint;

typedef struct BdhcPlate {
    u16 firstPoint;
    u16 secondPoint;
    u16 normal;
    u16 constant;
} BdhcPlate;

typedef struct BdhcStrip {
    fx32 lowerBound;
    u16 count;
    u16 offset;
} BdhcStrip;

typedef struct Bdhc {
    BdhcPlate *plates;
    fx32 *constants;
    BdhcStrip *strips;
    u16 *accessList;
    BdhcPoint *points;
    VecFx32 *normals;
    BOOL loaded;
    u32 stripsCount;
} Bdhc;

typedef struct MapSoundplates {
    int header;
    u8 data[0x200];
} MapSoundplates;

typedef struct LoadedMap {
    u16 permissions[32 * 32];
    NNSG3dRenderObj renderObj;
    NNSG3dResFileHeader *modelFile;
    u8 *bdhcBuffer;
    Bdhc *bdhc;
    int matrixIndex;
    BOOL modelLoaded;
    MapPropManager *propManager;
    MapSoundplates soundplates;
    u8 *permissionBytes;
} LoadedMap;

#endif // POKEHEARTGOLD_OVERLAY_01_LOADED_MAP_H
