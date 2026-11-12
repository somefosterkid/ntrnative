// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov293/demo3d_cmdsys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/demo3d/demo3d_cmdsys.h"
#include "app/demo3d/demo3d_capture.h"
#include "app/demo3d/demo3d_cmd.h"
#include "app/demo3d/demo3d_data.h"
#include "app/demo3d/demo3d_engine.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/tcbl.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/gf_font.h"
#include "system/text_speed.h"

// What runs a 3D demo's commands, named after the ROM's "demo3d_cmdsys.c". A demo's commands are sorted by the frame
// of the camera's animation they run at; the ones that come first with DEMO3D_CMD_FRAME_START run when the demo
// starts, and a second list runs when it ends

// No list is longer than this
#define CMD_MAX 9999

void func_ov203_021bf810(Demo3DCmdSys *sys);
void func_ov203_021bf858(Demo3DCmdSys *sys, fx32 nowFrame, BOOL droppedFrame);
void func_ov203_021bf924(Demo3DCmdSys *sys);
void func_ov203_021bf9b8(Demo3DCmdSys *sys, Demo3DEngine *engine, const Demo3DCmd *cmd);

// pokebw2's gDemo3DCmdFuncs, by command type
Demo3DCmdFunc data_ov203_021c3d00[DEMO3D_CMD_COUNT + 1] = {
    NULL,
    func_ov203_021bebc8,
    func_ov203_021bec24,
    func_ov203_021bec3c,
    func_ov203_021bec60,
    func_ov203_021bec84,
    func_ov203_021bee04,
    func_ov203_021bee14,
    func_ov203_021bee1c,
    func_ov203_021bee40,
    func_ov203_021bef84,
    func_ov203_021bf128,
    func_ov203_021bf248,
    func_ov203_021bf26c,
    func_ov203_021bf290,
    func_ov203_021bf4c4,
    func_ov203_021bf78c,
    func_ov203_021bf79c,
    NULL,
};

// Allocates the command system and runs the demo's start commands
// pokebw2's Demo3DCmdSys_Create
Demo3DCmdSys *func_ov203_021bf7c0(Demo3DEngine *engine, HeapID heapId) {
    Demo3DCmdSys *sys = GFL_HeapAllocate(heapId, sizeof(Demo3DCmdSys), TRUE, "demo3d_cmdsys.c", 78);

    sys->engine = engine;
    sys->heapId = heapId;
    sys->tailHeapId = HEAPID_TAIL(heapId);
    sys->demoId = engine->scene.demoId;
    sys->lastFrame = -1;
    func_ov203_021bf8e8(sys);
    sys->finished = func_ov203_021bf964(sys, engine, &sys->cmdIndex);
    return sys;
}

// Runs the demo's end commands, then ends the system and frees it
void func_ov203_021bf810(Demo3DCmdSys *sys) {
    Demo3DEngine *engine = sys->engine;
    const Demo3DCmd *cmd = func_ov203_021be9a4(sys->demoId);
    s32 i;

    for (i = 0; i < CMD_MAX; i++) {
        if (cmd->type == DEMO3D_CMD_END) {
            break;
        }
        func_ov203_021bf9b8(sys, engine, cmd);
        cmd++;
    }
    sys->ending = TRUE;
    GFL_TCBExMgrUpdate(sys->tcbMgr);
    func_ov203_021bf924(sys);
    GFL_HeapFree(sys);
}

// Runs the commands of the frame the camera's animation is at, from the command the system is at on
void func_ov203_021bf858(Demo3DCmdSys *sys, fx32 nowFrame, BOOL droppedFrame) {
    Demo3DEngine *engine = sys->engine;
    const Demo3DCmd *cmd;
    s32 i;

    sys->droppedFrame = droppedFrame ? TRUE : FALSE;
    GFL_TCBExMgrUpdate(sys->tcbMgr);
    if (!sys->finished && nowFrame != -FX32_ONE) {
        cmd = &func_ov203_021be990(sys->demoId)[sys->cmdIndex];
        nowFrame >>= FX32_SHIFT;
        if (cmd->frame == nowFrame && sys->lastFrame != nowFrame) {
            i = 0;
            do {
                func_ov203_021bf9b8(sys, engine, cmd);
                cmd++;
                sys->finished = cmd->type == DEMO3D_CMD_END;
                sys->cmdIndex++;
                if (sys->finished == TRUE || cmd->frame != nowFrame) {
                    break;
                }
                i++;
            } while (i < CMD_MAX);
        }
        sys->lastFrame = nowFrame;
    }
}

// Sets up the system: its heap's task manager, the font, the 2D graphics' file handle and the text speed
void func_ov203_021bf8e8(Demo3DCmdSys *sys) {
    sys->tcbMgr = GFL_TCBExMgrCreate(sys->heapId, sys->heapId, 0x10, 0x80);
    sys->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, sys->heapId);
    sys->arc = GFL_ArcSysCreateFileHandle(ARCID_DEMO3D_2D, sys->heapId);
    sys->textSpeed = func_02012ffc();
}

// pokebw2's Demo3DCmdSys_Exit
void func_ov203_021bf924(Demo3DCmdSys *sys) {
    if (sys->capture != NULL) {
        func_ov203_021be9e8(sys->capture);
    }
    GFL_TCBExMgrFreeTasks(sys->tcbMgr);
    if (sys->bgmPushed) {
        GFL_SndBGMPop();
        GFL_SndBGMSetPaused(FALSE);
        GFL_SndBGMFadeIn(60);
    }
    GFL_ArcToolFree(sys->arc);
    GFL_FontFree(sys->font);
    GFL_TCBExMgrFree(sys->tcbMgr);
}

// pokebw2's Demo3DCmdSys_RunStartCmds
BOOL func_ov203_021bf964(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *index) {
    const Demo3DCmd *cmds = func_ov203_021be990(engine->scene.demoId);
    s32 i;

    for (i = 0; i < CMD_MAX; i++) {
        const Demo3DCmd *cmd = &cmds[i];

        if (cmd->type == DEMO3D_CMD_END) {
            return TRUE;
        }
        if (cmd->frame == DEMO3D_CMD_FRAME_START) {
            func_ov203_021bf9b8(sys, engine, cmd);
        } else if (cmd->frame >= engine->scene.startFrame) {
            *index = i;
            return FALSE;
        }
    }
    return FALSE;
}

// pokebw2's Demo3DCmdSys_Run
void func_ov203_021bf9b8(Demo3DCmdSys *sys, Demo3DEngine *engine, const Demo3DCmd *cmd) {
    data_ov203_021c3d00[cmd->type](sys, engine, (s32 *)cmd->params);
}
