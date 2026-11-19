#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_021EAB38(struct Ov96SpriteSetElem *elem, s32 flag);

void ov96_022193CC(u32 param_1, s32 param_2);

void ov96_02216A54(Ov96RdSlot *slot, s16 *param_2, void *param_3, s32 param_4, s32 param_5);

void ov96_02216A54(Ov96RdSlot *slot, s16 *param_2, void *param_3, s32 param_4, s32 param_5) {
    ov96_021EAB38(slot->elem, 1);
    ManagedSprite_SetPositionXYWithSubscreenOffset(slot->sprites[3], *param_2, param_2[1], 0x1e0000);
    ManagedSprite_ResetSpriteAnimCtrlState(slot->sprites[3]);
    ManagedSprite_SetDrawFlag(slot->sprites[2], 0);
    ov96_022193CC(0x8b6, param_5);
    if (param_4 != 0) {
        *(u32 *)param_3 = *(u32 *)param_3 & 0xfff9ffff;
    }
    return;
}
