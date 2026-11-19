#ifndef POKEHEARTGOLD_OVERLAY_01_021FEEEC_H
#define POKEHEARTGOLD_OVERLAY_01_021FEEEC_H

#include "global.h"
#include "billboard.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"
#include "field_types_def.h"

void *ov01_021FEEEC(FieldEffectManager *fieldEffMan);

void ov01_021FEF08(void *arg0);
void ov01_021FEFF8(FieldEffectRendererContext *ctx);

void ov01_021FF070(LocalMapObject *arg0, s32 arg1);
void ov01_021FF0E4(LocalMapObject *object, s32 a1, s32 x, s32 y, s32 a4);

BOOL ov01_021FF174(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FF228(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FF234(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FF35C(OverworldAnimManager *manager, FieldEffectAnimWork *work);

#endif // POKEHEARTGOLD_OVERLAY_01_021FEEEC_H
