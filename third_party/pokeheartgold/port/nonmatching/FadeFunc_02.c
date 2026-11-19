#include "global.h"

struct WipeHBlank;
struct WipeScreen;
struct WipeWnd;
typedef struct WipeWnd WipeWnd;
typedef struct WipeHBlank WipeHBlank;
typedef struct WipeScreen WipeScreen;
struct WipeScreen {
    u32 type;
    s32 division;
    s32 sync;
    s32 seq;
    s32 screen;
    void *work;
    WipeWnd *wnd;
    WipeHBlank *hblank;
    u32 heapId;
    u16 color;

    BOOL endCovered;

    BOOL useBrightness;
};
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
    u8 start;
    u8 end;
    u16 covered;
} WipeLineRange;
typedef struct {
    const WipeLineRange *ranges;
    u16 count;
    u16 isOut;
} WipeLinePattern;
void sub_0201289C(WipeScreen *screen, WipeLinePattern *pattern);
BOOL sub_020128E0(WipeScreen *screen);

BOOL FadeFunc_02(WipeScreen *screen);

BOOL FadeFunc_02(WipeScreen *screen) {
    if (screen->seq == WIPE_SEQ_INIT) {
        static const WipeLineRange range = { 0x00, 0xc0, 1 };
        static WipeLinePattern pattern = { ((void *)0) , 1, 1 };

        pattern.ranges = &range;
        sub_0200FCDC(screen->color);
        sub_0201289C(screen, &pattern);
        screen->endCovered = 1 ;
        screen->useBrightness = 0 ;
        return 0 ;
    }
    return sub_020128E0(screen);
}
