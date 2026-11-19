#ifndef POKEHEARTGOLD_OVERLAY_01_021EABA8_H
#define POKEHEARTGOLD_OVERLAY_01_021EABA8_H

#include "global.h"
#include "constants/heap.h"

struct Camera;

typedef struct FieldCameraShiftBlock {
    u16 angle;
    u16 unk2;
    VecFx32 offset;
} FieldCameraShiftBlock;

typedef struct FieldCameraShift {
    FieldCameraShiftBlock block;
    u32 frames;
} FieldCameraShift;

// fieldSystem->unk28; 0x34 bytes on the DS
typedef struct FieldCameraState {
    struct Camera *camera;
    u8 duration;
    u8 frame;
    u8 unk6;
    u8 type;
    FieldCameraShiftBlock block;
    u8 unk18[0x10];
    int active;
    u16 unk2C;
    u16 unk2E;
    int cameraType;
} FieldCameraState;

struct FieldCameraState *ov01_021EAC4C(enum HeapID arg0);

s32 ov01_021EAEE0(s32 arg0, s32 arg1, s32 arg2);

#endif // POKEHEARTGOLD_OVERLAY_01_021EABA8_H
