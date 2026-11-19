#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "pokemon_types_def.h"
#include "seal_case.h"
#include "sys_task.h"
#include "sys_task_api.h"

struct WaitDial;
typedef struct WaitDial WaitDial;
enum WaitDialDrawMode {
    DIAL_DRAW_MODE_LOAD_ONLY = 0,
    DIAL_DRAW_MODE_LOAD_AND_DRAW,
    DIAL_DRAW_MODE_CLEAR,
};
struct WaitDial {
    Window *window;
    u8 pixels[(((8 * (8 / 2)) * (2 * 2)) * 8)];
    u8 messageBoxPixels[((8 * (8 / 2)) * (2 * 2))];
    u16 messageBoxTile;
    u8 counter;
    u8 curFrame : 7;
    u8 : 1;
    u8 deleteMode : 2;
    u8 : 6;
};
enum WaitDialDeleteMode {
    DIAL_DELETE_MODE_NONE = 0,
    DIAL_DELETE_MODE_CLEAR,
    DIAL_DELETE_MODE_DESTROY,
};
void sub_0200F1D4(WaitDial *dial, u32 drawMode);
static void sub_0200F43C(SysTask *task, void *data);

void sub_0200F1D4(WaitDial *dial, u32 drawMode);
void sub_0200F3D0(SysTask *task, void *data);
static void sub_0200F43C(SysTask *sysTask, void *ctx);
void sub_0200F450(void *taskData);
void sub_0200F478(void *taskData);

void sub_0200F1D4(WaitDial *dial, u32 drawMode) {
    u8 bgLayer = GetWindowBgId(dial->window);
    u8 x = GetWindowX(dial->window);
    u8 y = GetWindowY(dial->window);
    u8 width = GetWindowWidth(dial->window);

    if (drawMode == DIAL_DRAW_MODE_CLEAR) {
        BG_LoadCharTilesData(dial->window->bgConfig, bgLayer, dial->messageBoxPixels, ((8 * (8 / 2)) * (2 * 2)), dial->messageBoxTile + 18);
        FillBgTilemapRect(dial->window->bgConfig, bgLayer, dial->messageBoxTile + 10, x + width + 1, y + 2, 1, 1, 16);
        FillBgTilemapRect(dial->window->bgConfig, bgLayer, dial->messageBoxTile + 11, x + width + 2, y + 2, 1, 1, 16);
        FillBgTilemapRect(dial->window->bgConfig, bgLayer, dial->messageBoxTile + 10, x + width + 1, y + 3, 1, 1, 16);
        FillBgTilemapRect(dial->window->bgConfig, bgLayer, dial->messageBoxTile + 11, x + width + 2, y + 3, 1, 1, 16);
        BgCommitTilemapBufferToVram(dial->window->bgConfig, bgLayer);

        return;
    }

    BG_LoadCharTilesData(dial->window->bgConfig, bgLayer, &dial->pixels[((8 * (8 / 2)) * (2 * 2)) * dial->curFrame], ((8 * (8 / 2)) * (2 * 2)), dial->messageBoxTile + 18);

    if (drawMode == DIAL_DRAW_MODE_LOAD_ONLY) {
        return;
    }

    FillBgTilemapRect(dial->window->bgConfig, bgLayer, dial->messageBoxTile + 18, x + width + 1, y + 2, 1, 1, 16);
    FillBgTilemapRect(dial->window->bgConfig, bgLayer, dial->messageBoxTile + 19, x + width + 2, y + 2, 1, 1, 16);
    FillBgTilemapRect(dial->window->bgConfig, bgLayer, dial->messageBoxTile + 20, x + width + 1, y + 3, 1, 1, 16);
    FillBgTilemapRect(dial->window->bgConfig, bgLayer, dial->messageBoxTile + 21, x + width + 2, y + 3, 1, 1, 16);
    BgCommitTilemapBufferToVram(dial->window->bgConfig, bgLayer);
}

void sub_0200F3D0(SysTask *task, void *data) {
    WaitDial *dial = data;

    if (dial->deleteMode != DIAL_DELETE_MODE_NONE) {
        if (dial->deleteMode == DIAL_DELETE_MODE_CLEAR) {
            sub_0200F1D4(dial, DIAL_DRAW_MODE_CLEAR);
        }

        SysTask_Destroy(task);
        return;
    }

    dial->counter++;

    if (dial->counter == 16) {
        dial->counter = 0;
        dial->curFrame = (dial->curFrame + 1) & 7;
        sub_0200F1D4(dial, DIAL_DRAW_MODE_LOAD_ONLY);
    }
}

static void sub_0200F43C(SysTask *sysTask, void *ctx) {
    Heap_Free(ctx);
    SysTask_Destroy(sysTask);
}

void sub_0200F450(void *taskData) {
    WaitDial *dial = taskData;

    SysTask_CreateOnVWaitQueue(sub_0200F43C, dial, 0);
    dial->deleteMode = DIAL_DELETE_MODE_CLEAR;
}

void sub_0200F478(void *taskData) {
    WaitDial *dial = taskData;

    SysTask_CreateOnVWaitQueue(sub_0200F43C, dial, 0);
    dial->deleteMode = DIAL_DELETE_MODE_DESTROY;
}
