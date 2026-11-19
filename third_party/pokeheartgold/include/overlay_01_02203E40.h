#ifndef POKEHEARTGOLD_OVERLAY_01_02203E40_H
#define POKEHEARTGOLD_OVERLAY_01_02203E40_H

#include "overlay_01_021F1348.h"

// The context of the field effect renderer made by ov01_02203E40 (sizeof == 0x68 at 32 bits): the manager first,
// the render object of file 0x6A.
typedef struct UnkOv01_02203E40 {
    FieldEffectManager *fieldEffMan;
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *model;
    NNSG3dResFileHeader *resFile;
    OverworldAnimManager *anim; // the anim manager ov01_02203EA0 makes (NULL while there is none)
    f32 unk_64; // 1.0f from the start, ov01_02203F2C sets it
} UnkOv01_02203E40;

// userData of the ov01_022095B4 anim manager: ov01_02203EA0 builds it on the stack (0x1C bytes at 32 bits).
typedef struct UnkOv01_02203E40_Data {
    FieldEffectManager *fieldEffMan;
    UnkOv01_02203E40 *obj;
    LocalMapObject *mapObject;
    fx32 dist; // a third of the camera to map object distance
    VecFx32 direction; // normalized, map object to camera
} UnkOv01_02203E40_Data;

// The anim managers context (0x20 bytes at 32 bits; word 0 of ov01_022095B4 is its size, so the host table must use
// sizeof(UnkOv01_02203E40_Work)).
typedef struct UnkOv01_02203E40_Work {
    int unk0; // sub_02068D90(manager)
    UnkOv01_02203E40_Data data;
} UnkOv01_02203E40_Work;

void *ov01_02203E40(FieldEffectManager *fieldEffMan);
void ov01_02203E64(void *a0);
void ov01_02203E74(UnkOv01_02203E40 *ctx);
void ov01_02203E94(UnkOv01_02203E40 *ctx);

OverworldAnimManager *ov01_02203EA0(LocalMapObject *mapObj);
void ov01_02203F2C(OverworldAnimManager *animMan, float a0);
BOOL ov01_02203F3C(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work);
void ov01_02203F68(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work);
void ov01_02203F6C(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work);
void ov01_02203F98(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work);

#endif // POKEHEARTGOLD_OVERLAY_01_02203E40_H
