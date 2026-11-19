#ifndef POKEHEARTGOLD_OVERLAY_80_02229EE0_H
#define POKEHEARTGOLD_OVERLAY_80_02229EE0_H

#include "global.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "player_data.h"
#include "field_types_def.h"

void ov80_0222A3BC(s32 arg0, Party *arg1, Pokemon *arg2);

void ov80_0222A3D4(Sprite *sprite, u8 animID);
void ov80_0222A400(Sprite *sprite, s16 x, s16 y, u8 isSelected);
u8 ov80_0222A43C(u16 hp, u16 maxHp);

s32 ov80_0222A7EC(PlayerProfile *arg0);

BOOL ov80_0222A840(PlayerAvatar *playerAvatar);

#endif // POKEHEARTGOLD_OVERLAY_80_02229EE0_H
