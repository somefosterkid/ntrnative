#ifndef POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_TEMPLATES_H
#define POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_TEMPLATES_H

#include "overlay_manager.h"

// The Mystery Gift application's template (asm: overlay_74_thumb.s) and its OverlayFunction members.
extern const OverlayManagerTemplate gApp_MainMenu_SelectOption_MysteryGift;
BOOL ov74_0222AE6C(OverlayManager *manager, int *state);
BOOL ov74_0222C2EC(OverlayManager *manager, int *state);
BOOL ov74_0222CD94(OverlayManager *manager, int *state);

// Called by ov74_0222C2EC (state machine step; asm AB0C beside it).
void ov74_0222AC1C(OverlayManager *manager, int *state);

#endif // POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_TEMPLATES_H
