// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/ui.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/card.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "nitro/spi/ARM9/pm.h"

// The keys that reset the game when held together: L, R, START and SELECT
#define SOFT_RESET_KEYS (PAD_BUTTON_L | PAD_BUTTON_R | PAD_BUTTON_START | PAD_BUTTON_SELECT)

void func_0203524c(SystemUI *ui);
void func_02035268(SystemUI *ui);
void func_02035284(SystemUI *ui);
void func_020352ac(SystemUI *ui);
void battery(void);
void CallInputCallback(SystemUI *ui);
BOOL func_020353f8(SystemUI *ui);
static void setBacklightBothScreens(u32 backlight);
void sleepSetup(void);
void func_020356a0(SystemUI *ui);
BOOL CARD_TryWaitBackupAsync(void);
BOOL OS_IsRunOnTwl(void);

// Replaces the input of each frame, as for a replay. Never set
static HIDInputHook sInputHook;
// pokebw2's sSystemUI
static SystemUI *data_021469bc;


void GFL_HIDInit(HeapID heapId, u32 mode) {
    SystemUI *ui = GFL_HeapAllocate(heapId, sizeof(SystemUI), FALSE, "ui.c", 58);

    MI_CpuFill8(ui, 0, sizeof(SystemUI));
    ui->keypad = GFL_HIDCreateKeypadManager(heapId);
    ui->touchpad = initTouchpad(heapId);
    ui->usingTouch = FALSE;
    ui->updateRate = 60;
    data_021469bc = ui;
    ui->mode = mode;
    ui->batteryLevel = 5;
    if (mode == 0) {
        u32 backlight;

        PM_GetBackLight(0, &backlight);
        ui->backlight = backlight;
    }
    if (mode == 1) {
        u32 backlight;

        PM_GetBackLight(0, &backlight);
        ui->backlight = backlight;
        func_02034f84(0x10);
        func_0203516c(4);
    }
}

void GFL_HIDUpdate(SystemUI *ui) {
    ui->frameCount++;
    battery();
    GCTX_HIDUpdateKeypad();
    touchpadData();
    sleepSetup();
    func_020356a0(ui);
    if ((GCTX_HIDGetHeldKeys() & SOFT_RESET_KEYS) == SOFT_RESET_KEYS && ui->softResetBlock == 0) {
        if (ui->softResetCallback != NULL) {
            ui->softResetCallback(ui->softResetWork);
        }
        GFL_HIDDoSoftReset(0);
    }
}

void GCTX_HIDUpdate(void) {
    GFL_HIDUpdate(data_021469bc);
}

// pokebw2's GFL_HIDBlockSleep
void func_02034f74(SystemUI *ui, u8 flags) {
    ui->sleepBlock |= flags;
}

// pokebw2's GCTX_HIDBlockSleep
void func_02034f84(u8 flags) {
    func_02034f74(data_021469bc, flags);
}

// pokebw2's GFL_HIDUnblockSleep
void func_02034fa0(SystemUI *ui, u8 flags) {
    ui->sleepBlock &= ~flags;
}

// pokebw2's GCTX_HIDUnblockSleep
void func_02034fb8(u8 flags) {
    func_02034fa0(data_021469bc, flags);
}

// pokebw2's GFL_HIDIsSleepBlocked
BOOL func_02034fd4(SystemUI *ui, u8 flags) {
    if (ui->sleepBlock & flags) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GCTX_HIDIsSleepBlocked
BOOL func_02034fe8(u8 flags) {
    return func_02034fd4(data_021469bc, flags);
}

// pokebw2's GFL_HIDSetSleepCallback
void func_02035004(SystemUI *ui, UICallback callback, void *work) {
    ui->sleepCallback = callback;
    ui->sleepWork = work;
}

// pokebw2's GCTX_HIDSetSleepCallback
void func_02035010(UICallback callback, void *work) {
    func_02035004(data_021469bc, callback, work);
}

// pokebw2's GFL_HIDSetWakeCallback
void func_02035034(SystemUI *ui, UICallback callback, void *work) {
    ui->wakeCallback = callback;
    ui->sleepWork = work;
}

// pokebw2's GCTX_HIDSetWakeCallback
void func_02035040(UICallback callback, void *work) {
    func_02035034(data_021469bc, callback, work);
}

// pokebw2's GCTX_HIDSetSleepCheckCallback
void func_02035064(UISleepCheckCallback callback, void *work) {
    data_021469bc->sleepCheckCallback = callback;
    data_021469bc->sleepCheckWork = work;
}

// pokebw2's GFL_HIDSetLidCloseCallback
void func_02035080(SystemUI *ui, UICallback callback, void *work) {
    ui->lidCloseCallback = callback;
    ui->lidWork = work;
}

// Replacing the callbacks while the lid is closed opens it for the old ones and closes it for the new ones
// pokebw2's GCTX_HIDSetLidCallbacks
void func_0203508c(UICallback close, UICallback open, void *work) {
    SystemUI *ui = data_021469bc;

    if (ui->lidCloseCalled) {
        if (ui->lidOpenCallback != NULL) {
            ui->lidOpenCallback(ui->lidWork);
        }
        if (close != NULL) {
            close(work);
        }
    }
    func_02035080(data_021469bc, close, work);
    func_02035120(data_021469bc, open, work);
}

// pokebw2's GFL_HIDGetLidCloseCallback
UICallback func_02035100(SystemUI *ui) {
    return ui->lidCloseCallback;
}

// pokebw2's GCTX_HIDGetLidCloseCallback
UICallback func_02035108(void) {
    return func_02035100(data_021469bc);
}

// pokebw2's GFL_HIDSetLidOpenCallback
void func_02035120(SystemUI *ui, UICallback callback, void *work) {
    ui->lidOpenCallback = callback;
    ui->lidWork = work;
}

void SetUICallback(SystemUI *ui, UICallback callback, void *work) {
    ui->lowBatteryCallback = callback;
    ui->lowBatteryWork = work;
}

void SetUICallbackStatic(UICallback callback, void *work) {
    SetUICallback(data_021469bc, callback, work);
}

// pokebw2's GFL_HIDBlockSoftReset
void func_0203515c(SystemUI *ui, u8 flags) {
    ui->softResetBlock |= flags;
}

// pokebw2's GCTX_HIDBlockSoftReset
void func_0203516c(u8 flags) {
    func_0203515c(data_021469bc, flags);
}

// pokebw2's GFL_HIDUnblockSoftReset
void func_02035188(SystemUI *ui, u8 flags) {
    ui->softResetBlock &= ~flags;
}

// pokebw2's GCTX_HIDUnblockSoftReset
void func_020351a0(u8 flags) {
    func_02035188(data_021469bc, flags);
}

// pokebw2's GFL_HIDIsSoftResetBlocked
BOOL func_020351bc(SystemUI *ui, u8 flags) {
    if (ui->softResetBlock & flags) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GCTX_HIDIsSoftResetBlocked
BOOL func_020351d0(u8 flags) {
    return func_020351bc(data_021469bc, flags);
}

void GFL_HIDSetSoftResetCallback(SystemUI *ui, SoftResetCallback callback, void *work) {
    ui->softResetCallback = callback;
    ui->softResetWork = work;
}

void GCTX_HIDSetSoftResetCallback(SoftResetCallback callback, void *work) {
    GFL_HIDSetSoftResetCallback(data_021469bc, callback, work);
}

KeypadManager *GFL_HIDGetKeypadManager(SystemUI *ui) {
    return ui->keypad;
}

SystemUI *GCTX_HIDGetInstance(void) {
    if (data_021469bc == NULL) {
        OS_Terminate();
    }
    return data_021469bc;
}

// pokebw2's CallSleepCallback
void func_0203524c(SystemUI *ui) {
    if (ui->sleepCallback != NULL) {
        ui->sleepCallback(ui->sleepWork);
    }
}

// pokebw2's CallWakeCallback
void func_02035268(SystemUI *ui) {
    if (ui->wakeCallback != NULL) {
        ui->wakeCallback(ui->sleepWork);
    }
}

// pokebw2's CallLidCloseCallback
void func_02035284(SystemUI *ui) {
    if (ui->lidCloseCallback != NULL) {
        ui->lidCloseCallback(ui->lidWork);
        ui->lidCloseCalled = TRUE;
    }
}

// pokebw2's CallLidOpenCallback
void func_020352ac(SystemUI *ui) {
    if (ui->lidOpenCallback != NULL && ui->lidCloseCalled) {
        ui->lidOpenCallback(ui->lidWork);
        ui->lidCloseCalled = FALSE;
    }
}

void battery(void) {
    u16 level;
    PMBattery battery;

    if (OS_GetVBlankCount() % 600 == 1) {
        if (OS_IsRunOnTwl()) {
            if (PM_GetBatteryLevel(&level) == 0) {
                data_021469bc->batteryLevel = level;
            }
        } else if (PM_GetBattery(&battery) == 0) {
            if (battery == 0) {
                data_021469bc->batteryLevel = 5;
            } else {
                data_021469bc->batteryLevel = 1;
            }
        }
    }
}


// pokebw2's GCTX_HIDIsLidClosed
BOOL func_02035390(void) {
    if (PAD_DetectFold()) {
        return TRUE;
    }
    return FALSE;
}

BOOL GetIsShouldUICallback(void) {
    if (func_0203568c() <= 2) {
        return TRUE;
    }
    return FALSE;
}

void CallInputCallback(SystemUI *ui) {
    if (ui->lowBatteryCallback != NULL && GetIsShouldUICallback()) {
        ui->lowBatteryCallback(ui->lowBatteryWork);
    }
}

// pokebw2's IsSleepBlocked
BOOL func_020353f8(SystemUI *ui) {
    if (ui->sleepBlock) {
        return TRUE;
    }
    if (ui->sleepCheckCallback != NULL) {
        return ui->sleepCheckCallback(ui->sleepCheckWork);
    }
    return FALSE;
}

static void setBacklightBothScreens(u32 backlight) {
    int i;

    for (i = 0; i < 1000; i++) {
        if (PM_SetBackLight(PM_LCD_ALL, backlight) == 0) {
            break;
        }
    }
}

void sleepSetup(void) {
    SystemUI *ui = GCTX_HIDGetInstance();

    if (PAD_DetectFold()) {
        if (func_020353f8(ui) == 0) {
            func_02035e08();
            func_0203524c(ui);
            PM_GoSleepMode(12, 0, 0);
            CallInputCallback(ui);
            func_020352ac(ui);
            func_02035268(ui);
            func_02035dc8();
            if (ui->lidClosed) {
                ui->lidClosed = FALSE;
                setBacklightBothScreens(ui->backlight);
            }
        } else if (!ui->lidClosed) {
            ui->lidClosed = TRUE;
            setBacklightBothScreens(0);
            func_02035284(ui);
            func_0203524c(ui);
        }
    } else if (ui->lidClosed) {
        ui->lidClosed = FALSE;
        setBacklightBothScreens(ui->backlight);
        func_02035268(ui);
        func_020352ac(ui);
    }
}


void GFL_HIDDoSoftReset(u32 parameter) {
    SystemUI *ui = GCTX_HIDGetInstance();

    GX_SetMasterBrightness(16);
    GXS_SetMasterBrightness(16);
    func_0203ceac(0);
    for (;;) {
        if (func_0203d210() && CARD_TryWaitBackupAsync()) {
            setBacklightBothScreens(ui->backlight);
            OS_ResetSystem(parameter);
        }
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        func_0203cf5c();
        GFL_FadeUpdate();
    }
}



BOOL func_020355c4(void) {
    return data_021469bc->usingTouch;
}

void func_020355d8(BOOL touch) {
    data_021469bc->usingTouch = touch;
}

// pokebw2's GFL_HIDResetFrameCount
void func_020355ec(SystemUI *ui) {
    ui->frameCount = 0;
}

// Sets the frame rate the game updates at, 60 or 30, through data_021469bc whatever ui is
void GFL_HIDSetUpdateRate(SystemUI *ui, u8 rate) {
    data_021469bc->updateRate = rate;
}

// pokebw2's GFL_HIDGetUpdateRate
u8 func_0203560c(SystemUI *ui) {
    return ui->updateRate;
}

void GCTX_HIDSetUpdateRate(u8 rate) {
    GFL_HIDSetUpdateRate(data_021469bc, rate);
}

u8 GCTX_HIDGetUpdateRate(void) {
    return func_0203560c(data_021469bc);
}

// pokebw2's GCTX_HIDResetFrameCount
void func_02035648(void) {
    func_020355ec(data_021469bc);
}

void GCTX_HIDChangeFPS(u8 rate) {
    GFL_HIDSetUpdateRate(data_021469bc, rate);
    GFL_HIDClearKeypadState(data_021469bc);
    GFL_HIDClearTouchState(data_021469bc);
}

// pokebw2's GCTX_HIDGetBatteryLevel
int func_0203568c(void) {
    return data_021469bc->batteryLevel;
}

void func_020356a0(SystemUI *ui) {
    HIDInputState state;
    HIDInputState state30;

    if (sInputHook != NULL && sInputHook(&state, &state30)) {
        func_020363b8(ui, state.pressed, state30.pressed);
        func_020363d4(ui, state.held, state30.held);
        func_020363f0(ui, state.typed, state30.typed);
        func_02035e84(ui, state.touchX, state.touchY, state30.touchX, state30.touchY);
        func_02035eb0(ui, state.touchPressed, state30.touchPressed);
        func_02035ecc(ui, state.touchHeld, state30.touchHeld);
    }
}


