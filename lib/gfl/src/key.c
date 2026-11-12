// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/key.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/std.h"
#include "gfl/ui.h"
#include "nitro/os.h"

// What a remapping does to its keys
enum {
    KEY_REMAP_END,
    // Also reports from as to
    KEY_REMAP_ADD,
    // Swaps from and to
    KEY_REMAP_SWAP,
    // No longer reports from
    KEY_REMAP_REMOVE,
};

typedef struct {
    u32 from;
    u32 to;
    u8 mode;
} KeyRemap;

struct KeypadManager {
    const KeyRemap *remaps;
    u32 unk4;
    u32 remapIndex;
    u32 prevHeld;
    u32 pressedRaw;
    u32 typedRaw;
    u32 held;
    u32 pressed;
    u32 typed;
    // The keys of two frames, at 30 frames per second, and of the frame before
    u32 held30;
    u32 pressed30;
    u32 typed30;
    u32 heldNext;
    u32 pressedNext;
    u32 typedNext;
    u32 repeatCounter;
    // The frames between repeats of a held key, and before the first
    u32 repeatWait;
    u32 repeatStart;
    u32 unk48;
};

static void GFL_HIDUpdateKeypad(SystemUI *ui);
void keypressInterpreter(KeypadManager *keypad);
static void setFramecounts(SystemUI *ui, u32 repeatWait, u32 repeatStart);
static void setTempSpaceToKeyBlock(SystemUI *ui, const void *remaps);
KeypadManager *GFL_HIDGetKeypadManager(SystemUI *ui);
static u32 GFL_HIDGetPressedKeys(SystemUI *ui);
static u32 GFL_HIDGetHeldKeys(SystemUI *ui);
static u32 GFL_HIDGetTypedKeys(SystemUI *ui);
static void func_0203634c(SystemUI *ui, u32 *repeatWait, u32 *repeatStart);

KeypadManager *GFL_HIDCreateKeypadManager(HeapID heapId) {
    KeypadManager *keypad = GFL_HeapAllocate(heapId, sizeof(KeypadManager), FALSE, "key.c", 68);

    MI_CpuFill8(keypad, 0, sizeof(KeypadManager));
    keypad->repeatWait = 8;
    keypad->repeatStart = 15;
    return keypad;
}

static void GFL_HIDUpdateKeypad(SystemUI *ui) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);
    u16 keys;

    if (PAD_DetectFold()) {
        keypad->pressed = 0;
        keypad->held = 0;
        keypad->typed = 0;
        keypad->pressed30 = 0;
        keypad->held30 = 0;
        keypad->typed30 = 0;
        return;
    }
    keys = PAD_Read();
    // Up cancels down, and left cancels right
    keys = keys & ~((keys & PAD_KEY_UP) << 1) & ~((keys & PAD_KEY_LEFT) >> 1);
    keypad->pressedRaw = keys & (keys ^ keypad->prevHeld);
    keypad->typedRaw = keys & (keys ^ keypad->prevHeld);
    if (keys != 0 && keypad->prevHeld == keys) {
        if (--keypad->repeatCounter == 0) {
            keypad->typedRaw = keys;
            keypad->repeatCounter = keypad->repeatWait;
        }
    } else {
        keypad->repeatCounter = keypad->repeatStart;
    }
    keypad->prevHeld = keys;
    keypad->pressed = keypad->pressedRaw;
    keypad->held = keys;
    keypad->typed = keypad->typedRaw;
    keypressInterpreter(keypad);
    keypad->pressedNext |= keypad->pressed;
    keypad->heldNext |= keypad->held;
    keypad->typedNext |= keypad->typed;
    if (ui->frameCount % 2 == 0) {
        keypad->pressed30 = keypad->pressedNext;
        keypad->held30 = keypad->heldNext;
        keypad->typed30 = keypad->typedNext;
        keypad->pressedNext = 0;
        keypad->heldNext = 0;
        keypad->typedNext = 0;
    }
}

void GCTX_HIDUpdateKeypad(void) {
    GFL_HIDUpdateKeypad(GCTX_HIDGetInstance());
}

static inline u32 swapKeys(const u32 *keys, const KeyRemap *remap) {
    u32 swapped = 0;
    if (*keys & remap->from) {
        swapped |= remap->to;
    }
    if (*keys & remap->to) {
        swapped |= remap->from;
    }
    return swapped;
}

void keypressInterpreter(KeypadManager *keypad) {
    const KeyRemap *remap;

    if (keypad->remaps == NULL) {
        return;
    }
    remap = &keypad->remaps[keypad->remapIndex];
    for (;; remap++) {
        switch (remap->mode) {
        case KEY_REMAP_END:
            return;
        case KEY_REMAP_ADD:
            if (keypad->pressed & remap->from) {
                keypad->pressed |= remap->to;
            }
            if (keypad->held & remap->from) {
                keypad->held |= remap->to;
            }
            if (keypad->typed & remap->from) {
                keypad->typed |= remap->to;
            }
            break;
        case KEY_REMAP_SWAP:
            keypad->pressed = (keypad->pressed & ((remap->from | remap->to) ^ 0xffff)) | swapKeys(&keypad->pressed, remap);
            keypad->held = (keypad->held & ((remap->from | remap->to) ^ 0xffff)) | swapKeys(&keypad->held, remap);
            keypad->typed = (keypad->typed & ((remap->from | remap->to) ^ 0xffff)) | swapKeys(&keypad->typed, remap);
            break;
        case KEY_REMAP_REMOVE:
            keypad->pressed &= remap->from ^ 0xffff;
            keypad->held &= remap->from ^ 0xffff;
            keypad->typed &= remap->from ^ 0xffff;
            break;
        }
    }
}

static void setFramecounts(SystemUI *ui, u32 repeatWait, u32 repeatStart) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);

    keypad->repeatWait = repeatWait;
    keypad->repeatStart = repeatStart;
}

void setKeypressFramecounts(u32 repeatWait, u32 repeatStart) {
    setFramecounts(GCTX_HIDGetInstance(), repeatWait, repeatStart);
}

static void setTempSpaceToKeyBlock(SystemUI *ui, const void *remaps) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);

    keypad->remaps = remaps;
}

void setKeyBlkStarted(const void *remaps) {
    setTempSpaceToKeyBlock(GCTX_HIDGetInstance(), remaps);
}

static u32 GFL_HIDGetPressedKeys(SystemUI *ui) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);

    if (ui->updateRate == 30) {
        return keypad->pressed30;
    }
    return keypad->pressed;
}

u32 GCTX_HIDGetPressedKeys(void) {
    return GFL_HIDGetPressedKeys(GCTX_HIDGetInstance());
}

static u32 GFL_HIDGetHeldKeys(SystemUI *ui) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);

    if (ui->updateRate == 30) {
        return keypad->held30;
    }
    return keypad->held;
}

u32 GCTX_HIDGetHeldKeys(void) {
    return GFL_HIDGetHeldKeys(GCTX_HIDGetInstance());
}

static u32 GFL_HIDGetTypedKeys(SystemUI *ui) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);

    if (ui->updateRate == 30) {
        return keypad->typed30;
    }
    return keypad->typed;
}

u32 GCTX_HIDGetTypedKeys(void) {
    return GFL_HIDGetTypedKeys(GCTX_HIDGetInstance());
}

static void func_0203634c(SystemUI *ui, u32 *repeatWait, u32 *repeatStart) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);

    *repeatWait = keypad->repeatWait;
    *repeatStart = keypad->repeatStart;
}

// pokebw2's GCTX_HIDGetRepeat
void func_02036370(u32 *repeatWait, u32 *repeatStart) {
    func_0203634c(GCTX_HIDGetInstance(), repeatWait, repeatStart);
}

void GFL_HIDClearKeypadState(SystemUI *ui) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);

    keypad->held30 = 0;
    keypad->pressed30 = 0;
    keypad->typed30 = 0;
    keypad->heldNext = 0;
    keypad->pressedNext = 0;
    keypad->typedNext = 0;
}

void func_020363b8(SystemUI *ui, u32 pressed, u32 pressed30) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);

    keypad->pressed30 = pressed30;
    keypad->pressed = pressed;
}

void func_020363d4(SystemUI *ui, u32 held, u32 held30) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);

    keypad->held30 = held30;
    keypad->held = held;
}

void func_020363f0(SystemUI *ui, u32 typed, u32 typed30) {
    KeypadManager *keypad = GFL_HIDGetKeypadManager(ui);

    keypad->typed30 = typed30;
    keypad->typed = typed;
}

