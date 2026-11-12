// A key script in the exe itself, for runs nobody watches (tools/port_win64.sh run: Windows has no xdotool;
// tools/port_run.py presses keys with xdotool on Linux). Off unless BWPORT_KEYS is set.
//
// BWPORT_KEYS="start@100 a@110 a*0.5@115 down@120": each KEY[*HOLD]@SECONDS presses a DS button SECONDS after the
// start for HOLD seconds (default 0.1: the game reads the keys once a frame), as libntr's keyboard does: a bit of the
// key register (libntr's s_reg_PAD_KEYINPUT; reg_PAD_KEYINPUT is its read-only view) cleared, then set again.
// KEY: a b select start right left up down r l (the KEYINPUT bits), x y (the ARM7's word, HW_BUTTON_XY_BUF, as
// libntr's keyboard and gamepad press them).
// Host keys (rn80): "tab" (the Frame rate Unlimited toggle) and "space" (fast forward, held) are posted as SDL key events, as
// the keyboard would: "tab@f3000 tab@f6000" tests the toggle, "space*f300@f3000" a fast forward of 300 frames.
// WHEN is seconds, or fN for frame N of the V-blank count (OS_GetVBlankCount), as port_run.py's --keys: "start@f600".
// Touches (the phone's touch layout, libntr-ios-touch.patch; positions are window points): "tap:X,Y@f1700" a finger down at
// (X, Y) for 4 frames, "hold:X,Y@f1700-f1800" down at the first frame and up at the second (seconds without the f), and
// "drag:X0,Y0,X1,Y1@f1700-f1800" a finger moving from the first point to the second over that time. They are pushed as SDL
// finger events (a finger id of its own each), as the screen would send them: they work wherever the touch layout does.
// On Apple systems they are pushed on the main thread's queue (libntr-ios-inputprio.patch): they reach the game as UIKit's touches
// do, when the main run loop runs (SDL's pump, the game's waits on iOS), and go through the touch layer's event filter.
// HOLD is seconds, or fN for N frames ("down*f24@f3000": walking a step and a half whatever the frame rate). A frame
// key without *HOLD is held 4 frames.
#include <SDL2/SDL.h>
#include <nitro.h>
#include <pthread.h>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEYS_MAX 8192

typedef struct {
    u16 mask;
    int xy; // mask is in HW_BUTTON_XY_BUF (X, Y), not KEYINPUT
    double at;   // seconds after the start
    double hold; // seconds (frames if byFrame and no *HOLD)
    int byFrame; // at is a frame number
    int holdFrames;
} KeyPress;

static KeyPress sKeys[KEYS_MAX];
static int sKeyCount;

typedef struct {
    float x0, y0, x1, y1; // points
    double at, until;     // start, end (frames if byFrame)
    int byFrame;
    int state;            // 0 waiting, 1 down, 2 done
} TouchPress;

static TouchPress sTouches[KEYS_MAX];
static int sTouchCount;

static const char *const sKeyNames[] = {"a", "b", "select", "start", "right", "left", "up", "down", "r", "l", "x", "y", "tab", "space"};

static double Now(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

// tap:/hold:/drag: (positions with commas)
static void ParseTouch(const char *tok) {
    TouchPress *t = &sTouches[sTouchCount];
    const char *colon = strchr(tok, ':'), *at = strchr(tok, '@');
    float v[4] = {0, 0, 0, 0};
    int n, drag = strncmp(tok, "drag:", 5) == 0, tap = strncmp(tok, "tap:", 4) == 0;

    if (colon == NULL || at == NULL) {
        fprintf(stderr, "bwport: BWPORT_KEYS: %s is not tap:X,Y@WHEN / hold:X,Y@FROM-TO / drag:X0,Y0,X1,Y1@FROM-TO\n", tok);
        return;
    }
    n = sscanf(colon + 1, "%f,%f,%f,%f", &v[0], &v[1], &v[2], &v[3]);
    if (n < (drag ? 4 : 2)) {
        fprintf(stderr, "bwport: BWPORT_KEYS: %s: positions missing\n", tok);
        return;
    }
    t->x0 = v[0];
    t->y0 = v[1];
    t->x1 = drag ? v[2] : v[0];
    t->y1 = drag ? v[3] : v[1];
    t->byFrame = at[1] == 'f';
    t->at = atof(at + 1 + t->byFrame);
    {
        const char *dash = strchr(at, '-');
        t->until = dash != NULL ? atof(dash + 1 + (dash[1] == 'f')) : t->at + (tap ? 4 : 0);
    }
    t->state = 0;
    sTouchCount++;
}

static void ParseKeys(const char *script) {
    char *buf = strdup(script), *tok, *save;
    size_t k;
    int touch = 0;

    // commas separate keys, except inside a touch token (its positions)
    for (k = 0; buf[k] != 0; k++) {
        if (buf[k] == ' ') {
            touch = 0;
        } else if (k == 0 || buf[k - 1] == ' ') {
            touch = strncmp(buf + k, "tap:", 4) == 0 || strncmp(buf + k, "hold:", 5) == 0 || strncmp(buf + k, "drag:", 5) == 0;
        }
        if (buf[k] == ',' && !touch) {
            buf[k] = ' ';
        }
    }

    for (tok = strtok_r(buf, " ", &save); tok != NULL && sKeyCount < KEYS_MAX; tok = strtok_r(NULL, " ", &save)) {
        char *at = strchr(tok, '@'), *star = strchr(tok, '*');
        size_t len;
        u32 i;
        KeyPress *k = &sKeys[sKeyCount];

        if (strncmp(tok, "tap:", 4) == 0 || strncmp(tok, "hold:", 5) == 0 || strncmp(tok, "drag:", 5) == 0) {
            ParseTouch(tok);
            continue;
        }

        if (at == NULL) {
            fprintf(stderr, "bwport: BWPORT_KEYS: %s is not KEY[*HOLD]@SECONDS\n", tok);
            continue;
        }
        if (star != NULL && star > at) {
            star = NULL;
        }
        len = (size_t)((star != NULL ? star : at) - tok);
        k->mask = 0;
        for (i = 0; i < NELEMS(sKeyNames); i++) {
            if (strlen(sKeyNames[i]) == len && strncmp(tok, sKeyNames[i], len) == 0) {
                k->mask = (u16)(1 << i);
            }
        }
        if (k->mask == 0) {
            fprintf(stderr, "bwport: BWPORT_KEYS: no DS key %.*s\n", (int)len, tok);
            continue;
        }
        k->xy = k->mask >= (1 << 10) && k->mask < (1 << 12);
        k->hold = star != NULL ? atof(star + 1 + (star[1] == 'f')) : 0.1;
        k->byFrame = at[1] == 'f';
        k->holdFrames = (k->byFrame && star == NULL) || (star != NULL && star[1] == 'f');
        if (k->byFrame && star == NULL) {
            k->hold = 4;
        }
        k->at = atof(at + 1 + k->byFrame);
        sKeyCount++;
    }
    free(buf);
}

// A key down (or up): its bit in KEYINPUT or the X/Y word cleared (set), as libntr's keyboard does
static void SetKey(const KeyPress *k, int down) {
    if (k->mask >= (1 << 12)) { // a host key: tab, space
        SDL_Event e;
        memset(&e, 0, sizeof e);
        e.type = down ? SDL_KEYDOWN : SDL_KEYUP;
        e.key.state = down ? SDL_PRESSED : SDL_RELEASED;
        e.key.keysym.sym = k->mask == (1 << 12) ? SDLK_TAB : SDLK_SPACE;
        SDL_PushEvent(&e);
    } else if (k->xy) {
        vu16 *xy = (vu16 *)HW_BUTTON_XY_BUF;
        *xy = (u16)(down ? *xy & ~k->mask : *xy | k->mask);
    } else {
        s_reg_PAD_KEYINPUT = (u16)(down ? s_reg_PAD_KEYINPUT & ~k->mask : s_reg_PAD_KEYINPUT | k->mask);
    }
}

// when each script finger was let go (SDL's performance counter in ns, sim_touch.cpp's clock): its release log reads it as the
// moment the finger left the glass
static unsigned long long sUpNs[KEYS_MAX];

static unsigned long long CounterNs(void) {
    Uint64 c = SDL_GetPerformanceCounter(), f = SDL_GetPerformanceFrequency();
    return (c / f) * 1000000000ull + (c % f) * 1000000000ull / f;
}

unsigned long long bwport_keys_touch_ns(long long finger) {
    return finger >= 100 && finger < 100 + KEYS_MAX ? sUpNs[finger - 100] : 0;
}

typedef struct {
    int n, type;
    float x, y;
} FingerPush;

static void PushFingerNow(int n, int type, float x, float y);

#ifdef __APPLE__
static void PushFingerMain(void *arg) {
    FingerPush *p = arg;
    PushFingerNow(p->n, p->type, p->x, p->y);
    free(p);
}
#endif

// a finger event at (x, y) points, as SDL's UIKit sends them (normalised to the window)
static void PushFinger(int n, int type, float x, float y) {
    if (type == SDL_FINGERUP) {
        sUpNs[n] = CounterNs();
    }
#ifdef __APPLE__
    FingerPush *p = malloc(sizeof *p);
    p->n = n;
    p->type = type;
    p->x = x;
    p->y = y;
    dispatch_async_f(dispatch_get_main_queue(), p, PushFingerMain);
#else
    PushFingerNow(n, type, x, y);
#endif
}

static void PushFingerNow(int n, int type, float x, float y) {
    SDL_Window *w = NULL;
    SDL_Event e;
    int ww = 1, wh = 1, id;

    for (id = 1; id < 16 && w == NULL; id++) {
        w = SDL_GetWindowFromID(id);
    }

    if (w != NULL) {
        SDL_GetWindowSize(w, &ww, &wh);
    }
    memset(&e, 0, sizeof e);
    e.type = type;
    e.tfinger.touchId = 1;
    e.tfinger.fingerId = 100 + n;
    e.tfinger.x = x / (float)ww;
    e.tfinger.y = y / (float)wh;
    e.tfinger.pressure = 1.0f;
    SDL_PushEvent(&e);
}

static volatile int sRewind;

// The in-process restart (machine.c): the script runs again from the new boot's V-blank 0 (as when the program started
// again: the keys of a test replay after each reset)
void PORT_KeysReset(void) {
    sRewind = 1;
}

static void *ScriptThread(void *arg) {
    double start = *(double *)arg;
    struct timespec ms = {0, 1000000};
    int i, done = 0, touchDone = 0;
    u8 down[KEYS_MAX] = {0};
    double pf[KEYS_MAX], pt[KEYS_MAX];

    for (;;) {
        double t, fr;

        if (sRewind) {
            sRewind = 0;
            for (i = 0; i < sKeyCount; i++) {
                if (down[i] == 1) {
                    SetKey(&sKeys[i], 0);
                }
                down[i] = 0;
            }
            for (i = 0; i < sTouchCount; i++) {
                if (sTouches[i].state == 1) {
                    PushFinger(i, SDL_FINGERUP, sTouches[i].x1, sTouches[i].y1);
                }
                sTouches[i].state = 0;
            }
            done = touchDone = 0;
            start = Now();
        }
        if (done >= sKeyCount && touchDone >= sTouchCount) {
            struct timespec idle = {0, 10000000};
            nanosleep(&idle, NULL);   // all done: only a restart's rewind is waited for
            continue;
        }
        t = Now() - start;
        fr = (double)OS_GetVBlankCount();
        for (i = 0; i < sTouchCount; i++) {
            TouchPress *p = &sTouches[i];
            double n = p->byFrame ? fr : t;
            if (p->state == 0 && n >= p->at) {
                p->state = 1;
                PushFinger(i, SDL_FINGERDOWN, p->x0, p->y0);
                fprintf(stderr, "bwport: %.1f s, frame %.0f: finger %d down at %.0f,%.0f\n", t, fr, i, p->x0, p->y0);
            } else if (p->state == 1) {
                double f = p->until > p->at ? (n - p->at) / (p->until - p->at) : 1.0;
                if (n >= p->until) {
                    PushFinger(i, SDL_FINGERUP, p->x1, p->y1);
                    p->state = 2;
                    touchDone++;
                    fprintf(stderr, "bwport: %.1f s, frame %.0f: finger %d up\n", t, fr, i);
                } else if (p->x0 != p->x1 || p->y0 != p->y1) {
                    PushFinger(i, SDL_FINGERMOTION, p->x0 + (p->x1 - p->x0) * (float)f, p->y0 + (p->y1 - p->y0) * (float)f);
                }
            }
        }
        for (i = 0; i < sKeyCount; i++) {
            double n = sKeys[i].byFrame ? fr : t;
            if (down[i] == 0 && n >= sKeys[i].at) {
                SetKey(&sKeys[i], 1);
                down[i] = 1;
                pf[i] = fr;
                pt[i] = t;
                fprintf(stderr, "bwport: %.1f s, frame %.0f: %s\n", t, fr, sKeyNames[__builtin_ctz(sKeys[i].mask)]);
            } else if (down[i] == 1 && (sKeys[i].holdFrames ? fr - pf[i] : t - pt[i]) >= sKeys[i].hold) {
                SetKey(&sKeys[i], 0);
                down[i] = 2;
                done++;
            }
        }
        nanosleep(&ms, NULL);
    }
    return NULL;
}

__attribute__((constructor)) static void KeysScriptInit(void) {
    static double start;
    const char *keys = getenv("BWPORT_KEYS");
    pthread_t thread;

    if (keys == NULL) {
        return;
    }
    ParseKeys(keys);
    if (sKeyCount > 0 || sTouchCount > 0) {
        start = Now();
        pthread_create(&thread, NULL, ScriptThread, &start);
        pthread_detach(thread);
    }
}
