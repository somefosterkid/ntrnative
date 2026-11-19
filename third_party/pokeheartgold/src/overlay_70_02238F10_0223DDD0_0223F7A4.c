#include "global.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_0223F6E4();

void ov70_0223F7A4(void *arg0, GTSApplicationState *arg1);

void ov70_0223F7A4(void *arg0, GTSApplicationState *arg1) {
    ov70_0223F6E4();
    *(u16 *)((u8 *)arg0 + 0xEC) = (u16) *((u16 *)&arg1->offeredPokemonCriteria.species);
    *(u16 *)((u8 *)arg0 + 0xEE) = (u16) *((u16 *)&arg1->offeredPokemonCriteria.gender);
    *(u16 *)((u8 *)arg0 + 0xF0) = (u16) *((u16 *)&arg1->depositRequirements.species);
    *(u16 *)((u8 *)arg0 + 0xF2) = (u16) *((u16 *)&arg1->depositRequirements.gender);
    *(u16 *)((u8 *)arg0 + 0xF4) = (u16) *((u16 *)&arg1->depositRequirements.level2);
}
