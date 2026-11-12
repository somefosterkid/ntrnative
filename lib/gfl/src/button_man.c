// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/button_man.c.
#include "types.h"
#include "gfl/button_man.h"
#include "gfl/heap.h"
#include "gfl/touchpanel.h"

enum {
    BUTTON_RELEASED,
    BUTTON_PRESSED,
};

typedef struct {
    u8 state;
    // Whether the button is being touched
    u8 touched;
    // The frames the button has been held
    u16 holdFrames;
} Button;

struct ButtonMan {
    const TouchRect *rects;
    u32 count;
    ButtonManCallback callback;
    void *work;
    Button *buttons;
};

// Steps a button of a state, and returns its event or BMN_EVENT_NONE
typedef s32 (*ButtonStateFunc)(Button *button, BOOL touching, BOOL touchedNow);

static void func_02051c58(Button *button);
static s32 func_02051de8(Button *button, BOOL touching, BOOL touchedNow);
static s32 func_02051e10(Button *button, BOOL touching, BOOL touchedNow);
static void func_02051e64(Button *button, u8 state);

static const ButtonStateFunc data_020a15e8[] = {
    func_02051de8,
    func_02051e10,
};

ButtonMan *func_02051b84(const TouchRect *rects, ButtonManCallback callback, void *work, u32 heapId) {
    ButtonMan *mgr;
    int count;
    u32 i;

    for (count = 0; count < 0xff; count++) {
        if (rects[count].top == TOUCH_RECT_END) {
            break;
        }
    }
    mgr = GFL_HeapAllocate(heapId, sizeof(ButtonMan), FALSE, "button_man.c", 107);
    if (mgr != NULL) {
        mgr->rects = rects;
        mgr->count = count;
        mgr->callback = callback;
        mgr->work = work;
        mgr->buttons = GFL_HeapAllocate(heapId, count * sizeof(Button), FALSE, "button_man.c", 115);
        if (mgr->buttons != NULL) {
            for (i = 0; i < count; i++) {
                func_02051c58(&mgr->buttons[i]);
            }
        } else {
            GFL_HeapFree(mgr);
            mgr = NULL;
        }
    }
    return mgr;
}

static void func_02051c58(Button *button) {
    button->state = BUTTON_RELEASED;
    button->touched = FALSE;
    button->holdFrames = 0;
}

void func_02051c6c(ButtonMan *mgr) {
    GFL_HeapFree(mgr->buttons);
    GFL_HeapFree(mgr);
}

BOOL func_02051c88(ButtonMan *mgr) {
    BOOL touching;
    BOOL touchedNow;
    BOOL touchedButton = FALSE;
    unsigned long i; // NitroSDK's own u32; our u32 (unsigned int) swaps two registers here

    touching = func_02035c38();
    if (touching) {
        s32 held = func_02035ba4(mgr->rects);
        s32 touched = func_02035c08(mgr->rects);

        touchedNow = func_02035c60();
        for (i = 0; i < mgr->count; i++) {
            if (mgr->buttons[i].touched) {
                mgr->buttons[i].touched = held == i;
            } else {
                mgr->buttons[i].touched = touched == i;
                if (mgr->buttons[i].touched) {
                    touchedButton = TRUE;
                }
            }
        }
    } else {
        touchedNow = FALSE;
        for (i = 0; i < mgr->count; i++) {
            mgr->buttons[i].touched = FALSE;
        }
    }
    for (i = 0; i < mgr->count; i++) {
        Button *button = &mgr->buttons[i];
        s32 event = data_020a15e8[button->state](button, touching, touchedNow);

        if (event != BMN_EVENT_NONE) {
            mgr->callback(i, event, mgr->work);
        }
    }
    return touchedButton;
}

static s32 func_02051de8(Button *button, BOOL touching, BOOL touchedNow) {
    if (button->touched && touchedNow) {
        func_02051e64(button, BUTTON_PRESSED);
        return BMN_EVENT_TOUCH;
    }
    return BMN_EVENT_NONE;
}

static s32 func_02051e10(Button *button, BOOL touching, BOOL touchedNow) {
    if (button->touched) {
        if (button->holdFrames < 0xffff) {
            button->holdFrames++;
        }
        return BMN_EVENT_HOLD;
    }
    if (touching) {
        func_02051e64(button, BUTTON_RELEASED);
        return BMN_EVENT_SLIDEOUT;
    }
    func_02051e64(button, BUTTON_RELEASED);
    return BMN_EVENT_RELEASE;
}

static void func_02051e64(Button *button, u8 state) {
    button->state = state;
    button->holdFrames = 0;
}
