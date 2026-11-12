// iOS (device and Simulator): the program's entry. SDL's UIKit app runs IosRun once UIKit is up, which runs libntr's main
// (libntr-ios-gles.patch calls it SIM_main); the launcher (port/src/ios_launcher.m: the games, Add Game, the saves) comes up
// from the run loop before the game boots when no game is chosen (port/src/machine.c's SIM_RunGame). SDL_main.h would rename libntr's own main for SDL, so
// SDL_MAIN_HANDLED and an explicit start.
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif
#if defined(__APPLE__) && TARGET_OS_IOS
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>

int SIM_main(int argc, char *argv[]);
void PORT_EnterWorkDir(void);

static int IosRun(int argc, char *argv[]) {
    PORT_EnterWorkDir();    // SDL's UIKit start has moved the working directory to the app bundle
    return SIM_main(argc, argv);
}

#undef main   // port_prefix.h names the game's main NitroMain; this is the program's
int main(int argc, char *argv[]) {
    return SDL_UIKitRunApp(argc, argv, IosRun);
}
#endif
