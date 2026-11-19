#include "global.h"

#include "constants/heap.h"

#include "pm_string.h"
#include "sys_task.h"
#include "screen_scroll_manager.h"
#include "screen_shake_effect.h"

struct ScreenScrollManager;

void sub_02014B08(ScreenScrollManager *screenScrollMgr, u8 start, u8 end, u16 angleIncrement, fx32 amplitude, s16 speed, void *bg, u32 initValue, u32 priority);

void ov01_021F12E8(ScreenShakeEffect *screenShake, u8 startX, u8 endX, u16 angleIncrement, fx32 amplitude, s16 shakeSpeed, void *bg, u32 defaultValue, u32 priority);

void *sub_02014BF8(ScreenScrollManager *screenScrollMgr);
void ov01_021F1310(ScreenShakeEffect *screenShake, u32 interval);

void ov01_021F12E8(ScreenShakeEffect *screenShake, u8 startX, u8 endX, u16 angleIncrement, fx32 amplitude, s16 shakeSpeed, void *bg, u32 defaultValue, u32 priority) {
    sub_02014B08(screenShake->screenScrollMgr, startX, endX, angleIncrement, amplitude, shakeSpeed, bg, defaultValue, priority);
}

void ov01_021F1310(ScreenShakeEffect *screenShake, u32 interval) {
    u32 *writeBuffer = sub_02014BF8(screenShake->screenScrollMgr);

    for (int i = 0; i < 192; i++) {
        s16 xOff = writeBuffer[i] & 0xffff;

        if (((i / interval) % 2) == 0) {
            writeBuffer[i] = (xOff & 0xffff);
        } else {
            writeBuffer[i] = (-xOff & 0xffff);
        }
    }
}
