// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_scene_area.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_SCENE_AREA_H
#define BWPORT_FIELD_FIELD_SCENE_AREA_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"

typedef struct FieldSceneArea FieldSceneArea;

void ResetSceneArea(FieldSceneArea *area);
void SetFieldSceneAreaCameraAreaEnable(FieldSceneArea *area, BOOL enable);
void ResetSceneAreaLoader(FieldSceneAreaLoader *loader);

typedef struct CameraArea CameraArea;

void BuildSceneArea(FieldSceneArea *area, CameraArea *cameraData, u32 count, const FieldDynCameraFunctions *funcs);
void LoadCameraDataToSceneAreaLoader(FieldSceneAreaLoader *loader, u32 index, u32 cameraId, HeapID heapId);

// Black 2's, not checked against Black
typedef BOOL (*CameraAreaCollCheck)(FieldSceneArea *area, const CameraArea *cameraArea, const VecFx32 *pos);
typedef void (*CameraAreaCalcFunc)(FieldSceneArea *area, const CameraArea *cameraArea, const VecFx32 *pos);

// Black 2's; twinned code confirms the offsets of gridX, gridZ, gridW, unk06, gridH, pitch1, yaw1, tz1, pitch2, yaw2,
// tz2, forbidDelayManager, targetOffset1, targetOffset2, fov1 and fov2 in Black
// A rectangle of the map with two camera settings, which the camera blends between across it
typedef struct {
    u16 gridX;
    u16 gridZ;
    u16 gridW;
    u16 unk06;
    u16 gridH;
    u16 unk0A;
    u16 pitch1;
    u16 yaw1;
    fx32 tz1;
    u16 pitch2;
    u16 yaw2;
    fx32 tz2;
    BOOL forbidDelayManager;
    VecFx32 targetOffset1;
    VecFx32 targetOffset2;
    u16 fov1;
    u16 fov2;
} CameraAreaRect;

// Black 2's; twinned code confirms the offsets of angleStart, angleEnd, radiusStart, radiusEnd, x, y, z, camTarget and
// camPos in Black
// A ring of the map around a point, which the camera turns around
typedef struct {
    u32 angleStart;
    u32 angleEnd;
    fx32 radiusStart;
    fx32 radiusEnd;
    fx32 x;
    fx32 y;
    fx32 z;
    s32 pitch;
    int tzDist;
    VecFx32 camTarget;
    VecFx32 camPos;
} CameraAreaCircle;

// Black 2's; twinned code confirms the offsets of rect, circle, collCheckFunc, stayFunc, enterFunc and exitFunc, and
// its size in Black
struct CameraArea {
    union {
        CameraAreaRect rect;
        CameraAreaCircle circle;
    };
    int unk3C;
    // The functions that check whether the position is in the area, and run while it stays, enters and leaves
    u16 collCheckFunc;
    u16 stayFunc;
    u16 enterFunc;
    u16 exitFunc;
};

struct FieldDynCameraFunctions {
    CameraAreaCollCheck *collCheckFuncs;
    CameraAreaCalcFunc *calcFuncs;
    u16 collCheckFuncCount;
    u16 calcFuncCount;
};

struct FieldSceneArea {
    FieldCamera *camera;
    Field *field;
    CameraArea *cameraData;
    const FieldDynCameraFunctions *cameraFunctions;
    u32 cameraCount;
    int lastCamIdx;
    BOOL isCameraAreaEnable;
    u32 currentCamDataFlags;
};

FieldSceneArea *FieldSceneArea_Create(u32 heapId, FieldCamera *camera, Field *field);
// The camera area the position is in, or -1
int FieldCameraArea_Update(FieldSceneArea *area, const VecFx32 *pos);

// The loader of the camera areas: each zone's areas come from one file of one of the two archives
struct FieldSceneAreaLoader {
    CameraArea *cameraData;
    u32 cameraCount;
    ArcTool *camArcTools[2];
};

FieldSceneAreaLoader *FieldSceneAreaLoader_Create(HeapID heapId);
void FreeFieldSceneAreaLoader(FieldSceneAreaLoader *loader);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
FieldCamera *GetFieldSceneAreaCamera(FieldSceneArea *area);
Field *GetFieldSceneAreaField(FieldSceneArea *area);
CameraArea *GetFldSceneAreaLoaderCameraData(FieldSceneAreaLoader *loader);
u32 GetFldSceneAreaLoaderCamCount(FieldSceneAreaLoader *loader);
const FieldDynCameraFunctions *GetFldSceneAreaLoaderCamFuncsStaticOffs(FieldSceneAreaLoader *loader);
void FreeFieldSceneArea(FieldSceneArea *area);

#endif // BWPORT_FIELD_FIELD_SCENE_AREA_H
