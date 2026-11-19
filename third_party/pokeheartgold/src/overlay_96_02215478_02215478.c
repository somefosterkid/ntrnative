#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "overlay_96_app_02210C98.h"

int ov96_02215614(int x0, int y0, int x, int y);

int ov96_02215478(Ov96App_02210C98_Unit *work, Ov96App_02210C98_Cpu *cpu, u8 skip);

int ov96_02215478(Ov96App_02210C98_Unit *work, Ov96App_02210C98_Cpu *cpu, u8 skip) {
    u8 i;
    u8 j;
    u8 best;
    u8 counts[4] = { 0 };
    Ov96App_02210C98_Unit *obj;
    int area;
    int dx;
    int dy;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            obj = cpu->players[i].slots[j].unit;
            area = ov96_02215614(128, 96, obj->unk_30.x / FX32_ONE, obj->unk_30.y / FX32_ONE);
            if (area != 4) {
                counts[area]++;
            }
        }
    }
    best = 0;
    for (j = 1; j < 4; j++) {
        if (counts[j - 1] > counts[j] && j != skip) {
            best = j;
        }
    }
    dx = (int)LCRandom() % 64;
    dy = (int)LCRandom() % 32;
    switch (best) {
    case 0:
        work->unk_24.x = (128 - dx) * FX32_ONE;
        work->unk_24.y = (96 - dy) * FX32_ONE;
        break;
    case 1:
        work->unk_24.x = (128 + dx) * FX32_ONE;
        work->unk_24.y = (96 - dy) * FX32_ONE;
        break;
    case 2:
        work->unk_24.x = (128 - dx) * FX32_ONE;
        work->unk_24.y = (96 + dy) * FX32_ONE;
        break;
    case 3:
        work->unk_24.x = (128 + dx) * FX32_ONE;
        work->unk_24.y = (96 + dy) * FX32_ONE;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return 10;
}
