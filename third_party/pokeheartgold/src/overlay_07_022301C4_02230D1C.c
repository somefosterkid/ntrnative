#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "sys_task.h"
#include "overlay_07_megahorn_defender_context.h"

void ov07_02230BF8(SysTask *task, void *param);

void ov07_02230D1C(BattleAnimSystem *arg0);

void ov07_02230D1C(BattleAnimSystem *arg0) {
    Pokepic *pokepic;
    s32 v1;
    MegahornDefenderContext *v0;

    v0 = ov07_022324D8(arg0, sizeof(MegahornDefenderContext));
    v0->battleAnimSys = arg0;
    v1 = ov07_0221C470(arg0);
    pokepic = ov07_0221FA48(arg0, v1);
    v0->sprite = pokepic;
    v0->x = (s16)Pokepic_GetAttr(pokepic, 0);
    v0->y = (s16)Pokepic_GetAttr(v0->sprite, 1);
    v0->dirX = ov07_02222004(arg0, v1);
    v0->dirY = ov07_0222202C(arg0, v1);
    ov07_0221C410(v0->battleAnimSys, ov07_02230BF8, v0);
}
