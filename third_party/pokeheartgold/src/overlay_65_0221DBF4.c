#include "global.h"

#include "sys_task.h"
#include "screen_fade_internal.h"

struct SysTaskQueue;
typedef void (*UnkCallback)(void);
enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};
typedef struct System {
    Callback vBlankIntr;
    void *vBlankIntrArg;
    Callback hBlankIntr;
    void *hBlankIntrArg;
    UnkCallback unk10;
    UnkCallback unk14;
    SysTaskQueue *mainTaskQueue;
    SysTaskQueue *vblankTaskQueue;
    SysTaskQueue *vwaitTaskQueue;
    SysTaskQueue *printTaskQueue;
    u32 *unk28;
    u32 vblankCounter;
    u32 frameCounter;
    int buttonMode;
    int heldKeysRaw;
    int newKeysRaw;
    int newAndRepeatedKeysRaw;
    int heldKeys;
    int newKeys;
    int newAndRepeatedKeys;
    int keyRepeatCounter;
    int keyRepeatContinueDelay;
    int keyRepeatStartDelay;
    int simulatedInputs;
    u16 touchX;
    u16 touchY;
    u16 touchNew;
    u16 touchHeld;
    u8 touchpadReadAuto;
    u8 screensFlipped;
    u8 unk6A;
    u8 lidClosedPauseDisabled;
    u8 softResetDisabled;
    BOOL unk70;
    u32 *unk74;
} System;
extern System gSystem;

void ov65_0221DBF4(u32 *direction);

void ov65_0221DBF4(u32 *direction) {
    int pressedCount = 0;
    int dir = 0;

    if (gSystem.newAndRepeatedKeys & (0x0040)) {
        dir = 1;
        pressedCount++;
    }

    if (gSystem.newAndRepeatedKeys & (0x0080)) {
        dir = 2;
        pressedCount++;
    }

    if (gSystem.newAndRepeatedKeys & (0x0020)) {
        dir = 3;
        pressedCount++;
    }

    if (gSystem.newAndRepeatedKeys & (0x0010)) {
        dir = 4;
        pressedCount++;
    }

    if (pressedCount) {
        *direction = dir;
    }
}
