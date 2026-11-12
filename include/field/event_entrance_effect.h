// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_entrance_effect.h: the lines in
// use so far.
#ifndef BWPORT_FIELD_EVENT_ENTRANCE_EFFECT_H
#define BWPORT_FIELD_EVENT_ENTRANCE_EFFECT_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "field/field_camera.h"

// What the entrance camera plays: the warp's transition, coming out of the entrance or going into it
struct EntranceCameraParam {
    u32 transitionType;
    BOOL isArrival;
    BOOL isDeparture;
};

// One record of the entrance cameras' archive (0x40 bytes), what the camera animation starts or ends at
struct EntranceCameraRecord {
    u32 unk_00;
    s32 pitch;
    s32 yaw;
    s32 distance;
    BOOL hasTarget;
    // In whole units (shifted by 12 into fixed point)
    VecFx32 target;
    VecFx32 offset;
    u32 frames;
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    s32 unk_3C;
};

// The entrance camera (0x70 bytes): plays the camera animation of a warp's entrance
struct EntranceCamera {
    Field *field;
    FieldCamera *camera;
    FieldCameraAnimation animation;
    FieldCameraAnimationController *controller;
    EntranceCameraParam param;
    u32 unk_64;
    BOOL unk_68;
    BOOL unk_6C;
};

// The events of the warp's entrance as the player comes out of it and goes into it
GameEvent *func_ov021_021a7b20(WarpSequence *warp);
GameEvent *func_ov021_021a7e3c(WarpSequence *warp);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
EntranceCamera *func_ov021_021bdc94(Field *field);
void func_ov021_021bdcc8(EntranceCamera *camera, const EntranceCameraParam *param);
BOOL func_ov021_021bdd40(EntranceCamera *camera);
BOOL func_ov021_021bdd48(EntranceCamera *camera);
void func_ov021_021bdd20(EntranceCamera *camera);
void func_ov021_021bdd34(EntranceCamera *camera);
void func_ov021_021bdcb8(EntranceCamera *camera);
// The position in front of the player, where an entrance the player walks into stands
void func_ov021_021a7b08(Field *field, VecFx32 *pos);

#endif // BWPORT_FIELD_EVENT_ENTRANCE_EFFECT_H
