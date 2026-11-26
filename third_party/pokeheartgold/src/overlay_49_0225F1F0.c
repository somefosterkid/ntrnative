#include "global.h"

#include "overlay_49_getters.h"
#include "sys_task.h"
#include "screen_fade_internal.h"

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02263344_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;



typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);

typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;

void ov49_0225EF98(UnkStruct_ov70_022630A4 *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);

extern const UnkStruct_ov70_0226DA18 ov49_02269B78;

void ov49_0225F1F0(UnkStruct_ov70_0225DEE8 *param0);

struct SysTask;
struct SysTaskQueue;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
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
BOOL ov49_0225F224(int param0);
void ov49_0225EF68(UnkStruct_ov70_02263344 *param0);
void *ov49_0225EF84(UnkStruct_ov70_02263344 *param0);
void ov49_0225EFC4(UnkStruct_ov70_022630A4 *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);
typedef struct {
    u32 unk_00;
} UnkStruct_ov70_0226315C;
typedef void (*UnkFuncPtr_ov70_0226DBB8)(UnkStruct_ov70_0226315C *, UnkStruct_ov70_0225DEE8 *, u32, u32);
typedef struct {
    u8 unk_00[8];
    u32 unk_08;
    UnkStruct_ov70_0226DA18 unk_0C;
    UnkFuncPtr_ov70_0226DBB8 unk_14;
} UnkStruct_ov70_0226DBB8;
void ov49_0225F260(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, const UnkStruct_ov70_0226DBB8 *param3, u32 param4);

void ov49_0225F1F0(UnkStruct_ov70_0225DEE8 *param0) {
    int v0;
    u32 v1;
    UnkStruct_ov70_022630A4 *v2;

    v1 = ov49_0225A02C(param0);
    v2 = ov49_0225A010(param0);

    for (v0 = 0; v0 < 20; v0++) {
        if (v1 != v0) {
            ov49_0225EF98(v2, v0, &ov49_02269B78, NULL);
        }
    }
}

BOOL ov49_0225F224(int param0) {
    u32 v0;

    switch (param0) {
    case 0:
        v0 = PAD_KEY_UP;
        break;
    case 1:
        v0 = PAD_KEY_DOWN;
        break;
    case 2:
        v0 = PAD_KEY_LEFT;
        break;
    case 3:
        v0 = PAD_KEY_RIGHT;
        break;
    }

    if (gSystem.heldKeys & v0) {
        return 1;
    }

    return 0;
}

void ov49_0225F260(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, const UnkStruct_ov70_0226DBB8 *param3, u32 param4) {
    int v0;
    int v1;
    UnkStruct_ov70_0226315C *v2;
    UnkStruct_ov70_022630A4 *v3;

    v2 = ov49_0225EF84(param0);

    {
        v3 = ov49_0225A010(param1);
    }

    v0 = 0;

    while (param3[v0].unk_08 != 3) {
        for (v1 = 0; v1 < 8; v1++) {
            if (param3[v0].unk_00[v1] == 0xff) {
                break;
            }

            if (param3[v0].unk_00[v1] == param4) {
                switch (param3[v0].unk_08) {
                case 0:
                    ov49_0225EF68(param0);
                    ov49_0225EF98(v3, param2, &param3[v0].unk_0C, 0L);
                    break;
                case 1:
                    if (param3[v0].unk_14) {
                        param3[v0].unk_14(v2, param1, param2, param3[v0].unk_00[v1]);
                    }

                    ov49_0225EFC4(v3, param2, &param3[v0].unk_0C, 0L);
                    break;
                }

                return;
            }
        }

        v0++;
    }

    ((0) ? (void)0 : GF_AssertFail());
    return;
}
