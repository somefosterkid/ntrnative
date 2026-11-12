// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/boot_proc.c, Black's version: Black
// plays the Game Freak logo, then, unless it was skipped, the second demo, then the opening (Black 2 has no second
// demo); the procs are overlay 88's and 179's.
#include "system/boot_proc.h"
#include "types.h"
#include "app/boot_screens.h"
#include "app/cdemo.h"
#include "app/title.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"

// The first process of the game: the boot screens, the Game Freak logo, the demos and the title screen, over and
// over. The file's name is a guess, from what it does: the ROM has no string for it

// The demos of overlay 179
#define CDEMO_GF_LOGO 0
#define CDEMO_SECOND 1
#define CDEMO_OPENING 2

static BOOL func_0202f9f4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_0202f9f8(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_0202fab0(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's EVENT_TITLE_FUNCS
const GameProcFunctions data_020a0480 = {
    func_0202f9f4,
    func_0202f9f8,
    func_0202fab0,
};

// pokebw2's sBootScreensParam
static BootScreensParam data_0214694c;
// pokebw2's sCDemoParam
static CDemoParam data_02146948;

// pokebw2's StartMenuEventInit
static BOOL func_0202f9f4(GameProc *proc, u32 *state, void *param, void *work) {
    return TRUE;
}

// pokebw2's StartMenuEventTick. Never ends: the title screen's process is queued after it, and once that ends it
// starts over, showing only the copyright notice of the boot screens
static BOOL func_0202f9f8(GameProc *proc, u32 *state, void *param, void *work) {
    switch (*state) {
    case 0:
        GCTX_ProcMgrQueueProc(OVERLAY_ID(88), &data_ov088_021bd8e8, &data_0214694c);
        *state = 1;
        break;
    case 1:
        data_0214694c.skipLogos = TRUE;
        data_02146948.demo = CDEMO_GF_LOGO;
        data_02146948.canSkip = TRUE;
        data_02146948.skipped = FALSE;
        GCTX_ProcMgrQueueProc(OVERLAY_ID(179), &data_ov179_021b68d8, &data_02146948);
        *state = 2;
        break;
    case 2:
        if (data_02146948.skipped == FALSE) {
            data_02146948.demo = CDEMO_SECOND;
            data_02146948.skipped = FALSE;
            GCTX_ProcMgrQueueProc(OVERLAY_ID(179), &data_ov179_021b68d8, &data_02146948);
            *state = 3;
        } else {
            *state = 4;
        }
        break;
    case 3:
        data_02146948.demo = CDEMO_OPENING;
        data_02146948.skipped = FALSE;
        GCTX_ProcMgrQueueProc(OVERLAY_ID(179), &data_ov179_021b68d8, &data_02146948);
        *state = 4;
        break;
    case 4:
        GCTX_ProcMgrQueueProc(OVERLAY_ID(88), &data_ov088_021bd574, NULL);
        *state = 0;
        break;
    }
    return FALSE;
}

// pokebw2's StartMenuEventEnd
static BOOL func_0202fab0(GameProc *proc, u32 *state, void *param, void *work) {
    return TRUE;
}
