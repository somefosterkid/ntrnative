#ifndef POKEHEARTGOLD_OVERLAY_106_LEGENDARY_CINEMATIC_H
#define POKEHEARTGOLD_OVERLAY_106_LEGENDARY_CINEMATIC_H

#include "camera.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_motion_blur.h"
#include "unk_02097B78.h"


// Camera set-up read by ov106_021E59FC (static tables ov106_021E6E74 / 6E9C / 6EFC).
typedef struct LegendaryCinematicCamera {
    fx32 distance;
    CameraAngle angle;
    u8 perspectiveType;
    u8 unk_0D;
    u16 perspectiveAngle;
    s32 unk_10;
    VecFx32 target;
    fx32 nearClip;
    fx32 farClip;
} LegendaryCinematicCamera; // 0x28

// One row of the model tables read by ov106_021E5AA8.
typedef struct LegendaryCinematicModelDesc {
    s32 modelFileId;
    u8 anmFileIds[4]; // 0xFF = none
    BOOL resetMatColors;
} LegendaryCinematicModelDesc; // 0xC

typedef struct LegendaryCinematicModel {
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *resMdl;
    NNSG3dResFileHeader *fileHeader;
    void *anmData[4];
    NNSG3dAnmObj *anmObj[4];
} LegendaryCinematicModel; // 0x7C

// Data of the legendary-Pokemon cinematic overlay (OverlayManager data, allocated by LegendaryCinematic_Init).
typedef struct LegendaryCinematic {
    LegendaryCinematicArgs *args;
    MotionBlur *capture;
    NNSFndAllocator allocator;
    Camera *camera;
    LegendaryCinematicModel models[8];
    u32 numModels;
    VecFx32 cameraTarget;
    u32 state;
    u32 nextState;
    u32 frame;
    ArceusCinematic *arceus;
} LegendaryCinematic; // 0x41C

#endif // POKEHEARTGOLD_OVERLAY_106_LEGENDARY_CINEMATIC_H
