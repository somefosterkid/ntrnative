#include "global.h"
#include "bg_window.h"
#include "overlay_28.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
#define GRID_ROW0(app) ((app)->signalGrid)
#define CELL(p)        ((p)[0])
#else
// The DS code walks from the start of the struct and adds the grid offset to every access.
#define GRID_ROW0(app) ((u8 *)(app))
#define CELL(p)        ((p)[offsetof(DowsingMchnApp, signalGrid)])
#endif

extern u16 ov28_0225EB14[];
extern const u16 ov28_0225EB32[];

void ov28_0225E578(DowsingMchnApp *app, BOOL arg1) {
    u16 *sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    s32 sp10;
    s16 var_r5;
    s16 var_r7;
    u8 *var_r3;
    u8 *var_r4;
    u32 temp_r0;

    sp10 = arg1;
    if (sp10 == 1) {
        sp30 = (s16)((app->touchX >> 3) - 3);
        sp24 = (s16)((app->touchY >> 3) - 5);
    }
    sp34 = ov28_0225EB14;
    var_r7 = 0;
    var_r4 = GRID_ROW0(app);
    sp2C = sp30 - 3;
    sp28 = sp30 + 3;
    sp20 = sp24 - 3;
    sp1C = sp24 + 3;
    for (; var_r7 < 0xF; var_r7++) {
        for (var_r5 = 0; var_r5 < 0xF; var_r5++) {
            if (((1 << var_r5) & *sp34) == 0) {
                if (sp10 == 1) {
                    if (var_r5 >= sp2C && var_r5 <= sp28 && var_r7 >= sp20 && var_r7 <= sp1C && abs(sp30 - var_r5) + abs(sp24 - var_r7) <= 4) {
                        temp_r0 = ov28_0225E51C(app, (u16)var_r5, (u16)var_r7);
                        var_r3 = var_r4 + var_r5;
                        if (CELL(var_r3) < temp_r0) {
                            CELL(var_r3) += 0xA;
                            if (CELL(var_r3) > temp_r0) {
                                CELL(var_r3) = (u8)temp_r0;
                            }
                        } else {
                            CELL(var_r3) = (u8)temp_r0;
                        }
                    } else {
                        var_r3 = var_r4 + var_r5;
                        if (CELL(var_r3) >= 2) {
                            CELL(var_r3) -= 2;
                        } else {
                            CELL(var_r3) = 0;
                        }
                    }
                } else {
                    var_r3 = var_r4 + var_r5;
                    if (CELL(var_r3) >= 2) {
                        CELL(var_r3) -= 2;
                    } else {
                        CELL(var_r3) = 0;
                    }
                }
                if (CELL(var_r3) == 0) {
                    FillBgTilemapRect(app->bgConfig, 6, 0x1001, (u8)(var_r5 + 3), (u8)(var_r7 + 5), 1, 1, 0x11);
                } else {
                    FillBgTilemapRect(app->bgConfig, 6, ov28_0225EB32[CELL(var_r3) / 10], (u8)(var_r5 + 3), (u8)(var_r7 + 5), 1, 1, 0x11);
                }
            }
        }
        var_r4 += 0xF;
        sp34++;
    }
    ScheduleBgTilemapBufferTransfer(app->bgConfig, 6);
}
