#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

void *ov112_021E7668(void);

void ov112_021EA60C(u8 *param_1);

void ov112_021EA64C(void *arg0);

void ov112_021EA670(u8 *arg0, s32 arg1);
void ov112_021EA688(u8 *arg0, s32 arg1);
void ov112_021EA6A0(u8 *arg0, s32 arg1, s32 arg2);

void ov112_021EA60C(u8 *param_1) {
    u16 uVar1;
    void *iVar2;
    ManagedSprite *managedSprite;

    managedSprite = *(ManagedSprite **)(param_1 + 0x1e538);
    iVar2 = ov112_021E7668();
    if (iVar2 != 0) {
        uVar1 = ManagedSprite_GetActiveAnim(managedSprite);
        if (uVar1 == 2) {
            ManagedSprite_SetAnim(*(ManagedSprite **)(param_1 + 0x1e538), 1);
            return;
        }
    } else {
        uVar1 = ManagedSprite_GetActiveAnim(managedSprite);
        if (uVar1 == 1) {
            ManagedSprite_SetAnim(*(ManagedSprite **)(param_1 + 0x1e538), 2);
        }
    }
    return;
}

void ov112_021EA64C(void *arg0) {
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)arg0 + 0x1E534), 0);
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)arg0 + 0x1E538), 0);
}

void ov112_021EA670(u8 *arg0, s32 arg1) {
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)(arg0 + (arg1 * 4)) + 0x1E530), 1);
}

void ov112_021EA688(u8 *arg0, s32 arg1) {
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)(arg0 + (arg1 * 4)) + 0x1E530), 0);
}

void ov112_021EA6A0(u8 *arg0, s32 arg1, s32 seqno) {
    ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)(arg0 + (arg1 * 4)) + 0x1E530), seqno);
}
