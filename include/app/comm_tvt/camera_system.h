// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/camera_system.h: the lines in
// use so far.
#ifndef BWPORT_APP_COMM_TVT_CAMERA_SYSTEM_H
#define BWPORT_APP_COMM_TVT_CAMERA_SYSTEM_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "twl/camera.h"

// pokebw2's CameraSystem_Start
void func_ov172_021b6814(CameraSystem *sys);
// pokebw2's CameraSystem_Stop
void func_ov172_021b6868(CameraSystem *sys);

// Black 2's, not checked against Black
// Gets each captured frame, with the work given to CameraSystem_SetFrameCallback
typedef void (*CameraFrameCallback)(void *frame, void *work);

// pokebw2's CameraSystem_Create
CameraSystem *func_ov172_021b6100(HeapID heapId);
// pokebw2's CameraSystem_Delete
void func_ov172_021b63b8(CameraSystem *sys);
// pokebw2's CameraSystem_SetFrameCallback
void func_ov172_021b6744(CameraSystem *sys, CameraFrameCallback callback, void *work);
// pokebw2's CameraSystem_IsSoundDone
BOOL func_ov172_021b688c(CameraSystem *sys);
// pokebw2's CameraSystem_SetSize
void func_ov172_021b6950(CameraSystem *sys, CAMERASize size);
// pokebw2's CameraSystem_SwitchCamera
void func_ov172_021b69bc(CameraSystem *sys, CAMERASelect camera);
// pokebw2's CameraSystem_PlayVideoStartSound
void func_ov172_021b69d0(CameraSystem *sys);
// pokebw2's CameraSystem_PlayVideoEndSound
void func_ov172_021b69d8(CameraSystem *sys);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CameraSystem_AllocBuffers
void func_ov172_021b674c(CameraSystem *sys, int count, HeapID heapId);
// pokebw2's CameraSystem_UpdateSound
void func_ov172_021b64ac(CameraSystem *sys);

#endif // BWPORT_APP_COMM_TVT_CAMERA_SYSTEM_H
