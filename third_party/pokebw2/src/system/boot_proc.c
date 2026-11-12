#include "system/boot_proc.h"
#include "types.h"
#include "app/boot_screens.h"
#include "app/cdemo.h"
#include "app/title.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"

// The first process of the game: the boot screens, the Game Freak logo, the opening and the title screen, over and
// over. The file's name is a guess, from what it does: the ROM has no string for it

// The demos of overlay 264
#define CDEMO_GF_LOGO 0
#define CDEMO_OPENING 2

static BOOL StartMenuEventInit(GameProc *proc, u32 *state, void *param, void *work);
static BOOL StartMenuEventTick(GameProc *proc, u32 *state, void *param, void *work);
static BOOL StartMenuEventEnd(GameProc *proc, u32 *state, void *param, void *work);

const GameProcFunctions EVENT_TITLE_FUNCS = {
    StartMenuEventInit,
    StartMenuEventTick,
    StartMenuEventEnd,
};

static BootScreensParam sBootScreensParam;
static CDemoParam sCDemoParam;

#ifdef SDK_PORT
#include "app/game_start.h"
#include "save/c_gear_savedata.h"
#include "save/dream_world.h"
#include "save/save_control.h"
#include "system/dsi.h"
#include "system/game_beacon.h"

int PORT_SkipMenu(int saveUsable); // port/src/skip_menu.c

// src/ov162/game_start.c's parameters of CONTINUE's process
#define PORT_CONTINUE_CGEAR_OFF 1
#define PORT_CONTINUE_CGEAR_ON 2

// The port's "Skip title and menu" (docs/launcher.md): at a boot that port/src/skip_menu.c marks, with a save the start
// menu would offer CONTINUE for and show no message about (src/ov162/startmenu.c: no broken blocks, no Key System
// notice), queue CONTINUE's process as the start menu does when CONTINUE is chosen, doing what the menu does on the
// way: StartMenu_Setup's Dream World mark, StartMenu_Exit's GameBeaconSys_CheckGenuineCard, and StartMenu_Continue's
// C-Gear question answered as the save left it (C-Gear on and wireless on: on; otherwise off, as the menu does
// without asking or after its "wireless is off" message)
static BOOL StartMenuEvent_PortSkip(void) {
    SaveControl *save = SaveControl_GetInstance();
    TrainerGameInfoSave *gameInfo = getTrainerGameInfoAddress(save);
    BOOL cgear;

    if (!PORT_SkipMenu(SaveControl_IsDataAlreadyPresent(save) == TRUE && SaveControl_GetErrors(save) == 0
                       && TrainerGameInfo_GetKeyNotice(gameInfo) != TRUE)) {
        return FALSE;
    }
    if (DreamWorldSave_GetIDShown(getDreamWorldStuffAddress(save)) == TRUE) {
        TrainerGameInfo_AddStartMenuItemFlags(gameInfo, 2, 2);
    }
    cgear = CGearSettings_IsOn(CGearSettings_GetFromSave(save)) != 0 && isWirelessEnabled() != FALSE;
    GameBeaconSys_CheckGenuineCard();
    GCTX_ProcMgrQueueProc(OVERLAY_ID(162), &CONTINUE_PROC_FUNCTIONS,
                          (void *)(uintptr_t)(cgear ? PORT_CONTINUE_CGEAR_ON : PORT_CONTINUE_CGEAR_OFF));
    return TRUE;
}
#endif

static BOOL StartMenuEventInit(GameProc *proc, u32 *state, void *param, void *work) {
    return TRUE;
}

// Never ends: the title screen's process is queued after it, and once that ends it starts over, showing only the
// copyright notice of the boot screens
static BOOL StartMenuEventTick(GameProc *proc, u32 *state, void *param, void *work) {
    switch (*state) {
    case 0:
#ifdef SDK_PORT
        if (StartMenuEvent_PortSkip()) {
            break; // (*state stays 0: were CONTINUE's chain ever to end, the boot would go on as the game's)
        }
#endif
        GCTX_ProcMgrQueueProc(OVERLAY_ID(162), &BOOT_SCREENS_PROC_FUNCTIONS, &sBootScreensParam);
        *state = 1;
        break;
    case 1:
        sBootScreensParam.skipLogos = TRUE;
        sCDemoParam.demo = CDEMO_GF_LOGO;
        sCDemoParam.canSkip = TRUE;
        sCDemoParam.skipped = FALSE;
        GCTX_ProcMgrQueueProc(OVERLAY_ID(264), &CDEMO_PROC_FUNCTIONS, &sCDemoParam);
        *state = 2;
        break;
    case 2:
        sCDemoParam.demo = CDEMO_OPENING;
        sCDemoParam.skipped = FALSE;
        GCTX_ProcMgrQueueProc(OVERLAY_ID(264), &CDEMO_PROC_FUNCTIONS, &sCDemoParam);
        *state = 3;
        break;
    case 3:
        GCTX_ProcMgrQueueProc(OVERLAY_ID(162), &TITLE_PROC_FUNCTIONS, NULL);
        *state = 0;
        break;
    }
    return FALSE;
}

static BOOL StartMenuEventEnd(GameProc *proc, u32 *state, void *param, void *work) {
    return TRUE;
}
