#ifndef POKEHEARTGOLD_OVERLAY_74_APP_TEMPLATES_H
#define POKEHEARTGOLD_OVERLAY_74_APP_TEMPLATES_H

#include "overlay_manager.h"

// The ov74 main-menu applications' templates (asm: overlay_74_thumb.s / overlay_74_thumb_0222B3A4_0223B410.s) and their
// OverlayFunction members.

// Connect to Ranger
extern const OverlayManagerTemplate gApp_MainMenu_SelectOption_ConnectToRanger;
BOOL ov74_02229200(OverlayManager *manager, int *state);
BOOL ov74_02229294(OverlayManager *manager, int *state);
BOOL ov74_02229450(OverlayManager *manager, int *state);

// Wonder Cards (second app)
extern const OverlayManagerTemplate _0223B420;
BOOL ov74_0222F598(OverlayManager *manager, int *state);
BOOL ov74_0222F7D4(OverlayManager *manager, int *state);
BOOL ov74_0222FB8C(OverlayManager *manager, int *state);

#endif // POKEHEARTGOLD_OVERLAY_74_APP_TEMPLATES_H
