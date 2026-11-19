#include "global.h"

#include "overlay_96_app_02210C98.h"

void ov96_02214ABC(Ov96App_02210C98_CpuPlayer *player);

void ov96_02214A9C(Ov96App_02210C98_Cpu *cpu);

void ov96_02214A9C(Ov96App_02210C98_Cpu *cpu) {
    u32 i;

    i = 0;
    do {
        ov96_02214ABC(&cpu->players[i]);
        i = i + 1 & 0xff;
    } while (i < 4);
    return;
}
