#ifndef POKEHEARTGOLD_OVY_108_H
#define POKEHEARTGOLD_OVY_108_H

#include "overlay_108_safari_area_customizer_args.h"
#include "overlay_manager.h"

BOOL SafariDecoration_Init(OverlayManager *man, int *state);
BOOL SafariDecoration_Main(OverlayManager *man, int *state);
BOOL SafariDecoration_Exit(OverlayManager *man, int *state);

BOOL SafariAreaCustomizer_Init(OverlayManager *man, int *state);
BOOL SafariAreaCustomizer_Main(OverlayManager *man, int *state);
BOOL SafariAreaCustomizer_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_OVY_108_H
