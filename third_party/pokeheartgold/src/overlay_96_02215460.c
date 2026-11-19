#include "global.h"

#include "overlay_96_app_02210C98.h"

s32 ov96_02215058(Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Mover *mover, u32 action, s32 player);
s32 ov96_02215478(Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Cpu *cpu, u8 skip);

s32 ov96_02215460(Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Mover *mover, s32 action, u8 player, Ov96App_02210C98_Cpu *cpu);

s32 ov96_02215460(Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Mover *mover, s32 action, u8 player, Ov96App_02210C98_Cpu *cpu) {
    if (action == 10) {
        return ov96_02215478(unit, cpu, player);
    }
    return ov96_02215058(unit, mover, action, player);
}
