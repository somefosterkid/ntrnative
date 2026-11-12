// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/demo3d/demo3d_engine.h: the lines in use
// so far.
#ifndef BWPORT_APP_DEMO3D_DEMO3D_ENGINE_H
#define BWPORT_APP_DEMO3D_DEMO3D_ENGINE_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "app/demo3d/demo3d_data.h"
#include "app/demo3d/demo3d_graphic.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"

typedef struct Demo3DCmdSys Demo3DCmdSys;

// Black's layout, from the assembly of func_ov203_021be160 (Demo3DEngine_Create), 0x88 bytes
struct Demo3DEngine {
    Demo3DGraphic *graphic;     // 0x00
    u32 demoId;                 // 0x04
    // Set once the demo is ending: the commands no longer run
    u32 stopped;                // 0x08
    u8 unk_0c[0x4];             // 0x0c
    BOOL dual3D;                // 0x10
    fx32 frameStep;             // 0x14
    G3DCurve *cameraAnm;        // 0x18
    G3DManager *g3dMgr;         // 0x1c
    G3DCamera *camera;          // 0x20
    fx32 top;                   // 0x24
    fx32 bottom;                // 0x28
    u32 unitCount;              // 0x2c
    SRTMatrix srt;              // 0x30: translation, scale, rotation
    Demo3DCmdSys *cmdSys;       // 0x6c
    u16 *scenes;                // 0x70
    // The assembly of demo3d_cmd.c reads it at 0x74
    const Demo3DSetup *setup;   // 0x74
    Demo3DSceneParam scene;     // 0x78
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Demo3DEngine_Main
// Plays a frame, and the frames that were dropped before it. Returns whether the camera's animation has ended
BOOL func_ov203_021be538(Demo3DEngine *engine, u32 droppedFrames);
// pokebw2's Demo3DEngine_GetNowFrame
fx32 func_ov203_021be620(Demo3DEngine *engine);
// pokebw2's Demo3DEngine_SetStopped
void func_ov203_021be638(Demo3DEngine *engine, u32 stopped);
// pokebw2's Demo3DEngine_Create
Demo3DEngine *func_ov203_021be160(Demo3DGraphic *graphic, Demo3DParam *param, HeapID heapId);
// pokebw2's Demo3DEngine_Free
void func_ov203_021be4b4(Demo3DEngine *engine);
// pokebw2's Demo3DEngine_GetFrameCount
u32 func_ov203_021be62c(Demo3DEngine *engine);

#endif // BWPORT_APP_DEMO3D_DEMO3D_ENGINE_H
