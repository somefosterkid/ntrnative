#include "global.h"

#include "error_handling.h"
#include "sprite_system.h"

#include "overlay_96_ov96_rd_work.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_022193CC(s32, s32);
#else
s32 ov96_022193CC(s32, s32);
#endif


void ov96_02216934(Ov96RdSlot *slot, s16 *arg1, void *arg2, u32 arg3, void *arg4, s32 arg5, s32 arg6);

void ov96_02216934(Ov96RdSlot *slot, s16 *arg1, void *arg2, u32 arg3, void *arg4, s32 arg5, s32 arg6) {
    s16 sp0[2];

    switch (slot->state) {
    case 0:
        ManagedSprite_SetPositionXYWithSubscreenOffset(slot->sprites[2], arg1[0], arg1[1] - 0x190, FX32_CONST(480));
        ManagedSprite_SetDrawFlag(slot->sprites[2], 1);
        slot->timer = 0;
        ov96_022193CC(0x8B5, arg6);
        slot->state++;
        return;
    case 1:
        ManagedSprite_GetPositionXY(slot->sprites[2], &sp0[1], &sp0[0]);
        sp0[0] += 0x28;
        ManagedSprite_SetPositionXYWithSubscreenOffset(slot->sprites[2], sp0[1], sp0[0], FX32_CONST(480));
        slot->timer++;
        if (slot->timer >= 0xA) {
            slot->state++;
            return;
        }
        break;
    case 2:
        if (arg5 != 0) {
            *(u32 *)arg4 = (*(u32 *)arg4 & 0xFFF9FFFF) | 0x40000;
        }
        slot->state++;
        return;
    case 3:
        break;
    default:
        GF_AssertFail();
        break;
    }
}
