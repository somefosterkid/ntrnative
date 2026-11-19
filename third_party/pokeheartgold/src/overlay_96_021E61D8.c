#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_sprite_res_man.h"
#include "overlay_96_unk_struct_ov96_021e9a78.h"
#include "sprite.h"
#include "unk_02009D48.h"

Sprite *ov96_021EA214(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, NNS_G2D_VRAM_TYPE param_4, enum HeapID param_5);

Sprite *ov96_021EA4D4(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4);

Sprite *ov96_021EA584(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4);

Sprite **ov96_021E61D8(PokeathlonCourseData *param_1, s32 param_2, UnkStruct_ov96_021E9A78 *param_3, SpriteList *param_4);

Sprite **ov96_021E61D8(PokeathlonCourseData *param_1, s32 param_2, UnkStruct_ov96_021E9A78 *param_3, SpriteList *param_4) {
    Sprite *pSVar1;
    VecFx32 VStack_2c;
    VecFx32 VStack_20;

    pSVar1 = ov96_021EA214(param_3, param_4, 0, NNS_G2D_VRAM_TYPE_2DMAIN, param_1->heapId);
    param_1->hud.unk_0 = pSVar1;
    pSVar1 = ov96_021EA4D4(param_3, param_4, 0, param_1->heapId);
    param_1->hud.unk_8 = pSVar1;
    pSVar1 = ov96_021EA584(param_3, param_4, 0, param_1->heapId);
    param_1->hud.unk_C = pSVar1;
    VStack_2c.z = 0;
    VStack_2c.x = 0x80000;
    VStack_2c.y = (param_2 + 0x60) * 0x1000;
    Sprite_SetMatrix(param_1->hud.unk_0, &VStack_2c);
    param_1->hud.unk_6 = (s16)param_2;
    VStack_20.z = 0;
    VStack_20.x = 0x80000;
    VStack_20.y = ((u16)param_1->hud.unk_6 + 0x48) * 0x1000;
    Sprite_SetMatrix(param_1->hud.unk_C, &VStack_20);
    param_1->hud.unk_5 = 0;
    VStack_2c.y = (param_2 + 0x82) * 0x1000;
    Sprite_SetMatrix(param_1->hud.unk_8, &VStack_2c);
    return &param_1->hud.unk_0;
}
