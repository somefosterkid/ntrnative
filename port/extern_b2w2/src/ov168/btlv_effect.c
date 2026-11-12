// Black 2's and White 2's battle idle camera (port/extern/src/ov094/btlv_effect.c's twin; the maintainers' notes, 6.):
// pokebw2's src/ov168/btlv_effect.c compiled through this file. At the command menu the game turns on its idle camera
// (BtlvEffect_SetIdleEffectMode(2), src/ov168/btlv_input.c): after idleEffectWait ticks (900, 15 s on a DS; 300 in the
// short-wait battle types) with the menu up, BtlvEffect_UpdateIdleEffect starts camera pans picked at random, until the
// player chooses. Battle speed runs the battle's ticks up to 4x, so those pans would begin while the player is still
// choosing. So while the idle effect is on (the game waits for the player's command), the battle runs at the player's own
// speed (port/src/ovl.c): the menu and its idle camera keep the DS's timing, and the turn that follows runs at Battle
// speed again. The DS build never sees this file.
#define BtlvEffect_Main BtlvEffect_Main_game
#include "../../../../third_party/pokebw2/src/ov168/btlv_effect.c"
#undef BtlvEffect_Main

void PortOvl_SetBattleIdle(BOOL on);
void BtlvEffect_Main(void);

void BtlvEffect_Main(void) {
    BtlvEffect_Main_game();
    PortOvl_SetBattleIdle(sBtlvEffect != NULL && sBtlvEffect->idleEffectMode != 0);
}
