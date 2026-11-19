#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

extern u8 ov18_021FB628;

void ov18_021F6F38(PokedexAppData *arg0, s32 arg1);

void ov18_021F6F38(PokedexAppData *pokedexApp, s32 target) {
    u8 py[2];
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target), &py[1], &py[0]);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[0], (s16)(py[1]), (s16)(py[0]));
    ov18_021F118C(pokedexApp, 0, (s32)(*((u8 *)((&ov18_021FB628) + target))));
}
