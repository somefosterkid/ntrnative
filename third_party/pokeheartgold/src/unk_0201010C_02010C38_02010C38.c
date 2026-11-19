#include "global.h"

typedef struct {
    s16 left[192];
    s16 right[192];
} WipeScanLines;

typedef struct {
    WipeScanLines lines;
    WipeScanLines next;
    u32 window;
} WipeScanBuf;

typedef struct {
    WipeScanBuf *buf;
    s32 count;
    s32 screen;
} WipeScanWnd;

WipeScanBuf *sub_02010EE0(WipeScanWnd *scan, s32 index);

static inline void WipeScanWnd_SetLine(WipeScanWnd *scan, s32 index, s32 line) {
    WipeScanBuf *buf = sub_02010EE0(scan, index);
    s32 screen = scan->screen;
    s16 right = buf->lines.right[line];
    s16 left = buf->lines.left[line];

    if (buf->window == 0) {
        if (screen == 0) {
            if (GX_IsHBlank()) {
                G2_SetWnd0Position(left, 0, right, 192);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd0Position(left, 0, right, 192);
            }
        }
    } else {
        if (screen == 0) {
            if (GX_IsHBlank()) {
                G2_SetWnd1Position(left, 0, right, 192);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd1Position(left, 0, right, 192);
            }
        }
    }
}

void sub_02010C38(void *data);

void sub_02010C38(void *data) {
    WipeScanWnd *scan = data;
    if (scan == NULL) {
        GF_AssertFail();
    }
    s32 line = GX_GetVCount();

    if (line < 192) {
        line++;
        if (line > 191) {
            line -= 192;
        }
        if (scan->count == 1) {
            WipeScanWnd_SetLine(scan, 0, line);
        } else {
            WipeScanWnd_SetLine(scan, 0, line);
            WipeScanWnd_SetLine(scan, 1, line);
        }
    }
}
