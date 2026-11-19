#include "global.h"
#include "wipe_screen.h"

struct WipeHBlank;

struct WipeScreen;

struct WipeWnd;

typedef struct WipeWnd WipeWnd;

typedef struct WipeHBlank WipeHBlank;

typedef void (*WipeHBlankFunc)(void *work);

void sub_0200FCDC(u16 color);

struct WipeHBlank {
    void *work[2];
    WipeHBlankFunc func[2];
    BOOL active[2];
};

enum {
    WIPE_SEQ_INIT,
    WIPE_SEQ_MAIN,
    WIPE_SEQ_EXIT,
    WIPE_SEQ_DONE,
};

typedef struct {
    u32 visible;
    u32 screen;
} WipeWndDisp;

typedef struct {
    u32 planes;
    BOOL effect;
    u32 window;
    u32 screen;
} WipeWndIn;

typedef struct {
    u32 planes;
    BOOL effect;
    u32 screen;
} WipeWndOut;

typedef struct {
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    u32 window;
    u32 screen;
} WipeWndPos;

struct WipeWnd {
    WipeWndDisp disp[2];
    WipeWndIn in[2][2];
    u8 unk50[0x18];
    WipeWndOut out[2];
    WipeWndPos pos[2][2];
};

typedef struct {
    u8 start[4];
    u8 end[4];
    u8 window;
    u8 inPlanes;
    u8 outPlanes;
    u8 isOut;
} WipeRectParam;

void sub_02011B5C(WipeScreen *screen, const WipeRectParam *param);

BOOL sub_02011B94(WipeScreen *screen);

BOOL FadeFunc_21(void *screen);

BOOL FadeFunc_21(void *ptr) {
    WipeScreen *screen = ptr;
    if (screen->seq == WIPE_SEQ_INIT) {
        extern const WipeRectParam _020F5DC8;

        sub_0200FCDC(screen->color);
        sub_02011B5C(screen, &_020F5DC8);
        screen->endCovered = 0;
        screen->useBrightness = 0;
        return 0;
    }
    return sub_02011B94(screen);
}

void sub_0201164C(WipeScreen *screen, const WipeRectParam *param);

BOOL sub_0201169C(WipeScreen *screen);

BOOL FadeFunc_22(void *screen);

BOOL FadeFunc_22(void *ptr) {
    WipeScreen *screen = ptr;
    if (screen->seq == WIPE_SEQ_INIT) {
        extern const WipeRectParam _020F5E08;

        sub_0200FCDC(screen->color);
        sub_0201164C(screen, &_020F5E08);
        screen->endCovered = 1;
        screen->useBrightness = 0;
        return 0;
    }
    return sub_0201169C(screen);
}

BOOL FadeFunc_23(void *screen);

BOOL FadeFunc_23(void *ptr) {
    WipeScreen *screen = ptr;
    if (screen->seq == WIPE_SEQ_INIT) {
        extern const WipeRectParam _020F5E14;

        sub_0200FCDC(screen->color);
        sub_0201164C(screen, &_020F5E14);
        screen->endCovered = 0;
        screen->useBrightness = 0;
        return 0;
    }
    return sub_0201169C(screen);
}

BOOL FadeFunc_24(void *screen);

BOOL FadeFunc_24(void *ptr) {
    WipeScreen *screen = ptr;
    if (screen->seq == WIPE_SEQ_INIT) {
        extern const WipeRectParam _020F5E20;

        sub_0200FCDC(screen->color);
        sub_0201164C(screen, &_020F5E20);
        screen->endCovered = 1;
        screen->useBrightness = 0;
        return 0;
    }
    return sub_0201169C(screen);
}
