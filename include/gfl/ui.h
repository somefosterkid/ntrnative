// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/ui.h, the lines in use so far.
#ifndef BWPORT_GFL_UI_H
#define BWPORT_GFL_UI_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// The system's input, the lid, sleep, the backlight and the battery (ui.c). The GCTX_ functions act on the one
// instance, which owns the keypad and touch panel managers

void GCTX_HIDUpdate(void);
// Whether the player is using the touch screen rather than the keys (pokebw2's func_0203d554)
BOOL func_020355c4(void);
// Sets whether the player is using the touch screen rather than the keys (pokebw2's func_0203d564)
void func_020355d8(BOOL touch);
// Keep the system from sleeping while any flag is set, and clear a flag (pokebw2's GCTX_HIDBlockSleep and
// GCTX_HIDUnblockSleep)
void func_02034f84(u8 flags);
void func_02034fb8(u8 flags);

// Called on a soft reset, before the game restarts
typedef void (*SoftResetCallback)(void *work);

typedef void (*UICallback)(void *work);

// Returns TRUE to keep the system from sleeping when the lid closes
typedef BOOL (*UISleepCheckCallback)(void *work);

// The input of a frame, as the keypad and touch panel managers keep it
struct HIDInputState {
    u16 pressed;
    u16 held;
    u16 typed;
    u16 touchX;
    u16 touchY;
    u8 touchPressed;
    u8 touchHeld;
};

// Replaces the input of a frame, at 60 and at 30 frames per second, returning TRUE if it did
typedef BOOL (*HIDInputHook)(HIDInputState *state, HIDInputState *state30);

struct SystemUI {
    KeypadManager *keypad;
    TouchpadManager *touchpad;
    SoftResetCallback softResetCallback;
    void *softResetWork;
    UICallback wakeCallback;
    UICallback sleepCallback;
    UICallback lidOpenCallback;
    UICallback lidCloseCallback;
    UICallback lowBatteryCallback;
    UISleepCheckCallback sleepCheckCallback;
    void *sleepWork;
    void *lidWork;
    void *lowBatteryWork;
    void *sleepCheckWork;
    // Sleep and the soft reset are blocked while any flag is set
    u8 sleepBlock;
    u8 softResetBlock;
    u8 unk3A;
    u8 backlight;
    u8 usingTouch;
    // 60, or 30 to report the input of two frames together
    u8 updateRate;
    u8 frameCount;
    // 0 to 5
    u8 batteryLevel;
    u8 mode;
    u8 lidCloseCalled;
    u8 unk42;
    // Whether the lid is closed while sleep is blocked, with the backlight off
    u8 lidClosed;
};

void GFL_HIDDoSoftReset(u32 parameter);

// pokebw2's GCTX_HIDSetLidCallbacks
void func_0203508c(UICallback close, UICallback open, void *work);

SystemUI *GCTX_HIDGetInstance(void);

// Whether the battery is low
BOOL GetIsShouldUICallback(void);
// pokebw2's GCTX_HIDSetSleepCheckCallback
void func_02035064(UISleepCheckCallback callback, void *work);

void GFL_HIDInit(HeapID heapId, u32 mode);

// pokebw2's GCTX_HIDGetBatteryLevel
int func_0203568c(void);

void GFL_HIDUpdate(SystemUI *ui);
// Called after waking when the battery is low
void SetUICallback(SystemUI *ui, UICallback callback, void *work);
void GFL_HIDSetSoftResetCallback(SystemUI *ui, SoftResetCallback callback, void *work);
void GFL_HIDSetUpdateRate(SystemUI *ui, u8 rate);
// Sets the update rate and clears the input kept for 30 frames per second
void GCTX_HIDChangeFPS(u8 rate);
// pokebw2's GFL_HIDBlockSleep
void func_02034f74(SystemUI *ui, u8 flags);
// pokebw2's GFL_HIDUnblockSleep
void func_02034fa0(SystemUI *ui, u8 flags);
// pokebw2's GFL_HIDIsSleepBlocked
BOOL func_02034fd4(SystemUI *ui, u8 flags);
// pokebw2's GFL_HIDSetSleepCallback
// Called before sleeping and after waking
void func_02035004(SystemUI *ui, UICallback callback, void *work);
// pokebw2's GCTX_HIDSetSleepCallback
void func_02035010(UICallback callback, void *work);
// pokebw2's GFL_HIDSetWakeCallback
void func_02035034(SystemUI *ui, UICallback callback, void *work);
// pokebw2's GFL_HIDSetLidCloseCallback
// Called when the lid closes and opens while sleep is blocked
void func_02035080(SystemUI *ui, UICallback callback, void *work);
// pokebw2's GFL_HIDGetLidCloseCallback
UICallback func_02035100(SystemUI *ui);
// pokebw2's GFL_HIDSetLidOpenCallback
void func_02035120(SystemUI *ui, UICallback callback, void *work);
// pokebw2's GFL_HIDBlockSoftReset
void func_0203515c(SystemUI *ui, u8 flags);
// pokebw2's GFL_HIDUnblockSoftReset
void func_02035188(SystemUI *ui, u8 flags);
// pokebw2's GFL_HIDIsSoftResetBlocked
BOOL func_020351bc(SystemUI *ui, u8 flags);
// pokebw2's GCTX_HIDIsLidClosed
BOOL func_02035390(void);
// pokebw2's GFL_HIDResetFrameCount
void func_020355ec(SystemUI *ui);
// pokebw2's GFL_HIDGetUpdateRate
u8 func_0203560c(SystemUI *ui);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void GCTX_HIDSetSoftResetCallback(SoftResetCallback callback, void *work);
u8 GCTX_HIDGetUpdateRate(void);
// pokebw2's GCTX_HIDUnblockSoftReset
void func_020351a0(u8 flags);
// pokebw2's GCTX_HIDBlockSoftReset
void func_0203516c(u8 flags);
void SetUICallbackStatic(UICallback callback, void *work);
// pokebw2's GCTX_HIDGetLidCloseCallback
UICallback func_02035108(void);
KeypadManager *GFL_HIDGetKeypadManager(SystemUI *ui);
void GCTX_HIDSetUpdateRate(u8 rate);
// pokebw2's GCTX_HIDIsSleepBlocked
BOOL func_02034fe8(u8 flags);
// pokebw2's GCTX_HIDSetWakeCallback
void func_02035040(UICallback callback, void *work);
// pokebw2's GCTX_HIDIsSoftResetBlocked
BOOL func_020351d0(u8 flags);
// pokebw2's GCTX_HIDResetFrameCount
void func_02035648(void);

#endif // BWPORT_GFL_UI_H
