// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_camera.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTLV_CAMERA_H
#define BWPORT_BATTLE_BTLV_CAMERA_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

BtlvCamera *BtlvCamera_Create(TCBManager *tcbManager, HeapID heapId);
// pokebw2's BtlvCamera_SetPosTarget
void func_ov094_021f6edc(BtlvCamera *camera, const VecFx32 *pos, const VecFx32 *target);
// pokebw2's BtlvCamera_Rotate
void func_ov094_021f6f20(BtlvCamera *camera, s32 pitch, s32 yaw);
// pokebw2's BtlvCamera_MoveTo
void func_ov094_021f6f48(BtlvCamera *camera, const VecFx32 *pos, const VecFx32 *target, int frames, int wait,
                       int brakeFrames);
// pokebw2's BtlvCamera_Shake
void func_ov094_021f7000(BtlvCamera *camera, int dir, fx32 amplitude, int unused, int frames, int wait, int count);
// pokebw2's BtlvCamera_GetPosTarget
void func_ov094_021f70f0(BtlvCamera *camera, VecFx32 *pos, VecFx32 *target);
// pokebw2's BtlvCamera_CalcRotatedPos
void func_ov094_021f7108(BtlvCamera *camera, s32 pitch, s32 yaw, VecFx32 *pos, VecFx32 *target);
// pokebw2's BtlvCamera_GetDefaultPosTarget
void func_ov094_021f7208(VecFx32 *pos, VecFx32 *target);
// pokebw2's BtlvCamera_IsMoving
BOOL func_ov094_021f722c(BtlvCamera *camera);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvCamera_Delete
void func_ov094_021f6ec0(BtlvCamera *camera);
// pokebw2's BtlvCamera_Main
void func_ov094_021f6ed4(BtlvCamera *camera);

#endif // BWPORT_BATTLE_BTLV_CAMERA_H
