#ifndef POKEHEARTGOLD_FRONTIER_TOP_H
#define POKEHEARTGOLD_FRONTIER_TOP_H

#include "global.h"
#include "overlay_manager.h"

s32 Frontier_Init(OverlayManager *arg0);

s32 Frontier_Exit(OverlayManager *arg0);

void Frontier_LaunchApplication(void *a0, OverlayManagerTemplate *ovyTemp, void *args, u32 a3, s32 func);

void sub_02096854(void *arg0, s8 arg1, s16 arg2);

s32 sub_0209686C(s32 arg0, s32 arg1);

#endif // POKEHEARTGOLD_FRONTIER_H
