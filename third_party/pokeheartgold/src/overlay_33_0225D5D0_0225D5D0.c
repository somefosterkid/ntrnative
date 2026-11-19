#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "overlay_33.h"
#include "pm_string.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "overlay_33_ov33work.h"

struct Background;

struct BgConfig;

struct Ov33Args;

struct Ov33Config;

struct Ov33Work;

struct SysTask;

struct SysTaskQueue;

struct System;

struct Window;

struct WindowTemplate;

typedef struct Ov33Args {
    const u32 *strIds;
    u8 count;
    u8 unk5;
    u16 unk6;
} Ov33Args;

typedef struct Ov33Config {
    const WindowTemplate *windowTemplates;
    const TouchscreenHitbox *hitboxes;
    u8 unk8;
    u8 unk9;
    u16 unkA;
} Ov33Config;

extern const Ov33Config ov33_0225DA50[];

void ov33_0225D5D0(SysTask *task, void *taskEnv);

void ov33_0225D5D0(SysTask *task, void *taskEnv) {
    Ov33Work *work = taskEnv;
    const Ov33Config *config = &ov33_0225DA50[work->count - 1];
    if (work->state < 2) {
        if (work->unk10->unk5 == 1) {
            work->state = 2;
        } else if (work->unk10->unk5 == 2) {
            work->state = 4;
        }
    }
    switch (work->state) {
    case 0:
        work->touchedHitbox = TouchscreenHitbox_FindRectAtTouchNew(config->hitboxes);
        if (work->touchedHitbox == (u32)-1) {
            break;
        }
        ov33_0225D9D4(work, 1);
        work->unk10->unk6 = work->touchedHitbox;
        work->state = 1;
        break;
    case 1: {
        int held = TouchscreenHitbox_FindRectAtTouchHeld(config->hitboxes);
        work->unk10->unk6 = 0xFFFF;
        if (held == (int)work->touchedHitbox) {
            break;
        }
        ov33_0225D9D4(work, 0);
        work->state = 0;
        break;
    }
    case 2:
        BgClearTilemapBufferAndCommit(work->bgConfig, 4);
        BgClearTilemapBufferAndCommit(work->bgConfig, 5);
        ScheduleBgTilemapBufferTransfer(work->bgConfig, 4);
        ScheduleBgTilemapBufferTransfer(work->bgConfig, 5);
        work->state = 3;
        break;
    case 3:
        break;
    case 4:
        BgClearTilemapBufferAndCommit(work->bgConfig, 4);
        BgClearTilemapBufferAndCommit(work->bgConfig, 5);
        ScheduleBgTilemapBufferTransfer(work->bgConfig, 4);
        ScheduleBgTilemapBufferTransfer(work->bgConfig, 5);
        work->state = 5;
        break;
    case 5:
        if (gSystem.touchHeld != 0) {
            gSystem.simulatedInputs = 1;
        }
        break;
    }
}
