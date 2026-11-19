#ifndef POKEHEARTGOLD_OVERLAY_01_021F4704_H
#define POKEHEARTGOLD_OVERLAY_01_021F4704_H

#include "field/area_data.h"
#include "filesystem.h"
#include "map_matrix.h"
#include "overlay_01_map_load_manager.h"
#include "sys_task.h"

struct FieldSystemUnkSubCC;

// The context of the map model load task (ov01_021F676C allocates 0x30 bytes, ov01_021F6620 is the task).
typedef struct MapModelLoadTask {
    NARC *narc;
    u32 modelNo;
    int modelSize;
    NNSG3dRenderObj *renderObj;
    NNSG3dResFileHeader **modelFile;
    NNSG3dResTex *texture;
    u8 state;
    BOOL *modelLoaded;
    BOOL cancel;
    BOOL *loading;
    struct FieldSystemUnkSubCC *areaMapAnimation;
    int bytesRead;
} MapModelLoadTask;

void ov01_021F477C(MapLoadSlot *slot);
void ov01_021F47A0(int x, int z, int width, int height, u32 stride, int *matrixIndices);
void ov01_021F488C(MapLoadManager *mapLoadManager, BOOL hasPermissionBytes);
void ov01_021F497C(MapLoadManager *mapLoadManager);
void ov01_021F49FC(NARC *narc, u32 fileId, u32 *header);
u16 ov01_021F4A50(u32 *header, MapLoadManager *mapLoadManager, MapMatrix *mapMatrix, LoadedMap *map, s32 matrixIndex, s32 tileCount, s32 arg6);
void ov01_021F4AAC(MapLoadManager *mapLoadManager, LoadedMap *map);
void ov01_021F4AE4(MapLoadManager *mapLoadManager, LoadedMap *map, u32 fileId);
void ov01_021F4B1C(MapLoadManager *mapLoadManager, LoadedMap *map, u32 *header, s32 matrixIndex);
void ov01_021F4BE8(MapLoadManager *mapLoadManager, LoadedMap *map, u32 *header, AreaDataManager *areaDataManager, MapLoadSlot *slot, u32 modelNo, BOOL loadBdhc);
void ov01_021F4C6C(MapLoadManager *mapLoadManager, LoadedMap *map, u32 *header, AreaDataManager *areaDataManager, u32 modelNo, int matrixIndex, BOOL arg6, BOOL loadBdhc);
void ov01_021F4D10(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot);
void ov01_021F4D88(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot);
void ov01_021F4E1C(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot);
void ov01_021F4E88(int matrixIndex, int mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, s32 arg6, MapLoadManager *mapLoadManager);
void ov01_021F4F0C(int matrixIndex, int mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, s32 arg6, MapLoadManager *mapLoadManager);
NNSG3dResMdl *ov01_021F67B4(NARC *landDataNARC, const int mapModelFileSize, NNSG3dRenderObj *mapRenderObj, NNSG3dResFileHeader **mapModelFile, NNSG3dResTex *mapTexture);
void ov01_021F4FB0(int matrixIndex, int mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, s32 arg6, MapLoadManager *mapLoadManager);
int ov01_021F5A04(const u32 tileIndex, const int mapMatrixWidth, const int mapMatrixWidthTiles);
u32 ov01_021F5A80(const u8 trackedTargetDirection, const int mapMatrixHeight, const int mapMatrixWidthTiles, const int trackedTargetTileIndex);
u8 ov01_021F5AE8(const u8 direction, const int quadrant);
void ov01_021F5B24(const u8 direction, const int quadrant, u8 *oppositeQuadrant);
u8 ov01_021F5B88(const u8 direction);
s32 ov01_021F5A28(s32 x, s32 z, s32 stride);
u8 ov01_021F6328(u32 tileIndex, int stride);
void ov01_021F5D8C(MapLoadManager *mapLoadManager, int x, int z, u32 stride);
void ov01_021F5E20(MapLoadManager *mapLoadManager, int x, int z, u32 stride);
void ov01_021F5EA0(MapLoadManager *mapLoadManager, int x, int z, u32 stride);
void ov01_021F562C(const int trackedTargetMapMatrixIndex, const u8 trackedTargetDirection, MapLoadManager *mapLoadManager);
void ov01_021F613C(u8 direction, MapLoadManager *mapLoadManager);
s32 ov01_021F652C(MapLoadManager *mapLoadManager, u32 index);
Bdhc *ov01_021F65D0(MapLoadManager *mapLoadManager, u8 mapSlot);
struct SoundplateStruct *ov01_021F6600(MapLoadManager *mapLoadManager, u8 mapSlot);
void ov01_021F6620(SysTask *sysTask, void *sysTaskParam);
SysTask *ov01_021F676C(NARC *narc, u32 modelNo, int modelSize, NNSG3dRenderObj *renderObj, NNSG3dResFileHeader **modelFile, NNSG3dResTex *texture, struct FieldSystemUnkSubCC *areaMapAnimation, BOOL *modelLoaded, BOOL *loading);

#endif // POKEHEARTGOLD_OVERLAY_01_021F4704_H
