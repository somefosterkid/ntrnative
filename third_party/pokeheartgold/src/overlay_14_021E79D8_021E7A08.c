#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F2F88(u32 param_1, s16 *param_2, s16 *param_3, s32 param_4);
void ov14_021E7A08(UnkStruct_ov14_boxapp *param_1, u16 param_2, u32 param_3, s32 param_4, s32 param_5);

void ov14_021E7A08(UnkStruct_ov14_boxapp *param_1, u16 param_2, u32 param_3, s32 param_4, s32 param_5) {
    s32 iVar1;
    u32 *puVar2;
    s32 iVar3;
    s32 iVar4;
    UnkStruct_ov14_dragstate *puVar5;
    s16 sStack_1a;
    s16 sStack_1c;
    s16 sStack_1e;
    s16 sStack_20;
    puVar5 = param_1->sys->unk_C;
    ManagedSprite_GetPositionXY(param_1->sys->sprites[11], &sStack_1a, &sStack_1c);
    ov14_021F2F88(param_3, &sStack_1e, &sStack_20, param_4);
    if (param_5 == 1) {
        sStack_1e = sStack_1e + 8;
        sStack_20 = sStack_20 + 8;
    } else {
        sStack_20 = sStack_20 + (iVar1 = 4);
    }
    puVar5->slot = (s16)param_3;
    puVar5->unk_2 = param_2;
    puVar5->flags = puVar5->flags & 3;
    puVar5->mode = param_4;
    iVar1 = (s32)sStack_1e;
    iVar4 = (s32)sStack_1a;
    puVar2 = &puVar5->flags;
    if (iVar4 > iVar1) {
        *puVar2 = ((*puVar2) & 0xfffffffe) | 1;
        iVar1 = iVar4 - iVar1;
    } else {
        *puVar2 = (*puVar2) & 0xfffffffe;
        iVar1 = iVar1 - iVar4;
    }
    puVar5->dx = ((s32)((iVar1 * 0x100) + (((u32)((iVar1 * 0x100) >> 2)) >> 0x1d))) >> 3;
    iVar1 = (s32)sStack_20;
    iVar3 = (s32)sStack_1c;
    if (iVar3 > iVar1) {
        puVar5->flags = puVar5->flags | 2;
        iVar1 = iVar3 - iVar1;
    } else {
        puVar5->flags = puVar5->flags & 0xfffffffd;
        iVar1 = iVar1 - iVar3;
    }
    puVar5->dy = ((s32)((iVar1 * 0x100) + (((u32)((iVar1 * 0x100) >> 2)) >> 0x1d))) >> 3;
    puVar5->x = iVar4 << 8;
    puVar5->y = iVar3 << 8;
    return;
}
