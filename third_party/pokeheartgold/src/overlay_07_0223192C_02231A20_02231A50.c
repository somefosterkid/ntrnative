#include "global.h"
#include "overlay_07_battle_anim_system.h"

u32 ov07_0221BFC0(void *);

u8 ov07_02231924(BattleAnimSystem *arg0, s32 arg1);

void ov07_02231A20(s32 param_1, u32 param_2, u16 *param_3);

void ov07_02231A50(void *param_1, u32 param_2, u16 *param_3);

void ov07_02231A50(void *param_1, u32 param_2, u16 *param_3) {
    s32 iVar1;
    u32 uVar2;

    iVar1 = ov07_02231924(param_1, param_2);
    uVar2 = ov07_0221BFC0(param_1);
    ov07_02231A20(iVar1, uVar2, param_3);
    return;
}
