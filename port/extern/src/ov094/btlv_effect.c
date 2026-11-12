// The PC build compiles src/ov094/btlv_effect.c through this file (tools/port_sources.py). At the command menu the
// game turns on its idle camera (BtlvEffect_SetIdleEffectMode(2), src/ov094/btlv_input.c): after idleEffectWait ticks
// (900, 15 s on a DS) with the menu up, BtlvEffect_UpdateIdleEffect starts camera pans picked at random, until the
// player chooses (BtlvEffect_StopIdleEffect). Battle speed (libntr-ios-battlespeed.patch) runs the battle's ticks up to
// 4x, so at 4x those pans began 3.75 s after the menu came up, while the player was still choosing: the camera drifted
// and cut about at the command menu, where on a DS it holds still for 15 s. So while the idle effect is on (the game
// waits for the player's command), the battle runs at the player's own speed (port/src/ovl.c): the menu and its idle
// camera keep the DS's timing, and the turn that follows runs at Battle speed again. The DS build never sees this file.
#pragma push_macro("func_ov094_021f78e4")
#define func_ov094_021f78e4 func_ov094_021f78e4_game
#include "../../../../src/ov094/btlv_effect.c"
#pragma pop_macro("func_ov094_021f78e4")

void PortOvl_SetBattleIdle(BOOL on);
void func_ov094_021f78e4(void);

// pokebw2's BtlvEffect_Main
void func_ov094_021f78e4(void) {
    func_ov094_021f78e4_game();
    PortOvl_SetBattleIdle(data_ov094_0220af20 != NULL && data_ov094_0220af20->idleEffectMode != 0);
}
