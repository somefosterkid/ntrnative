#ifndef POKEHEARTGOLD_UNK_02061284_H
#define POKEHEARTGOLD_UNK_02061284_H

#include "global.h"
#include "field_types_def.h"

void sub_020612B4(LocalMapObject *param_1);
void sub_020612C0(LocalMapObject *param_1);
void sub_020612CC(LocalMapObject *param_1);
void sub_020612D8(LocalMapObject *param_1);
void sub_020612E4(LocalMapObject *param_1);
void sub_020612F0(LocalMapObject *param_1);
void sub_020612FC(LocalMapObject *param_1);
void sub_02061308(LocalMapObject *param_1);
void sub_02061314(LocalMapObject *param_1);

void sub_02061320(LocalMapObject *param_1);
void sub_0206132C(LocalMapObject *param_1);
void sub_0206139C(void);
void sub_020613A0(LocalMapObject *param_1);

void sub_020613B0(LocalMapObject *param_1);
void sub_020613C0(LocalMapObject *param_1);

void sub_020613F8(LocalMapObject *param_1);

void sub_02061690(LocalMapObject *param_1);
void sub_0206169C(LocalMapObject *param_1);
void sub_020616A8(LocalMapObject *param_1);

void sub_020616B4(LocalMapObject *param_1);

void sub_020616E4(LocalMapObject *param_1);
void sub_020616F0(LocalMapObject *param_1);

u32 sub_02061754(LocalMapObject *param_1, void *param_2);

void sub_02061844(LocalMapObject *param_1);

void sub_02061874(LocalMapObject *param_1, void *param_2);
u32 sub_02061894(LocalMapObject *param_1, void *param_2);

u32 sub_020618B0(LocalMapObject *param_1, void *param_2);

void sub_0206197C(LocalMapObject *param_1);

void sub_02061B1C(LocalMapObject *param_1);
void sub_02061B2C(LocalMapObject *param_1);
void sub_02061B3C(LocalMapObject *param_1);
void sub_02061B4C(LocalMapObject *param_1);
void sub_02061B5C(LocalMapObject *param_1);

void sub_02061B6C(LocalMapObject *param_1);
void sub_02061B7C(LocalMapObject *param_1);
void sub_02061B8C(LocalMapObject *param_1);
void sub_02061B9C(LocalMapObject *param_1);
void sub_02061BAC(LocalMapObject *param_1);
void sub_02061BBC(LocalMapObject *param_1);
void sub_02061BCC(LocalMapObject *param_1);
void sub_02061BDC(LocalMapObject *param_1);
void sub_02061BEC(LocalMapObject *param_1);
void sub_02061BFC(LocalMapObject *param_1);
void sub_02061C0C(LocalMapObject *param_1);

void sub_02061C40(LocalMapObject *param_1, u8 *param_2);

u32 sub_02061D50(LocalMapObject *param_1, u8 *param_2);
void sub_02061D80(LocalMapObject *param_1);
void sub_02061D90(LocalMapObject *param_1);

void sub_02061DA0(LocalMapObject *param_1);
void sub_02061DB0(LocalMapObject *param_1);
void sub_02061DC0(LocalMapObject *param_1);
void sub_02061DD0(LocalMapObject *param_1);
void sub_02061DE0(LocalMapObject *param_1);

void sub_02061DF0(LocalMapObject *param_1);

// One record of the NPC facing-pattern table (_020FD838): a pattern id and the
// -1 terminated list of facing directions it walks through. 8 bytes on the DS,
// 16 on the host. The table ends at the record whose id is 0x27.
typedef struct MapObjectDirectionPattern {
    s32 id;
    const s32 *directions;
} MapObjectDirectionPattern;

extern const MapObjectDirectionPattern _020FD838[];
extern const s32 _020FD7B8[];

s32 sub_02061E00(const s32 *list, s32 terminator);
s32 sub_02061E20(const s32 *list, s32 terminator);
s32 sub_02061E44(s32 patternId, s32 terminator);
const s32 *sub_02061E6C(s32 patternId);

#endif // POKEHEARTGOLD_UNK_02061284_H
