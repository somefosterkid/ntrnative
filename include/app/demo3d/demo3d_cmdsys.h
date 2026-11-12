// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/demo3d/demo3d_cmdsys.h: the lines in use
// so far.
#ifndef BWPORT_APP_DEMO3D_DEMO3D_CMDSYS_H
#define BWPORT_APP_DEMO3D_DEMO3D_CMDSYS_H

#include "types.h"
#include "app/demo3d/demo3d_capture.h"
#include "app/demo3d/demo3d_engine.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of heapId, tailHeapId, ending, textSpeed, bgmPushed, tcbMgr, font, arc
// and capture in Black
struct Demo3DCmdSys {
    HeapID heapId;
    HeapID tailHeapId;
    u32 demoId;
    // No commands are left
    BOOL finished;
    // The demo is over: the commands' tasks end at once
    BOOL ending;
    s32 lastFrame;
    s32 cmdIndex;
    s8 textSpeed;
    // DEMO3D_BGM_CHANGE_PUSH has pushed the BGM
    u8 bgmPushed;
    u8 droppedFrame;
    Demo3DEngine *engine;
    TCBExManager *tcbMgr;
    Font *font;
    ArcTool *arc;
    Demo3DCapture *capture;
};

// pokebw2's Demo3DCmdSys_Create
Demo3DCmdSys *func_ov203_021bf7c0(Demo3DEngine *engine, HeapID heapId);
void func_ov203_021bf858(Demo3DCmdSys *sys, fx32 nowFrame, BOOL droppedFrame);
void func_ov203_021bf8e8(Demo3DCmdSys *sys);
BOOL func_ov203_021bf964(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *index);

#endif // BWPORT_APP_DEMO3D_DEMO3D_CMDSYS_H
