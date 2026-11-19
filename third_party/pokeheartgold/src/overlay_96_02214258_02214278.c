#include "global.h"

#include "bg_window.h"

s32 ov96_02214044(void *, u8, s32);

extern u8 ov96_0221D1F4[4];

void ov96_02214278(SysTask *task, void *param);

void ov96_02214278(SysTask *task, void *param) {
    u8 *arg1 = param;
    u8 spC[4];
    s32 temp;
    u8 var;

    switch (arg1[0]) {
    case 0:
        break;
    case 1:
        if (arg1[0xE] != 0) {
            if (arg1[0xF] != 0) {
                ov96_02214044(*(void **)(arg1 + 8), arg1[1], 1);
            } else {
                ov96_02214044(*(void **)(arg1 + 8), arg1[1], 2);
            }
        } else {
            ov96_02214044(*(void **)(arg1 + 8), arg1[1], 3);
        }
        arg1[2] = 0;
        arg1[0xD] = 0;
        arg1[0xC] = 0;
        arg1[0] = 2;
        break;
    case 2:
        arg1[2] += 1;
        if (arg1[2] > 0x1EU) {
            BgTilemapRectChangePalette(*(BgConfig **)((u8 *)*(void **)(arg1 + 8) + 4), 1U, (u8)(arg1[1] * 8), 0U, 8U, 2U, 1U);
            ScheduleBgTilemapBufferTransfer(*(BgConfig **)((u8 *)*(void **)(arg1 + 8) + 4), 1U);
            ov96_02214044(*(void **)(arg1 + 8), arg1[1], 0);
            *(s8 *)(arg1 + 3) = -1;
            arg1[0] = 0;
            break;
        }
        if (arg1[0xC] == 0) {
            u8 *hi = &spC[2];
            u8 *lo;
            spC[2] = ov96_0221D1F4[2];
            spC[3] = ov96_0221D1F4[3];
            lo = &spC[0];
            spC[0] = ov96_0221D1F4[0];
            spC[1] = ov96_0221D1F4[1];
            temp = arg1[0xD] + 1;
            arg1[0xD] = temp % 2;
            arg1[0xC] = 5;
            if (arg1[0xE] != 0) {
                var = hi[arg1[0xD]];
            } else {
                var = lo[arg1[0xD]];
            }
            BgTilemapRectChangePalette(*(BgConfig **)((u8 *)*(void **)(arg1 + 8) + 4), 1U, (u8)(arg1[1] * 8), 0U, 8U, 2U, var);
            ScheduleBgTilemapBufferTransfer(*(BgConfig **)((u8 *)*(void **)(arg1 + 8) + 4), 1U);
        } else {
            arg1[0xC] -= 1;
        }
        break;
    }
}
