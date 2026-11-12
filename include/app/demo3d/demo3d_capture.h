// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/demo3d/demo3d_capture.h: the lines in
// use so far.
#ifndef BWPORT_APP_DEMO3D_DEMO3D_CAPTURE_H
#define BWPORT_APP_DEMO3D_DEMO3D_CAPTURE_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

typedef struct Demo3DCapture Demo3DCapture;

// pokebw2's Demo3DCapture_Stop
void func_ov203_021be9e8(Demo3DCapture *capture);

// pokebw2's Demo3DCapture_Start
Demo3DCapture *func_ov203_021be9b8(TCBExManager *manager, int eva, int evb);

// Black 2's; twinned code confirms the offsets of eva and evb in Black
struct Demo3DCaptureParam {
    // The display mode that shows the capture, and the BG mode and 3D BG0 with it
    int dispMode;
    int bgMode;
    int bg0As3D;
    int size;
    int mode;
    int sourceA;
    int sourceB;
    int dest;
    // The blend of the new frame and of the captured one, of 16
    int eva;
    int evb;
    HeapID heapId;
};

// Black 2's, not checked against Black
// The display modes that show VRAM A to C, as GX_DISPMODE_VRAM_D shows VRAM D. These belong in nitro/gx.h
#define GX_DISPMODE_VRAM_A 2

// Black 2's, not checked against Black
#define GX_DISPMODE_VRAM_B 6
#define GX_DISPMODE_VRAM_C 10

// pokebw2's Demo3DCapture_Create
Demo3DCapture *func_ov203_021be9f8(TCBExManager *manager, const Demo3DCaptureParam *param);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Demo3DCapture_End
// Ends the capture and goes back to a display mode
void func_ov203_021bea48(Demo3DCapture *capture, int dispMode, int bgMode, int bg0As3D);

#endif // BWPORT_APP_DEMO3D_DEMO3D_CAPTURE_H
