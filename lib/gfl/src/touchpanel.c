// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/touchpanel.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/os.h"
#include "nitro/spi/ARM9/tp.h"

// A sampling attempt is retried this many times before the sample is dropped
#define SAMPLING_RETRIES 32

#define AUTO_SAMPLES 9

// A circle in a table of TouchRect
typedef struct {
    u8 code;
    u8 x;
    u8 y;
    u8 radius;
} TouchCircle;

struct TouchpadManager {
    u8 unk0[0xc];
    TPData samples[AUTO_SAMPLES];
    u8 unk54[4];
    u16 autoSampling;
    // Whether auto sampling is stopped while the system sleeps
    u16 stopped;
    u16 x;
    u16 y;
    u16 pressed;
    u16 held;
    // The input of two frames, at 30 frames per second, and of the frame before
    u16 x30;
    u16 y30;
    u16 pressed30;
    u16 held30;
    u16 xNext;
    u16 yNext;
    u16 pressedNext;
    u16 heldNext;
    u8 autoMode;
    u8 frequency;
    u8 unk76;
    u8 unk77[5];
};

static TouchpadManager *getTouchpadBlock(void);
static u16 func_02035760(SystemUI *ui);
static u16 func_02035780(SystemUI *ui);
static u16 func_020357a0(SystemUI *ui);
static u16 func_020357c0(SystemUI *ui);
void getTouchpadInput(SystemUI *ui);
static BOOL func_02035a70(const TouchRect *rect, u32 x, u32 y);
static BOOL func_02035aa4(const TouchRect *rect, u32 x, u32 y);
static s32 func_02035adc(const TouchRect *rects, u16 x, u16 y);
static s32 func_02035b58(SystemUI *ui, const TouchRect *rects);
static s32 func_02035bbc(SystemUI *ui, const TouchRect *rects);
static BOOL func_02035c20(SystemUI *ui);
static BOOL func_02035c48(SystemUI *ui);
static BOOL func_02035c70(SystemUI *ui, u32 *x, u32 *y);
static BOOL func_02035cd8(SystemUI *ui, u32 *x, u32 *y);
u32 func_02035d5c(TouchpadManager *tp);
static void func_02035d98(SystemUI *ui);
static void func_02035dd8(SystemUI *ui);
u32 func_02035e18(TouchpadManager *tp);

// pokebw2's sTouchpad
static TouchpadManager *data_021469c4;

static TouchpadManager *getTouchpadBlock(void) {
    return data_021469c4;
}

// pokebw2's GetTouchX
static u16 func_02035760(SystemUI *ui) {
    TouchpadManager *tp = getTouchpadBlock();

    if (ui->updateRate == 30) {
        return tp->x30;
    }
    return tp->x;
}

// pokebw2's GetTouchY
static u16 func_02035780(SystemUI *ui) {
    TouchpadManager *tp = getTouchpadBlock();

    if (ui->updateRate == 30) {
        return tp->y30;
    }
    return tp->y;
}

// pokebw2's GetTouchHeld
static u16 func_020357a0(SystemUI *ui) {
    TouchpadManager *tp = getTouchpadBlock();

    if (ui->updateRate == 30) {
        return tp->held30;
    }
    return tp->held;
}

// pokebw2's GetTouchPressed
static u16 func_020357c0(SystemUI *ui) {
    TouchpadManager *tp = getTouchpadBlock();

    if (ui->updateRate == 30) {
        return tp->pressed30;
    }
    return tp->pressed;
}

TouchpadManager *initTouchpad(HeapID heapId) {
    TPCalibrateParam calibrate;
    TPCalibrateParam *p;
    TouchpadManager *tp = GFL_HeapAllocate(heapId, sizeof(TouchpadManager), FALSE, "touchpanel.c", 131);

    MI_CpuFill8(tp, 0, sizeof(TouchpadManager));
    tp->autoSampling = 0;
    tp->stopped = 0;
    TP_Init();
    p = &calibrate;
    if (TP_GetUserInfo(p) == TRUE) {
        TP_SetCalibrateParam(p);
    } else {
        calibrate.x0 = 0x2ae;
        calibrate.y0 = 0x58c;
        calibrate.xDotSize = 0xe25;
        calibrate.yDotSize = 0x1208;
        TP_SetCalibrateParam(p);
    }
    tp->frequency = 4;
    tp->unk76 = 0;
    data_021469c4 = tp;
    return tp;
}

void getTouchpadInput(SystemUI *ui) {
    TPData raw;
    TPData disp;
    BOOL failed = FALSE;
    TouchpadManager *tp = getTouchpadBlock();

    if (tp == NULL) {
        return;
    }
    if (PAD_DetectFold()) {
        MI_CpuFill8(tp, 0, sizeof(TouchpadManager));
        return;
    }
    if (tp->autoMode == 0) {
        u8 retries = 0;

        TP_RequestSamplingAsync();
        while (TP_WaitRawResult(&raw) != 0) {
            retries++;
            if (retries >= SAMPLING_RETRIES) {
                failed = TRUE;
                disp.validity = TP_VALIDITY_INVALID_XY;
                break;
            }
            TP_RequestSamplingAsync();
        }
    } else {
        TP_GetLatestRawPointInAuto(&raw);
    }
    if (!failed) {
        TP_GetCalibratedPoint(&disp, &raw);
    }
    if (disp.validity == TP_VALIDITY_VALID) {
        tp->x = disp.x;
        tp->y = disp.y;
    } else if (tp->held) {
        // Keep the coordinate that is not valid
        switch (disp.validity) {
        case TP_VALIDITY_INVALID_X:
            tp->y = disp.y;
            break;
        case TP_VALIDITY_INVALID_Y:
            tp->x = disp.x;
            break;
        case TP_VALIDITY_INVALID_XY:
            break;
        }
    } else {
        disp.touch = 0;
    }
    tp->pressed = disp.touch & (disp.touch ^ tp->held);
    tp->held = disp.touch;
    tp->pressedNext |= tp->pressed;
    tp->heldNext |= tp->held;
    if (tp->y != 0) {
        tp->yNext = tp->y;
    }
    if (tp->x != 0) {
        tp->xNext = tp->x;
    }
    if (ui->frameCount % 2 == 0) {
        tp->x30 = tp->xNext;
        tp->y30 = tp->yNext;
        tp->pressed30 = tp->pressedNext;
        tp->held30 = tp->heldNext;
        tp->xNext = 0;
        tp->yNext = 0;
        tp->pressedNext = 0;
        tp->heldNext = 0;
    }
}



void touchpadData(void) {
    getTouchpadInput(GCTX_HIDGetInstance());
}

// pokebw2's HitCircle
static BOOL func_02035a70(const TouchRect *rect, u32 x, u32 y) {
    const TouchCircle *circle = (const TouchCircle *)rect;
    u32 dx = circle->x - x;
    u32 dy = circle->y - y;

    if (dx * dx + dy * dy < circle->radius * circle->radius) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's HitRect
static BOOL func_02035aa4(const TouchRect *rect, u32 x, u32 y) {
    if (x - rect->left <= (u32)(rect->right - rect->left) && y - rect->top <= (u32)(rect->bottom - rect->top)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GetHitRect
static s32 func_02035adc(const TouchRect *rects, u16 x, u16 y) {
    int i;

    for (i = 0; rects[i].top != TOUCH_RECT_END; i++) {
        if (rects[i].top == TOUCH_RECT_SKIP) {
            continue;
        }
        if (rects[i].top == TOUCH_RECT_CIRCLE) {
            if (func_02035a70(&rects[i], x, y)) {
                return i;
            }
        } else if (func_02035aa4(&rects[i], x, y)) {
            return i;
        }
    }
    return TOUCH_RECT_NONE;
}

// pokebw2's GetHeldRect
static s32 func_02035b58(SystemUI *ui, const TouchRect *rects) {
    getTouchpadBlock();
    if (func_020357a0(ui)) {
        return func_02035adc(rects, func_02035760(ui), func_02035780(ui));
    }
    return TOUCH_RECT_NONE;
}

s32 func_02035ba4(const TouchRect *rects) {
    return func_02035b58(GCTX_HIDGetInstance(), rects);
}

// pokebw2's GetPressedRect
static s32 func_02035bbc(SystemUI *ui, const TouchRect *rects) {
    getTouchpadBlock();
    if (func_020357c0(ui)) {
        return func_02035adc(rects, func_02035760(ui), func_02035780(ui));
    }
    return TOUCH_RECT_NONE;
}

s32 func_02035c08(const TouchRect *rects) {
    return func_02035bbc(GCTX_HIDGetInstance(), rects);
}

// pokebw2's IsTouchHeld
static BOOL func_02035c20(SystemUI *ui) {
    getTouchpadBlock();
    return func_020357a0(ui);
}

BOOL func_02035c38(void) {
    return func_02035c20(GCTX_HIDGetInstance());
}

// pokebw2's IsTouchPressed
static BOOL func_02035c48(SystemUI *ui) {
    getTouchpadBlock();
    return func_020357c0(ui);
}

BOOL func_02035c60(void) {
    return func_02035c48(GCTX_HIDGetInstance());
}

// pokebw2's GetHeldPoint
static BOOL func_02035c70(SystemUI *ui, u32 *x, u32 *y) {
    getTouchpadBlock();
    if (func_020357a0(ui)) {
        *x = func_02035760(ui);
        *y = func_02035780(ui);
        return TRUE;
    }
    return FALSE;
}

BOOL func_02035cb8(u32 *x, u32 *y) {
    return func_02035c70(GCTX_HIDGetInstance(), x, y);
}

// pokebw2's GetPressedPoint
static BOOL func_02035cd8(SystemUI *ui, u32 *x, u32 *y) {
    getTouchpadBlock();
    if (func_020357c0(ui)) {
        *x = func_02035760(ui);
        *y = func_02035780(ui);
        return TRUE;
    }
    return FALSE;
}

BOOL func_02035d20(u32 *x, u32 *y) {
    return func_02035cd8(GCTX_HIDGetInstance(), x, y);
}

s32 func_02035d40(const TouchRect *rects, u32 x, u32 y) {
    return func_02035adc(rects, x, y);
}

u32 func_02035d5c(TouchpadManager *tp) {
    if (tp->autoSampling == 0) {
        return 1;
    }
    TP_RequestAutoSamplingStopAsync();
    TP_WaitBusy(TP_REQUEST_COMMAND_FLAG_AUTO_OFF);
    if (TP_CheckError(TP_REQUEST_COMMAND_FLAG_AUTO_OFF) != 0) {
        return 2;
    }
    return 1;
}


// pokebw2's RestartSampling
static void func_02035d98(SystemUI *ui) {
    TouchpadManager *tp = getTouchpadBlock();

    if (tp != NULL && tp->stopped != 0 && tp->autoSampling != 0) {
        func_02035e18(tp);
        tp->stopped = 0;
    }
}

void func_02035dc8(void) {
    func_02035d98(GCTX_HIDGetInstance());
}

// pokebw2's StopSampling
static void func_02035dd8(SystemUI *ui) {
    TouchpadManager *tp = getTouchpadBlock();

    if (tp != NULL && tp->stopped != 1 && tp->autoSampling != 0) {
        func_02035d5c(tp);
        tp->stopped = 1;
    }
}

void func_02035e08(void) {
    func_02035dd8(GCTX_HIDGetInstance());
}

u32 func_02035e18(TouchpadManager *tp) {
    TP_RequestAutoSamplingStartAsync(0, tp->frequency, tp->samples, AUTO_SAMPLES);
    TP_WaitBusy(TP_REQUEST_COMMAND_FLAG_AUTO_ON);
    if (TP_CheckError(TP_REQUEST_COMMAND_FLAG_AUTO_ON) != 0) {
        return 2;
    }
    return 1;
}


void GFL_HIDClearTouchState(SystemUI *ui) {
    TouchpadManager *tp = getTouchpadBlock();

    tp->x30 = 0;
    tp->y30 = 0;
    tp->pressed30 = 0;
    tp->held30 = 0;
    tp->xNext = 0;
    tp->yNext = 0;
    tp->pressedNext = 0;
    tp->heldNext = 0;
}

void func_02035e84(SystemUI *ui, u16 x, u16 y, u16 x30, u16 y30) {
    TouchpadManager *tp = getTouchpadBlock();

    tp->x30 = x30;
    tp->y30 = y30;
    tp->x = x;
    tp->y = y;
}

void func_02035eb0(SystemUI *ui, u16 pressed, u16 pressed30) {
    TouchpadManager *tp = getTouchpadBlock();

    tp->pressed30 = pressed30;
    tp->pressed = pressed;
}

void func_02035ecc(SystemUI *ui, u16 held, u16 held30) {
    TouchpadManager *tp = getTouchpadBlock();

    tp->held30 = held30;
    tp->held = held;
}
