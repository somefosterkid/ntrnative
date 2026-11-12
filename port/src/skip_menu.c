// "Skip title and menu" (docs/launcher.md, "Skip title and menu"): launcher.ini's SkipMenu=on, or BWPORT_SKIPMENU=1|0 over
// it. The boot process's stand-in (port/extern/src/system/boot_proc.c) asks at the first tick of each boot whether to go
// straight into CONTINUE. Yes only once per start: a start of the process or the launcher's Play (PORT_EnterGame arms it);
// a Restart to title or the game's soft reset (an in-process reset, or a new process with reset.c's BWPORT_RESET) goes to
// the title, the way to NEW GAME and the menu's other items. The game's side checks the save (CONTINUE offered, no
// message about erased blocks); without one the boot is the game's own.
#include <nitro.h>
#include <stdio.h>
#include <stdlib.h>
#include "launcher_core.h"

static int sArmed = -1;   // -1: no PORT_EnterGame yet (a flat BWPORT_DIR folder)
static int sEntered;

void PORT_SkipMenuArm(void) {
    // the first entry of a process that a reset started (reset.c) is the title's way back, not a start
    sArmed = !(sEntered++ == 0 && getenv("BWPORT_RESET") != NULL);
}

int PORT_SkipMenu(int saveUsable) {
    const char *env = getenv("BWPORT_SKIPMENU");
    int on = env != NULL && env[0] != 0 ? atoi(env) != 0 : LC_SkipMenu();
    int armed = sArmed == -1 ? getenv("BWPORT_RESET") == NULL : sArmed;

    sArmed = 0;
    if (!on || !armed) {
        return 0;
    }
    if (!saveUsable) {
        fprintf(stderr, "bwport: skipmenu: no save to continue (or the menu would show a message about it): the title\n");
        return 0;
    }
    fprintf(stderr, "bwport: skipmenu: CONTINUE at V-blank %u, no title or menu\n", (unsigned)OS_GetVBlankCount());
    return 1;
}
