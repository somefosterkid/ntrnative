#ifndef POKEHEARTGOLD_OVERLAY_88_H
#define POKEHEARTGOLD_OVERLAY_88_H

#include "global.h"
#include "overlay_manager.h"
#include "overlay_88_wifi_plaza_news.h"
#include "pm_string.h"

s32 ov88_02258800(OverlayManager *arg0, int *state);

s32 ov88_022589FC(OverlayManager *arg0);

void ov88_02259560(WiFiPlazaNewsNameEntry *entry, String *name, s32 holdDuration, const WiFiPlazaNewsNamePalette *palette, const NNSG2dPaletteData *plttData);
void ov88_022595DC(WiFiPlazaNewsNameEntry *entry);
BOOL ov88_022595E4(WiFiPlazaNewsNameEntry *entry);

#endif // POKEHEARTGOLD_OVERLAY_88_H
