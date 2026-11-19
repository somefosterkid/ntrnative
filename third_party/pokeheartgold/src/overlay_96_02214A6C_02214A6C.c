#include "global.h"

#include "overlay_96_app_02210C98.h"

void ov96_02214A6C(Ov96App_02210C98_Cpu *cpu, int idx, Ov96App_02210C98_Unit *unit);

void ov96_02214A6C(Ov96App_02210C98_Cpu *cpu, int idx, Ov96App_02210C98_Unit *unit) {
    u8 row = idx / 3;
    u8 col = idx % 3;
    cpu->players[row].units[col] = unit;
}
