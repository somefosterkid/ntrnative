// "Save first", the third choice of the leave warnings (Restart to title, Games: libntr's sim_gui.cpp, port/src/ios_settings.m,
// Android's SettingsScreen.java; docs/save-first.md): the game's own save, the one its menu's Save writes, then the leave.
// The UI asks (bwport_save_first); the game's frame hook saves on the game thread when its own menu would offer Save
// (Gen 4: port/game/save_gen4.c from the field's frame hook; Gen 5: port/extern/src/system/game_event.c and
// port/extern_b2w2's, an event of the port's that runs the save) and says each frame whether it could (PortSaveFirst_Can:
// the UI greys the button out when not). A waiter thread then presses the leave (port/src/restart_press.c, the same as Leave)
// once the game says the save is written, or gives up after SF_WAIT_FRAMES V-blanks (the game did not get free: a step,
// a script, a menu or a battle that did not end) and the game goes on with nothing left. V-blanks, not the wall clock: the
// settings sheets stop the game (iOS, Android), and the request waits for it.
#include <nitro.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "port_save_first.h"

enum { SF_IDLE, SF_WANTED, SF_SAVING, SF_SAVED, SF_REFUSED };
#define SF_CAN_FRAMES 8     // the hook said yes this many V-blanks ago at most (Gen 4's field frame is every other V-blank)
#define SF_WAIT_FRAMES 180  // 3 s of the game's time for the player to stand and the field to be free

void PORT_SetLauncherWanted(int on);   // main_start.c
void bwport_press_restart(void);       // restart_press.c

static pthread_mutex_t sLock = PTHREAD_MUTEX_INITIALIZER;
static int sState;
static int sThen;
static volatile int sCan;
static volatile u32 sCanFrame;

void PortSaveFirst_Can(int can) {
    if (can != sCan) {
        fprintf(stderr, "bwport: save first: %s (frame %u)\n", can ? "possible" : "not possible", (unsigned)OS_GetVBlankCount());
    }
    sCan = can;
    sCanFrame = OS_GetVBlankCount();
}

int bwport_save_first_possible(void) {
    return sCan && OS_GetVBlankCount() - sCanFrame <= SF_CAN_FRAMES;
}

int PortSaveFirst_Take(void) {
    int take;

    pthread_mutex_lock(&sLock);
    take = sState == SF_WANTED;
    if (take) {
        sState = SF_SAVING;
    }
    pthread_mutex_unlock(&sLock);
    if (take) {
        fprintf(stderr, "bwport: save first: saving (frame %u)\n", (unsigned)OS_GetVBlankCount());
    }
    return take;
}

void PortSaveFirst_Done(int saved, const char *why) {
    pthread_mutex_lock(&sLock);
    sState = saved ? SF_SAVED : SF_REFUSED;
    pthread_mutex_unlock(&sLock);
    fprintf(stderr, "bwport: save first: %s%s (frame %u)\n", saved ? "saved" : "not saved: ", saved ? "" : why, (unsigned)OS_GetVBlankCount());
}

int bwport_save_first_state(void) {
    int st;

    pthread_mutex_lock(&sLock);
    st = sState;
    pthread_mutex_unlock(&sLock);
    return st == SF_WANTED ? 1 : st == SF_SAVED ? 2 : st == SF_REFUSED ? 3 : st == SF_SAVING ? 4 : 0;
}

void bwport_save_first_cancel(void) {
    pthread_mutex_lock(&sLock);
    if (sState == SF_WANTED) {   // (a save the game has begun is not stopped: it ends, and the leave follows)
        sState = SF_IDLE;
        fprintf(stderr, "bwport: save first: cancelled (frame %u)\n", (unsigned)OS_GetVBlankCount());
    }
    pthread_mutex_unlock(&sLock);
}

static void *Waiter(void *arg) {
    struct timespec ms = {0, 2000000};
    u32 f0 = OS_GetVBlankCount();
    int st;

    (void)arg;
    for (;;) {
        pthread_mutex_lock(&sLock);
        st = sState;
        if (st == SF_WANTED && OS_GetVBlankCount() - f0 > SF_WAIT_FRAMES) {
            sState = st = SF_REFUSED;
        }
        pthread_mutex_unlock(&sLock);
        if (st == SF_SAVED || st == SF_REFUSED || st == SF_IDLE) {
            break;
        }
        nanosleep(&ms, NULL);
    }
    if (st == SF_SAVED) {
        fprintf(stderr, "bwport: save first: %s\n", sThen ? "the games list" : "restart to title");
        PORT_SetLauncherWanted(sThen ? 1 : 0);
        bwport_press_restart();
    } else if (st == SF_REFUSED) {
        fprintf(stderr, "bwport: save first: nothing saved, the game goes on (frame %u)\n", (unsigned)OS_GetVBlankCount());
    }
    return NULL;
}

int bwport_save_first(int then) {
    pthread_t thread;
    int busy;

    if (!bwport_save_first_possible()) {
        fprintf(stderr, "bwport: save first: refused, the game can't save here now (frame %u)\n", (unsigned)OS_GetVBlankCount());
        pthread_mutex_lock(&sLock);
        sState = SF_REFUSED;
        pthread_mutex_unlock(&sLock);
        return 0;
    }
    pthread_mutex_lock(&sLock);
    busy = sState == SF_WANTED || sState == SF_SAVING;
    if (!busy) {
        sState = SF_WANTED;
        sThen = then;
    }
    pthread_mutex_unlock(&sLock);
    if (busy) {
        return 1;
    }
    fprintf(stderr, "bwport: save first: asked, then %s (frame %u)\n", then ? "the games list" : "restart to title", (unsigned)OS_GetVBlankCount());
    if (pthread_create(&thread, NULL, Waiter, NULL) != 0) {
        pthread_mutex_lock(&sLock);
        sState = SF_IDLE;
        pthread_mutex_unlock(&sLock);
        return 0;
    }
    pthread_detach(thread);
    return 1;
}
