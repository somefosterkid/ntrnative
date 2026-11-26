#include "global.h"

#include "party.h"
#include "pokemon_types_def.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6070(UnkStruct_ov14_boxapp *, s32, u32, void *);

void ov14_021E7034(void *param_1, u32 param_2, s32 param_3) {
    u32 uVar1;
    s32 iVar2;
    MoveSet *iVar3;
    MoveEntry *e;

    iVar3 = ((UnkStruct_ov14_boxapp *)param_1)->sys->unk_C;
    ((UnkStruct_ov14_boxapp *)param_1)->sys->unk_40C4 = 1;
    uVar1 = 0;
    e = iVar3->e;
    do {
        e->idx = 0;
        e->arg = 0;
        e->active = 0;
        uVar1 = uVar1 + 1;
        e++;
    } while (uVar1 < 7);
    if (!((param_3 != 0xff) && (iVar2 = ov14_021E6070(param_1, param_3, 0xac, 0), iVar2 == 0))) {
        iVar3->from = 0xff;
        iVar3->to = 0xff;
        iVar3->e[0].idx = param_2;
        iVar3->e[0].arg = param_2;
        iVar3->e[0].active = 1;
        ov14_021E6B48(param_1, &iVar3->e[0]);
        return;
    }
    iVar3->from = param_2;
    iVar3->to = param_3;
    iVar2 = Party_GetCount(((UnkStruct_ov14_boxapp *)param_1)->party);
    ov14_021E6CC8(param_1, param_2, iVar2 + 0x1e);
    return;
}
