#include "global.h"

extern void VCT_Main(void);

extern int ov00_021E6964(void);

extern BOOL ov00_021E74A8(u8 arg);

extern BOOL ov00_021E74FC(int arg);

extern u32 _u32_div_f(u32 a, u32 b);

typedef struct {
    s32 timeToProcess;
    u8 *manager;
    u64 previousTick;
} UnkStruct_ov00_021E765C;

extern UnkStruct_ov00_021E765C _0221A684;

void ov00_021E765C(void);

void ov00_021E765C(void) {
    u64 tick = OS_GetTick();

    _0221A684.timeToProcess += (s32)((((u32)tick - (u32)_0221A684.previousTick) * 0xFA00) / 0x82EA - 0x411A);
    if (_0221A684.timeToProcess < -10000) {
        _0221A684.timeToProcess = 0;
    }

    _0221A684.previousTick = tick;
    VCT_Main();

    while (_0221A684.timeToProcess >= 0x411A) {
        VCT_Main();
        _0221A684.timeToProcess -= 0x411A;
    }

    if ((*(u32 *)((_0221A684.manager) + (0x1A80))) != 0) {
        switch (*(u32 *)((_0221A684.manager) + (0x1A7C))) {
        case 0:
            PM_SetAmpGain(PM_AMPGAIN_160);
            break;
        case 1:
            PM_SetAmpGain(PM_AMPGAIN_80);
            break;
        case 2:
            PM_SetAmpGain(PM_AMPGAIN_40);
            break;
        case 3:
            PM_SetAmpGain(PM_AMPGAIN_20);
            break;
        }
        (*(u32 *)((_0221A684.manager) + (0x1A80))) = 0;
    }

    if ((*(u32 *)((_0221A684.manager) + (0x19E8))) != 3) {
        switch (*(u32 *)((_0221A684.manager) + (0x19EC))) {
        case 0:
            if (ov00_021E6964() == 0) {
                if (ov00_021E74A8(1)) {
                    (*(u32 *)((_0221A684.manager) + (0x19EC))) = 1;
                    (*(u16 *)((_0221A684.manager) + (0x1A5A))) = 60;
                }
            } else if (ov00_021E6964() == 1) {
                if (ov00_021E74FC(0)) {
                    (*(u32 *)((_0221A684.manager) + (0x19EC))) = 2;
                }
            }
            break;
        case 1:
            (*(u16 *)((_0221A684.manager) + (0x1A5A))) = (*(u16 *)((_0221A684.manager) + (0x1A5A))) - 1;
            if ((*(u16 *)((_0221A684.manager) + (0x1A5A))) == 0) {
                (*(u32 *)((_0221A684.manager) + (0x19EC))) = 0;
            }
            break;
        case 2:
            break;
        }
    }
}
