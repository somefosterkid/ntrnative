#include "global.h"

#include "overlay_31.h"
#include "screen_fade.h"
#include "sys_task.h"

void ov31_0225D7A0(SysTask *task, void *arg1);

void ov31_0225D7A0(SysTask *task, void *arg1) {
    MartSubScreen *screen = arg1;
    s32 temp_r0, temp_r1;
    s32 temp_r0_3;
    s32 temp_r0_4;
    u8 temp_r0_2;

    temp_r0 = screen->state;
    switch (temp_r0) {
    case 0:
        if (IsPaletteFadeFinished() == 1) {
            screen->state = 1;
            return;
        }
        return;
    case 1:
        temp_r0_2 = screen->martData->state;
        switch (temp_r0_2) {
        case 3:
            temp_r0_3 = ov31_0225DAA4(screen, 0);
            if ((temp_r0_3 != -1) && (ov31_0225D75C(screen, temp_r0_3) != 0)) {
                screen->martData->queuedButtonPress = temp_r0_3;
            }
            break;
        case 7:
            temp_r0_4 = ov31_0225DAA4(screen, 1);
            if (temp_r0_4 != -1) {
                screen->martData->queuedButtonPress = temp_r0_4;
            }
            break;
        case 11:
            ov31_0225E774(screen);
            break;
        }
        temp_r1 = screen->martData->unk298;
        if (temp_r1 != 0) {
            ov31_0225D83C(screen, temp_r1);
            screen->martData->unk298 = 0;
        }
        break;
    }
}
