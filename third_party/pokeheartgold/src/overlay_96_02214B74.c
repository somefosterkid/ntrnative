#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "heap.h"
#include "overlay_96_app_02210C98.h"

void ov96_02214B74(Ov96App_02210C98_Cpu *cpu, Ov96App_02210C98_Mover *movers);

void ov96_02214B74(Ov96App_02210C98_Cpu *cpu, Ov96App_02210C98_Mover *movers) {
    cpu->movers = movers;
    return;
}

void ov96_02214B7C(Ov96App_02210C98_Cpu *cpu);

void ov96_02214B7C(Ov96App_02210C98_Cpu *cpu) {
    Heap_Free(cpu);
}

void ov96_022155A0(PokeathlonCourseData *data, Ov96App_02210C98_Cpu *cpu, Ov96App_02210C98_CpuSlot *slot);

void ov96_02214B84(PokeathlonCourseData *data, Ov96App_02210C98_Cpu *cpu);

void ov96_02214B84(PokeathlonCourseData *data, Ov96App_02210C98_Cpu *cpu) {
    s32 timer;
    Ov96App_02210C98_CpuPlayer *player;

    if (cpu->numHuman != 4) {
        cpu->tick = (u8)(cpu->tick + 1);
        if ((u32)cpu->tick >= (u32)cpu->interval) {
            cpu->running = TRUE;
            cpu->tick = 0U;
        }
        if (cpu->running != FALSE) {
            player = &cpu->players[cpu->firstCpu + cpu->cur];
            timer = player->slots[cpu->slot].timer;
            if (timer == 0) {
                ov96_022155A0(data, cpu, &player->slots[cpu->slot]);
            } else {
                player->slots[cpu->slot].timer = timer - 1;
            }
            cpu->cur = (u8)(cpu->cur + 1);
            if ((u32)cpu->cur >= (u32)cpu->numCpu) {
                cpu->cur = 0U;
                cpu->slot = (u8)((s32)(cpu->slot + 1) % 3);
                cpu->running = FALSE;
            }
        }
    }
}
