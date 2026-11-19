#include "global.h"
#include "overlay_113.h"

#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"

s32 ov113_021E6238(UnownReportData *, UnownReportScreen *, u16);

s32 ov113_021E629C(Window *, String *, s32, s32);

s32 ov113_021E6AE8(UnownReportData *, s32, s32);

void ov113_021E6084(UnownReportData *arg0, s32 arg1);

void ov113_021E6084(UnownReportData *arg0, s32 arg1) {
    Window *spC;
    Window *sp8;
    s32 sp4;
    struct ManagedSprite **sp0;
    s32 temp_r7;
    s32 var_r4;
    s32 var_r6;

    ov113_021E6238(arg0, &arg0->screens[4], 6);
    ov113_021E6238(arg0, &arg0->screens[1], 2);
    sp4 = 0xE * arg1;
    spC = &arg0->windows[2];
    var_r4 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    sp0 = arg0->monIcons;
#else
    sp0 = (struct ManagedSprite **)arg0;
#endif
    var_r6 = 0;
    sp8 = &arg0->windows[1];
    do {
        temp_r7 = sp4 + var_r4;
        if (temp_r7 >= (s32)arg0->numFormsSeen) {
#if defined(SDK_PORT) || defined(NONMATCHING)
            ManagedSprite_SetDrawFlag(*sp0, 0);
#else
            ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)sp0 + offsetof(UnownReportData, monIcons)), 0);
#endif
        } else {
#if defined(SDK_PORT) || defined(NONMATCHING)
            ManagedSprite_SetDrawFlag(*sp0, 1);
#else
            ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)sp0 + offsetof(UnownReportData, monIcons)), 1);
#endif
            ov113_021E6AE8(arg0, var_r4, arg0->forms[temp_r7]);
            ReadMsgDataIntoString(arg0->msgData, arg0->forms[temp_r7] + 0x14, arg0->strBuf2);
            if (var_r4 < 7) {
                ov113_021E629C(sp8, arg0->strBuf2, 0x30, (u16)var_r6);
            } else {
                ov113_021E629C(spC, arg0->strBuf2, 0x30, (u16)(s32)(var_r6 - 0xA8));
            }
        }
        var_r4 += 1;
        var_r6 += 0x18;
        sp0 += 1;
    } while (var_r4 < 0xE);
}
