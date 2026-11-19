#ifndef POKEHEARTGOLD_OVERLAY_01_021FF854_H
#define POKEHEARTGOLD_OVERLAY_01_021FF854_H

#include "global.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"
#include "field_types_def.h"

void *ov01_021FF854(FieldEffectManager *fieldEffMan);

void ov01_021FF870(void *arg0);
void ov01_021FF8C8(FieldEffectRendererContext *ctx);

void ov01_021FF8F0(LocalMapObject *arg0, s32 arg1);
void ov01_021FF964(LocalMapObject *object, s32 a1, s32 x, s32 y, s32 a4);

BOOL ov01_021FF9F4(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FFAB0(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FFABC(OverworldAnimManager *manager, FieldEffectAnimWork *work);
void ov01_021FFBD4(OverworldAnimManager *manager, FieldEffectAnimWork *work);

#endif // POKEHEARTGOLD_OVERLAY_01_021FF854_H
