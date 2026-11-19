#include "global.h"

#include "sprite_system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_struct_02089688.h"
#include "options.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "sys_task.h"
#include "screen_fade_internal.h"

struct BgConfig;

struct CharTransferAllocation;

struct FontOAM;

struct ManagedSprite;

struct NARC;

struct Options;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct SysTask;

struct SysTaskManager;

struct System;

struct TouchScreenActions;

struct UnkStruct_02012744_t;

struct Window;

typedef struct {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C;
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} UnkStruct_02089438;

typedef struct FontOAM FontOAM;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchScreenHitTable;

typedef struct TouchScreenActions TouchScreenActions;

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    BgConfig *unk_0C;
    PaletteData *unk_10;
    TouchScreenActions *unk_14;
    TouchScreenRect unk_18[28];
    BOOL unk_88;
    UnkStruct_02012744 *unk_8C;
    FontOAM *unk_90[2];
    CharTransferAllocation unk_98[2];
    Window unk_B0;
} UnkStruct_02089688_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02089688_sub2;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_0208AF44_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    BOOL unk_08;
    ManagedSprite *unk_0C;
    TouchScreenRect *unk_10;
    UnkStruct_0208AF44_sub1 unk_14;
} UnkStruct_0208AF44;

void sub_02085F80(UnkStruct_02089688 *param0);

void sub_02085FFC(UnkStruct_02089688 *param0);

typedef struct SysTaskManager SysTaskManager;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

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
    SysTaskManager *mainTaskQueue;
    SysTaskManager *vblankTaskQueue;
    SysTaskManager *vwaitTaskQueue;
    SysTaskManager *printTaskQueue;
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

void sub_02086AB4(UnkStruct_02089688 *param0, int param1, BOOL param2);

void sub_02086B2C(UnkStruct_02089688 *param0, int param1);

void sub_02086B6C(UnkStruct_02089688 *param0, int param1, int param2);

int sub_02086D98(int param0, BOOL param1);

void sub_02086180(u32 param0, enum TouchScreenButtonState param1, void *param2);

void sub_02085C20(UnkStruct_02089688 *param0);

void sub_02085C20(UnkStruct_02089688 *param0) {
    const int Unk_020F2EC8[][5] = {
        { 0,  1,  2,  3,  4  },
        { 5,  6,  7,  8,  9  },
        { 10, 10, 10, 11, 11 },
    };
    BOOL v1 = FALSE;
    int v2 = Unk_020F2EC8[param0->unk_214[1].unk_14.unk_02][param0->unk_214[1].unk_14.unk_00];

    if (param0->unk_2C0 != 1 || param0->unk_3AC.unk_00 == 1) {
        return;
    }

    if (param0->unk_2EC.unk_88 == TRUE) {
        if (gSystem.newKeys && !System_GetTouchHeld()) {
            param0->unk_2EC.unk_88 = FALSE;
            sub_02086B2C(param0, v2);
            if (v2 == 10 || v2 == 11) {
                if (param0->unk_214[1].unk_00 != 2) {
                    param0->unk_214[1].unk_00 = 2;
                }
            } else {
                if (param0->unk_214[1].unk_00 != 1) {
                    param0->unk_214[1].unk_00 = 1;
                }
            }
        }
        return;
    }

    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (param0->unk_214[1].unk_14.unk_02 > 0) {
            param0->unk_214[1].unk_14.unk_02--;
        } else {
            param0->unk_214[1].unk_14.unk_02 = 2;
        }
        v1 = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        param0->unk_214[1].unk_14.unk_02++;
        param0->unk_214[1].unk_14.unk_02 %= 3;
        v1 = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {

        if (v2 == 10) {
            param0->unk_214[1].unk_14.unk_00 = 3;
        } else if (v2 == 11) {
            param0->unk_214[1].unk_14.unk_00 = 0;
        } else {
            param0->unk_214[1].unk_14.unk_00++;
            param0->unk_214[1].unk_14.unk_00 %= 5;
        }
        v1 = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {

        if (v2 == 10) {
            param0->unk_214[1].unk_14.unk_00 = 3;
        } else if (v2 == 11) {
            param0->unk_214[1].unk_14.unk_00 = 0;
        } else {
            if (param0->unk_214[1].unk_14.unk_00 > 0) {
                param0->unk_214[1].unk_14.unk_00--;
            } else {
                param0->unk_214[1].unk_14.unk_00 = 4;
            }
        }
        v1 = TRUE;
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        int v3;
        int v4;
        int v5;

        if (v2 == 10) {
            sub_02085FFC(param0);

            PlaySE(1509);
        } else if (v2 == 11) {
            sub_02085F80(param0);

            PlaySE(1506);
        } else {
            if (param0->unk_2D4 == 0) {
                return;
            }

            v3 = param0->unk_214[0].unk_00;
            param0->unk_00[v3].unk_00 = v2 + 1;
            sub_02086AB4(param0, 1, FALSE);
            sub_02086AB4(param0, 2, TRUE);
            sub_02086B6C(param0, v2, 2);
            ManagedSprite_SetAnim(param0->unk_00[v3].unk_0C, sub_02086D98(param0->unk_00[v3].unk_00, param0->unk_00[v3].unk_08));
            ManagedSprite_SetAnim(param0->unk_214[2].unk_0C, 3);

            v4 = param0->unk_00[v3].unk_04;
            v3++;
            if (v3 == param0->unk_2D0) {

                param0->unk_3AC.unk_00 = 1;
                param0->unk_3AC.unk_04 = 0;
                param0->unk_214[1].unk_14.unk_00 = 3;
                param0->unk_214[1].unk_14.unk_02 = 2;
                v1 = TRUE;
            } else {
                v5 = param0->unk_00[v3].unk_04;

                if (v4 != v5) {

                    param0->unk_3AC.unk_00 = 1;
                    param0->unk_3AC.unk_04 = v5;
                } else {
                    param0->unk_3AC.unk_00 = 2;
                    param0->unk_3AC.unk_04 = v3;
                }
                PlaySE(1509);
            }
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        sub_02085FFC(param0);
        PlaySE(1509);
    } else if (gSystem.newAndRepeatedKeys & PAD_BUTTON_L) {
        int v6 = param0->unk_214[0].unk_00;

        if (v6 == param0->unk_3F0) {
            param0->unk_214[0].unk_00 = param0->unk_2D0 - 1;
        } else {
            param0->unk_214[0].unk_00--;
        }
        v6 = param0->unk_214[0].unk_00;

        if (param0->unk_00[v6].unk_08 == 1) {
            param0->unk_3AC.unk_00 = 2;
            param0->unk_3AC.unk_04 = v6;
        } else {
            param0->unk_3AC.unk_00 = 1;
            param0->unk_3AC.unk_04 = param0->unk_00[v6].unk_04;
            param0->unk_3AC.unk_08 = 1;
        }
        PlaySE(1504);
    } else if (gSystem.newAndRepeatedKeys & PAD_BUTTON_R) {
        int v7 = param0->unk_214[0].unk_00;

        if (v7 == param0->unk_2D0 - 1) {
            param0->unk_214[0].unk_00 = param0->unk_3F0;
        } else {
            param0->unk_214[0].unk_00++;
        }
        v7 = param0->unk_214[0].unk_00;

        if (param0->unk_00[v7].unk_08 == 1) {
            param0->unk_3AC.unk_00 = 2;
            param0->unk_3AC.unk_04 = v7;
        } else {
            param0->unk_3AC.unk_00 = 1;
            param0->unk_3AC.unk_04 = param0->unk_00[v7].unk_04;
        }
        PlaySE(1504);
    }

    if (v1 == TRUE) {
        PlaySE(1504);

        v2 = Unk_020F2EC8[param0->unk_214[1].unk_14.unk_02][param0->unk_214[1].unk_14.unk_00];
        sub_02086B2C(param0, v2);

        if (v2 == 10 || v2 == 11) {
            if (param0->unk_214[1].unk_00 != 2) {
                param0->unk_214[1].unk_00 = 2;
            }
        } else {
            if (param0->unk_214[1].unk_00 != 1) {
                param0->unk_214[1].unk_00 = 1;
            }
        }
    }
}
