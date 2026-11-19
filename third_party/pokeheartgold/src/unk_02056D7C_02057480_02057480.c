#include "global.h"

#include "comm_player_manager.h"
#include "field_types_def.h"
#include "map_events.h"
#include "unk_02054648.h"

static u32 sub_02057480(u32 param_1, u32 param_2);

static u32 sub_02057480(u32 param_1, u32 param_2) {
    u32 uVar1;
    ObjectEvent *pOVar2;
    s32 iVar3;

    uVar1 = Field_GetNumObjectEvents(_021D41C4->fieldSystem);
    pOVar2 = (struct ObjectEvent *)(Field_GetObjectEvents(_021D41C4->fieldSystem));
    iVar3 = 0;
    if (0 < (s32)uVar1) {
        do {
            if ((param_1 == pOVar2->x) && (param_2 == pOVar2->z)) {
                return 1;
            }
            iVar3 = iVar3 + 1;
            pOVar2 = pOVar2 + 1;
        } while (iVar3 < (s32)uVar1);
    }
    return 0;
}

u16 sub_02057B4C(s32);

u16 sub_02057B9C(s32);

BOOL sub_020574C4(u32 param_1, u32 param_2, s32 param_3);

BOOL sub_020574C4(u32 param_1, u32 param_2, s32 param_3) {
    u32 uVar1;
    BOOL BVar2;
    s32 iVar3;

    if ((param_1 != 0xffff) && (param_2 != 0xffff)) {
        iVar3 = 0;
        do {
            if (((iVar3 != param_3) && (uVar1 = sub_02057B4C(iVar3), param_1 == uVar1)) && (uVar1 = sub_02057B9C(iVar3), param_2 == uVar1)) {
                return 1;
            }
            iVar3 = iVar3 + 1;
        } while (iVar3 < 8);
        iVar3 = sub_02057480(param_1, param_2);
        if (iVar3 != 0) {
            return 1;
        }
    }
    BVar2 = sub_020548C0(_021D41C4->fieldSystem, param_1, param_2);
    return BVar2;
}
