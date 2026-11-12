// The PC build compiles src/system/boot_proc.c through this file (tools/port_sources.py). The port's "Skip title and menu"
// setting (docs/launcher.md, "Skip title and menu"): at the first tick of a boot that the port marks (port/src/skip_menu.c:
// the setting is on, the game was just started or entered from the launcher, not a Restart to title or a soft reset) and
// with a save the start menu would offer CONTINUE for without a message, the boot process queues CONTINUE's process
// itself, as the start menu does when CONTINUE is chosen, instead of the boot screens, the demos, the title and the menu.
// What the menu does on the way is done here too: its start's Dream World mark (src/ov088/startmenu.c, func_ov088_021ba114),
// its exit's func_020274c8, and the C-Gear's question answered as the save left it (no C-Gear: off, as the menu does
// without asking). Every other boot, and the boot proc's later ticks, are the game's own. The DS build never sees this
// file.
#pragma push_macro("data_020a0480")
#define data_020a0480 data_020a0480_game
#include "../../../../src/system/boot_proc.c"
#pragma pop_macro("data_020a0480")

#include "save/config.h"
#include "save/dream_world.h"
#include "save/save_control.h"
#include "save/trainer_data_savedata.h"
#include "system/game_beacon.h"

int PORT_SkipMenu(int saveUsable);   // port/src/skip_menu.c

extern const GameProcFunctions data_ov088_021bd824;   // src/ov088/game_start.c: CONTINUE's process

// src/ov088/game_start.c's parameters of CONTINUE's process
#define PORT_CONTINUE_CGEAR_OFF 1
#define PORT_CONTINUE_CGEAR_ON 2

static BOOL PortBootTick(GameProc *proc, u32 *state, void *param, void *work) {
    SaveControl *save;
    BOOL cgear;

    if (*state != 0) {
        return func_0202f9f8(proc, state, param, work);
    }
    save = SaveControl_GetInstance();
    // the start menu's CONTINUE item (func_ov088_021bb4b4), and none of its messages about erased blocks (func_ov088_021bc0c8)
    if (!PORT_SkipMenu(SaveControl_IsDataAlreadyPresent(save) == TRUE && func_020071e8(save) == 0)) {
        return func_0202f9f8(proc, state, param, work);
    }
    if (func_020092ec(getDreamWorldStuffAddress(save)) == 1) {
        func_0200c2f0(getTrainerGameInfoAddress(save), 2, 2);
    }
    cgear = func_020090e0(func_02009210(save)) && func_02008448(&getTrainerDataBlkAddress(save)->config);
    func_020274c8();
    GCTX_ProcMgrQueueProc(OVERLAY_ID(88), &data_ov088_021bd824,
                          (void *)(uintptr_t)(cgear ? PORT_CONTINUE_CGEAR_ON : PORT_CONTINUE_CGEAR_OFF));
    return FALSE;   // (*state stays 0: were CONTINUE's chain ever to end, the boot would go on as the game's)
}

const GameProcFunctions data_020a0480 = {
    func_0202f9f4,
    PortBootTick,
    func_0202fab0,
};
