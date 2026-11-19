#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_app_02210C98.h"

s32 ov96_02214C3C(PokeathlonCourseData *data, u8 player, u8 slot, Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Mover *movers);
u32 ov96_02214EB4(PokeathlonCourseData *data, u8 player, u8 slot, Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Mover *movers);
u32 ov96_02215184(PokeathlonCourseData *data, u8 player, u8 slot, Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Mover *movers, Ov96App_02210C98_Cpu *cpu);

void ov96_022155A0(PokeathlonCourseData *data, Ov96App_02210C98_Cpu *cpu, Ov96App_02210C98_CpuSlot *slot);

void ov96_022155A0(PokeathlonCourseData *data, Ov96App_02210C98_Cpu *cpu, Ov96App_02210C98_CpuSlot *slot) {
    u8 v0;
    u8 v1;

    v1 = cpu->firstCpu + cpu->cur;
    v0 = cpu->slot;
    switch (v0) {
    case 0:
        slot->timer = ov96_02214EB4(data, v1, v0, slot->unit, cpu->movers);
        return;
    case 1:
        slot->timer = ov96_02214C3C(data, v1, v0, slot->unit, cpu->movers);
        return;
    case 2:
        slot->timer = ov96_02215184(data, v1, v0, slot->unit, cpu->movers, cpu);
        return;
    default:
        GF_AssertFail();
        return;
    }
}

s32 ov96_02215614(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov96_02215614(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if ((arg2 < arg0) && (arg3 < arg1)) {
        return 0;
    }
    if ((arg2 >= arg0) && (arg3 < arg1)) {
        return 1;
    }
    if ((arg2 < arg0) && (arg3 >= arg1)) {
        return 2;
    }
    if ((arg2 >= arg0) && (arg3 >= arg1)) {
        return 3;
    }
    GF_AssertFail();
    return 4;
}

u32 ov96_02215650(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u8 param_5, u8 param_6);

u32 ov96_02215650(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u8 param_5, u8 param_6) {
    u32 uVar1;
    u32 uVar2;

    uVar1 = ov96_02215614(param_1, param_2, param_3, param_4);
    uVar2 = (u32)param_6;
    if (uVar2 != param_5) {
        uVar2 = 3 - param_5 & 0xff;
    }
    if (uVar1 == uVar2) {
        return 1;
    }
    return 0;
}
