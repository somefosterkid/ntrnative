#include "global.h"
#include "sys_task.h"
#include "screen_fade_internal.h"

struct SysTask;
struct SysTaskQueue;
struct System;
void GF_RTC_CopyDateTime(RTCDate *date, RTCTime *time);
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

void sub_0203778C(MATHRandContext32 *rand);

void sub_0203778C(MATHRandContext32 *rand) {
    u64 seed = 0;
    RTCDate date;
    RTCTime time;

    GF_RTC_CopyDateTime(&date, &time);
    seed = (((((((u64)date.year * 16ULL + date.month) * 32ULL) + date.day) * 32ULL + time.hour) * 64ULL + time.minute) * 64ULL + (time.second + gSystem.vblankCounter));
    MATH_InitRand32(rand, seed);
}
