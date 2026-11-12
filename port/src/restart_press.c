// "Restart to title" (the settings sheets, libntr's Game menu, cfgscript restart=1): presses the game's own soft-reset
// combination, L + R + START + SELECT (lib/gfl/src/ui.c GFL_HIDUpdate: all four held in one update and no
// softResetBlock flag), through the key register as the key script and the keyboard do. The game resets itself as a DS
// does, and keeps its rule: the combination is ignored while a save is being written (softResetBlock, save_control_intr.c).
// The press runs on its own thread so the caller (the UI, the cfgscript step in the frame thread) returns at once, and the
// keys are held for a few V-blanks: the game reads them once a frame.
#include <nitro.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define RESTART_KEYS (PAD_BUTTON_L | PAD_BUTTON_R | PAD_BUTTON_START | PAD_BUTTON_SELECT)
#define RESTART_HOLD_FRAMES 6

static volatile int sPressing;

static void *PressThread(void *arg) {
    struct timespec ms = {0, 1000000};
    struct timespec t0, t;
    u32 f0 = OS_GetVBlankCount();

    (void)arg;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    s_reg_PAD_KEYINPUT = (u16)(s_reg_PAD_KEYINPUT & ~RESTART_KEYS);
    // released after a few frames, or after 2 s when no frame runs (the game paused)
    for (;;) {
        clock_gettime(CLOCK_MONOTONIC, &t);
        if (OS_GetVBlankCount() - f0 >= RESTART_HOLD_FRAMES || t.tv_sec - t0.tv_sec >= 2) {
            break;
        }
        nanosleep(&ms, NULL);
    }
    s_reg_PAD_KEYINPUT = (u16)(s_reg_PAD_KEYINPUT | RESTART_KEYS);
    sPressing = 0;
    return NULL;
}

void bwport_press_restart(void) {
    pthread_t thread;

    if (sPressing) {
        return;
    }
    sPressing = 1;
    fprintf(stderr, "bwport: restart to title: L+R+START+SELECT, frame %u\n", (unsigned)OS_GetVBlankCount());
    if (pthread_create(&thread, NULL, PressThread, NULL) != 0) {
        sPressing = 0;
        return;
    }
    pthread_detach(thread);
}

// Game > Games... (libntr's Game menu on macOS, cfgscript games=1; libntr-ios-launcher.patch): the same press, flagged "then the
// launcher" (port/src/machine.c's run loop asks after the reset). If the game ignores the press (during a save) the flag stays
// until the next reset; Restart to title clears it.
void PORT_SetLauncherWanted(int on);   // main_start.c

void bwport_press_games(void) {
    fprintf(stderr, "bwport: games: the launcher after the reset\n");
    PORT_SetLauncherWanted(1);
    bwport_press_restart();
}
