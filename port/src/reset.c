// The game's OS_ResetSystem (port_runtime.h makes the game's calls this one): the L+R+START+SELECT soft reset, the
// delete-save reset, the end of the game system proc, Mystery Gift's exit. The DS reboots there and never returns;
// libntr's returns (OSi_DoBoot is empty) after reloading ROM images through 32-bit DS addresses, which crashed on a
// 64-bit host. Here the game begins again at its boot with save.bin (written by the game's own backup writes, one
// fopen/fclose each) as it is:
// - macOS, iOS, Linux, Windows: in this process (machine.c: the machine's memory put back as before the game first
//   started; the window, the renderers, the audio and the settings stay);
// - Android cannot exec or spawn: a game the launcher started (BWPORT_GAME) ends its process and the launcher starts it again, or
//   shows the library (android_main.c bwport_android_leave); an old-layout run: a message box and the app ends (the maintainer opens it again; the save is on disk already;
//   an exec of /proc/self/exe would start the app's zygote binary, not the game).
// Android gets the in-process restart when its link has the machine's ranges (the maintainers' notes, owed).
#define BWPORT_PORT_RUNTIME_IMPL
#include <nitro.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__ANDROID__)
#include <SDL2/SDL.h>
#endif

void PORT_ResetSystem(u32 parameter) {
    (void)parameter;
    fprintf(stderr, "bwport: the game reset the system: starting again\n");
    fflush(NULL);
#if defined(__ANDROID__)
    {
        extern int bwport_android_leave(int play);    // android_main.c: a game started by the launcher goes back to it
        extern int PORT_LauncherWanted(void);         // main_start.c: Games asked for the library, else the same game again

        if (bwport_android_leave(!PORT_LauncherWanted())) {
            for (;;) {
                SDL_Delay(1000);    // (the process ends within a moment)
            }
        }
    }
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game reset",
                             "The game reset itself, as the DS does when it restarts.\n"
                             "The save is kept. Open the app again to start over.",
                             NULL);
    exit(0);
#else
    PORT_MachineReset();   // machine.c: back to the title in this process; never returns
#endif
    fprintf(stderr, "bwport: could not start again\n");
    exit(1);
}
