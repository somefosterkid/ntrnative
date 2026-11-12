// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/ctvt_camera.h: the lines in use
// so far.
#ifndef BWPORT_APP_COMM_TVT_CTVT_CAMERA_H
#define BWPORT_APP_COMM_TVT_CTVT_CAMERA_H

#include "types.h"
#include "struct_decls.h"
#include "twl/camera.h"
#include "gfl/heap.h"

// pokebw2's CtvtCamera_IsSoundDone
// pokebw2's CtvtCamera_Create
CtvtCamera *func_ov172_021b6ee0(CommTvtWork *sys, HeapID heapId);
BOOL func_ov172_021b7590(CommTvtWork *sys, CtvtCamera *work);
// pokebw2's CtvtCamera_StopCamera
void func_ov172_021b7578(CommTvtWork *sys, CtvtCamera *work);
// pokebw2's CtvtCamera_StartRecording
void func_ov172_021b7b64(CommTvtWork *sys, CtvtCamera *work);

// pokebw2's CtvtCamera_EndRecording
void func_ov172_021b7b80(CommTvtWork *sys, CtvtCamera *work);
// pokebw2's CtvtCamera_SetSize
void func_ov172_021b7b3c(CommTvtWork *sys, CtvtCamera *work, CAMERASize size);

// pokebw2's CtvtCamera_Delete
void func_ov172_021b70a8(CommTvtWork *sys, CtvtCamera *work);
// pokebw2's CtvtCamera_Update
void func_ov172_021b712c(CommTvtWork *sys, CtvtCamera *work);

#define CTVT_CAMERA_MEMBERS 4

// Black 2's, not checked against Black
typedef struct {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} CtvtRect;

// Black 2's, not checked against Black
typedef struct {
    BOOL moving;
    BOOL slide;
    BOOL playSE;
    CtvtRect cur;
    CtvtRect target;
} CtvtCameraWindow;

// Black 2's, not checked against Black
struct CtvtCamera {
    HeapID heapId;
    u8 *screen;
    // The pictures for a member without a camera, for the normal and the zoomed screen
    void *noCameraPicture;
    void *noCameraPictureZoomed;
    // The windows to draw again
    u8 dirtyMask;
    BOOL redrawAll;
    BOOL startOpening;
    BOOL updateDisplay;
    BOOL zoomed;
    BOOL recording;
    BOOL recordingEnded;
    u8 redrawFrames;
    u8 openLines;
    BOOL opening;
    CtvtCameraWindow windows[CTVT_CAMERA_MEMBERS];
    CameraSystem *camera;
    void *frameBuffers[2];
    u16 *frameCopy;
};

// pokebw2's CtvtCamera_SetDirty
void func_ov172_021b7714(CommTvtWork *sys, CtvtCamera *work, u8 member);
// pokebw2's CtvtCamera_ClearDirty
void func_ov172_021b7724(CommTvtWork *sys, CtvtCamera *work, u8 member);
// pokebw2's CtvtCamera_Redraw
void func_ov172_021b7740(CommTvtWork *sys, CtvtCamera *work, BOOL fadeIn, BOOL updateDisplay);
// pokebw2's CtvtCamera_IsRedrawing
BOOL func_ov172_021b77b4(CommTvtWork *sys, CtvtCamera *work);
// pokebw2's CtvtCamera_GetWidth
u16 func_ov172_021b77b8(CommTvtWork *sys, CtvtCamera *work);
// pokebw2's CtvtCamera_GetPicture
void *func_ov172_021b7830(CommTvtWork *sys, CtvtCamera *work, u8 member);
// pokebw2's CtvtCamera_GetPictureSize
u32 func_ov172_021b785c(CommTvtWork *sys, CtvtCamera *work);
// pokebw2's CtvtCamera_SetWindow
void func_ov172_021b796c(CommTvtWork *sys, CtvtCamera *work, u8 member);
// pokebw2's CtvtCamera_MarkWindow
void func_ov172_021b7b20(CommTvtWork *sys, CtvtCamera *work, u8 member);
// pokebw2's CtvtCamera_IsWindowMoving
BOOL func_ov172_021b7b30(CommTvtWork *sys, CtvtCamera *work, u8 member);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CtvtCamera_Draw
void func_ov172_021b71b8(CommTvtWork *sys, CtvtCamera *work);
// pokebw2's CtvtCamera_GetHeight
u16 func_ov172_021b77e8(CommTvtWork *sys, CtvtCamera *work);
// pokebw2's CtvtCamera_GetOwnPicture
void *func_ov172_021b7818(CommTvtWork *sys, CtvtCamera *work);

#endif // BWPORT_APP_COMM_TVT_CTVT_CAMERA_H
