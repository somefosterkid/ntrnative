#include "global.h"
#include "overlay_83_battle_castle_self_app.h"
#include "battle_castle_app_sprite.h"
struct BattleCastleSelfApp;

void ov83_02241E18(BattleCastleSelfApp *arg0);

s32 ov83_022421E0(void *, s32);

void ov83_02247668(BattleCastleAppSprite *obj, struct BoxPokemon *boxMon, int species, u32 personality);

void ov83_02242814(struct BattleCastleSelfApp *arg0, s32 arg1);

void ov83_02242814(BattleCastleSelfApp *arg0, s32 arg1) {
    ov83_02241E18(arg0);
    ov83_022421E0(arg0, arg1);
    ov83_02247668(arg0->selectedMonSprite, arg0->boxMon, arg0->species, arg0->personality);
}
