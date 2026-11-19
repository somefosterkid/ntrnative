#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_021EAB38(struct Ov96SpriteSetElem *elem, s32 flag);

void ov96_022193CC(u32 param_1, s32 param_2);

u32 ov96_02219794(u32 *param_1, u32 param_2, u8 param_3);

u32 ov96_02219940(u32 *param_1, u32 param_2, u8 param_3);

typedef struct {
    u32 unk0_0 : 17;
    u32 unk0_17 : 2;
    u32 unk0_19 : 13;
} UnkStruct_ov96_02216770;


void ov96_02216770(Ov96RdSlot *slot, s16 *pos, u32 *param_3, u32 param_4, UnkStruct_ov96_02216770 *param_5, s32 param_6, s32 param_7);

void ov96_02216770(Ov96RdSlot *slot, s16 *pos, u32 *param_3, u32 param_4, UnkStruct_ov96_02216770 *param_5, s32 param_6, s32 param_7) {
    s16 sStack[2];

    switch (slot->state) {
    case 0:
        ManagedSprite_SetPositionXYWithSubscreenOffset(slot->sprites[3], pos[0], pos[1], FX32_CONST(480));
        ManagedSprite_SetDrawFlag(slot->sprites[3], 1);
        ManagedSprite_ResetSpriteAnimCtrlState(slot->sprites[3]);
        ManagedSprite_SetPositionXYWithSubscreenOffset(slot->sprites[2], pos[0], pos[1], FX32_CONST(480));
        ManagedSprite_SetDrawFlag(slot->sprites[2], 1);
        ov96_021EAB38(slot->elem, 0);
        slot->timer = 0;
        ov96_022193CC(0x8b4, param_7);
        slot->state++;
        break;
    case 1:
        ManagedSprite_GetPositionXY(slot->sprites[2], &sStack[1], &sStack[0]);
        sStack[0] -= 0x28;
        ManagedSprite_SetPositionXYWithSubscreenOffset(slot->sprites[2], sStack[1], sStack[0], FX32_CONST(480));
        slot->timer++;
        if (slot->timer >= 10) {
            ManagedSprite_SetDrawFlag(slot->sprites[2], 0);
            slot->state++;
        }
        break;
    case 2:
        if (param_7 != 0) {
            if (ov96_02219794(param_3, param_4, slot->unk_E4_14) == 0) {
                break;
            }
            param_5->unk0_17 = 1;
            slot->state++;
            break;
        }
        if (param_6 != 0) {
            if (ov96_02219940(param_3, param_4, slot->unk_E4_14) == 0) {
                break;
            }
            param_5->unk0_17 = 1;
            slot->state++;
            break;
        }
        slot->state++;
        break;
    case 3:
        break;
    default:
        GF_AssertFail();
    }
}
