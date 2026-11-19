#include "global.h"

#include "overlay_96_ov96_arena_work.h"

u32 ov96_021F22FC(void *param_1, Ov96ArenaMon *param_2, s32 param_3, VecFx32 *param_4);

u32 ov96_021F22FC(void *param_1, Ov96ArenaMon *param_2, s32 param_3, VecFx32 *param_4) {
    fx32 fVar1;
    u32 uVar2;
    Ov96ArenaLink *row;
    VecFx32 VStack_20;

    uVar2 = 0;
    row = _0221DCA0[param_3];
    do {
        if (row[uVar2].linked != 0) {
            VEC_Subtract(&row[uVar2].other->pos, &param_2->pos, &VStack_20);
            fVar1 = VEC_DotProduct(param_4, &VStack_20);
            if (0 < fVar1) {
                return 0;
            }
        }
        uVar2 = (uVar2 + 1) & 0xff;
    } while (uVar2 < 0xc);
    return 1;
}
