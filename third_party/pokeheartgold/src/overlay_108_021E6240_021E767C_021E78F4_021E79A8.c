#include "global.h"
struct SafariAreaCustomizer;

#include "bg_window.h"
#include "overlay_108_safari_area_customizer.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E79A8(SafariAreaCustomizer *arg0, u8 arg1, s32 arg2, s32 arg3);

void ov108_021E79A8(SafariAreaCustomizer *arg0, u8 arg1, s32 arg2, s32 arg3) {
    u8 sp1C;
    u8 var_r6;
    NNSG2dScreenData *temp_r0;

    if (arg3 == 0) {
        BgClearTilemapBufferAndCommit(arg0->bgConfig, arg1);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, arg1);
        return;
    }
    temp_r0 = arg0->unk_4D4;
    CopyToBgTilemapRect(arg0->bgConfig, arg1, 0, (u8)(0x18 * arg0->unk_184E2_3), 0x20, 0x18, temp_r0->rawData, 0, 0, (u8)((u32)temp_r0->screenWidth >> 3), (u8)((u32)temp_r0->screenHeight >> 3));
    if (arg0->unk_184E2_3 == 0) {
        sp1C = (u8)(((arg2 % 3) * 9) + 3);
        var_r6 = (u8)(((arg2 / 3) * 9) + 3);
        if (arg1 == 4) {
            temp_r0 = arg0->unk_4D4;
            CopyToBgTilemapRect(arg0->bgConfig, arg1, 0, 0x15, 0x20, 3, temp_r0->rawData, 0, 0x18, (u8)((u32)temp_r0->screenWidth >> 3), (u8)((u32)temp_r0->screenHeight >> 3));
        }
    } else {
        sp1C = (u8)((0xA * (arg2 % 3)) + 2);
        var_r6 = (u8)(((arg2 / 3) * 9) + 0x1B);
    }
    FillBgTilemapRect(arg0->bgConfig, arg1, 0, sp1C, var_r6, 8, 8, 0x11);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, arg1);
}

struct BgConfig;

void ov108_021E7ADC(SafariAreaCustomizer *arg0);

void ov108_021E7ADC(SafariAreaCustomizer *arg0) {
    s32 i;
    s32 j;
    s32 k;
    s32 y;
    u8 x;
    u32 tile = 1;

    for (i = 0; i < 6; i++) {
        j = 0;
        y = (u8)(((i / 3) * 9) + 3);
        x = (u8)(((i % 3) * 9) + 3);
        for (; j < 8; j++) {
            for (k = 0; k < 8; k++) {
                FillBgTilemapRect(arg0->bgConfig, 5, (u16)tile++, (u8)(x + k), (u8)y, 1, 1, 0x11);
            }
            y++;
        }
    }
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 5);
}
