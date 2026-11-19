#include "global.h"
struct PokeathlonMedals;

#include "sprite_system.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_99_pokeathlon_medals.h"

void ov99_021E7C58(struct PokeathlonMedals *arg0, s32 arg1);

void ov99_021E7C58(PokeathlonMedals *arg0, s32 target) {
    u8 py[2];

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->grid, target), &py[1], &py[0]);
    ManagedSprite_SetPositionXY(arg0->sprites[0], (s16) py[1], (s16) py[0]);
    *((s32 *)&arg0->flags) = (s32)((*((s32 *)&arg0->flags) & 0xF807FFFF) | ((u32)(target << 0x18) >> 5));
    PlaySE(0x5DCU);
}
