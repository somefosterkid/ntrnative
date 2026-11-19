#include "global.h"

#include "pokemon_types_def.h"
#include "sys_task.h"
#include "screen_fade_internal.h"
#include "overlay_manager.h"
#include "touch_hitbox_controller.h"

struct OverlayManager;
struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct SysTaskQueue;
struct TouchHitboxController;
struct UnkStruct_ov20_021D2128_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatArgs EasyChatArgs;
typedef struct EasyChatWordList EasyChatWordList;
typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);
void *OverlayManager_GetData(OverlayManager *appMan);
typedef struct EasyChatApp EasyChatApp;
typedef int (*EasyChatMainFunc)(EasyChatApp *easyChatApp, int *state);
typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);
typedef struct {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_ov20_021D1F34;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    int unk_08;
} UnkStruct_ov20_021D18BC;
typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
} UnkStruct_ov20_021D0F64;
struct EasyChatApp {
    EasyChatArgs *args;
    u32 type;
    MailMessage sentence;
    u16 words[2];
    UnkStruct_ov20_021D2128 *unk_14;
    EasyChatWordList *unk_18;
    int state;
    EasyChatMainFunc mainFunc;
    EasyChatMainFunc nextMainFunc;
    int taskFuncState;
    EasyChatTaskFunc taskFunc;
    u16 pressedKeys;
    u16 heldKeysUnused;
    u16 pressedKeysRepeatable;
    int touchedButtonIndex;
    int unk_3C;
    TouchHitboxController *buttonAction;
    BOOL unk_44;
    BOOL unk_48;
    u16 unk_4C;
    u16 currentWordSlot;
    u16 categoryCursorPos;
    u16 prevCategoryCursorPos;
    UnkStruct_ov20_021D18BC unk_54;
    UnkStruct_ov20_021D0F64 unk_60;
    u8 unk_64;
    u8 mode;
    UnkStruct_ov20_021D1F34 unk_66;
};
typedef void (*UnkCallback)(void);
enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};
typedef struct System {
    Callback vblankCallback;
    void *vblankCallbackData;
    Callback hblankCallback;
    void *hblankCallbackData;
    UnkCallback dummyCallback_10;
    UnkCallback dummyCallback_14;
    SysTaskQueue *mainTaskMgr;
    SysTaskQueue *vBlankTaskMgr;
    SysTaskQueue *postVBlankTaskMgr;
    SysTaskQueue *printTaskMgr;
    u32 *unused_28;
    u32 vblankCounter;
    u32 frameCounter;
    enum ButtonMode buttonMode;
    u32 heldKeysRaw;
    u32 pressedKeysRaw;
    u32 pressedKeysRepeatableRaw;
    u32 heldKeys;
    u32 pressedKeys;
    u32 pressedKeysRepeatable;
    int autorepeatTimer;
    int autorepeatRate;
    int autorepeatDelay;
    u16 touchX;
    u16 touchY;
    u16 touchPressed;
    u16 touchHeld;
    u8 touchAutoSampling;
    u8 whichScreenIs3D;
    u8 gbaCartridgeVersion;
    u8 inhibitSleep;
    u8 inhibitReset;
    u8 padding_69[3];
    BOOL showTitleScreenIntro;
    u32 *heapCanary;
} System;
extern System gSystem;

BOOL EasyChat_Main(OverlayManager *appMan, int *state);

s32 ov102_021E940C(UnkStruct_ov20_021D2128 *, s32);
void ov102_021E77F8(u32 arg0, u32 arg1, void *arg2);

BOOL EasyChat_Main(OverlayManager *appMan, int *state) {
    EasyChatApp *easyChatApp = OverlayManager_GetData(appMan);

    easyChatApp->pressedKeys = gSystem.pressedKeys;
    easyChatApp->heldKeysUnused = gSystem.heldKeys;
    easyChatApp->pressedKeysRepeatable = gSystem.pressedKeysRepeatable;

    TouchHitboxController_IsTriggered(easyChatApp->buttonAction);

    if (easyChatApp->taskFunc != 0L) {
        easyChatApp->taskFunc(easyChatApp, &easyChatApp->taskFuncState);
        return 0;
    } else {
        return easyChatApp->mainFunc(easyChatApp, &easyChatApp->state);
    }
}

void ov102_021E77F8(u32 arg0, u32 arg1, void *arg2) {
    switch (arg1) {
    case 0:
        *(s32 *)((u8 *)arg2 + 0x38) = arg0;
        switch (arg0) {
        case 2:
            ov102_021E940C(*(void **)((u8 *)arg2 + 0x14), 0x1C);
            return;
        case 3:
            ov102_021E940C(*(void **)((u8 *)arg2 + 0x14), 0x1D);
            return;
        }
        break;
    case 1:
    case 3:
        switch (arg0) {
        case 2:
            ov102_021E940C(*(void **)((u8 *)arg2 + 0x14), 0x1E);
            break;
        case 3:
            ov102_021E940C(*(void **)((u8 *)arg2 + 0x14), 0x1F);
            break;
        }
        *(s32 *)((u8 *)arg2 + 0x38) = 4;
        return;
    case 2:
        if ((u32)(arg0 - 2) <= 1U) {
            *(s32 *)((u8 *)arg2 + 0x38) = arg0;
            return;
        }
        return;
    default:
        *(s32 *)((u8 *)arg2 + 0x38) = 4;
        break;
    }
}
