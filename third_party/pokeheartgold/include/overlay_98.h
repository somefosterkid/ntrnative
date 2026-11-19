#ifndef POKEHEARTGOLD_OVERLAY_98_H
#define POKEHEARTGOLD_OVERLAY_98_H

#include "global.h"
#include "sprite_system.h"
#include "sprite.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "bg_window.h"
#include "player_data.h"
#include "pokeathlon/pokeathlon_save.h"
#include "pm_string.h"
#include "overlay_98_ov98text_sys.h"
#include "overlay_98_sprite_ctx.h"

void ov98_0221E784(SpriteSystem *arg0, SpriteManager *arg1, void *arg2, s32 arg3);

void ov98_0221E8A8(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4);

void ov98_0221E9FC(Sprite **arg0, void *arg1, u32 arg2);

void ov98_0221EA4C(Sprite **arg0, enum NarcId arg1, s32 arg2, s32 arg3, s32 arg4, enum HeapID arg5);

void ov98_0221EAA8(void *arg0, s32 arg1, NNSG2dCharacterData **arg2);

s32 ov98_0221EF64(u32 arg0);

u32 ov98_0221EF80(s32 param_1);

s32 ov98_0221EFB4(s32 arg0, s32 arg1, u32 arg2);

u16 ov98_0221F01C(const Pokeathlon_RecordsSolo *record, s32 idx);

u16 ov98_0221F024(const u8 *unkDC);

u8 ov98_0221F058(const Pokeathlon_RecordsSolo *records);
void ov98_0221F090(void);

u8 ov98_0221F150(s32 arg0);

#endif // POKEHEARTGOLD_OVERLAY_98_H
