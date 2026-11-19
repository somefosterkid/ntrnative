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

void sub_02012DD8(WipeScreen *screen, const WipeRectParam *param);

BOOL sub_02012E10(WipeScreen *screen);

BOOL FadeFunc_42(void *screen);

BOOL FadeFunc_42(void *ptr) {
    WipeScreen *screen = ptr;
    if (screen->seq == WIPE_SEQ_INIT) {
        static const WipeRectParam param = {
            { 0x80, 0x00, 0x60, 0x0 },
            { 0x38, 0x8E, 0x00, 0x0 },
            0, 0x20, 0x3F, 1
        };

        sub_0200FCDC(screen->color);
        sub_02012DD8(screen, &param);
        screen->endCovered = 1;
        screen->useBrightness = 0;
        return 0;
    }
    return sub_02012E10(screen);
}
