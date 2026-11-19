#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

extern u8 ov18_021FB618;

void ov18_021F7444(PokedexAppData *arg0, s32 arg1);

void ov18_021F7444(PokedexAppData *pokedexApp, s32 target) {
    u8 py[2];
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->unk_1864, target), &py[1], &py[0]);
    if (target == 0) {
        py[1] = (u8)ov18_021F3AD0(pokedexApp, pokedexApp->dexSearchCriteria[5]);
    } else if (target == 1) {
        py[1] = (u8)ov18_021F3AD0(pokedexApp, (s32)pokedexApp->dexSearchCriteria[4]);
    }
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[0], (s16)(py[1]), (s16)(py[0]));
    ov18_021F118C(pokedexApp, 0, (s32)(*((u8 *)((&ov18_021FB618) + target))));
}
