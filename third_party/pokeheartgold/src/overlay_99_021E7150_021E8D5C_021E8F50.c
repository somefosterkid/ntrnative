#include "global.h"

#include "overlay_99_pokeathlon_event_record.h"

#include "sprite.h"
#include "sprite_system.h"

void ov98_0221E970(void *, void *, u32 *, u32, u32);

void ov99_021E9464(void *, u32 *, u16, u32, s32, u32);

s32 ov99_021E94CC(void *, u16, u32, u32);

void ov99_021E8F50(u32 *param_1);

void ov99_021E8F50(u32 *param_1) {
    s32 iVar1;
    s32 iVar2;
    u32 uVar3;
    s32 iStack_28;
    s32 auStack_24[3];
    u32 *puVar5;

    uVar3 = 0;
    iStack_28 = 0;
    do {
        iVar1 = ov99_021E94CC(((PokeathlonEventRecord *)param_1)->data, (s32)((PokeathlonEventRecord *)param_1)->cursor, uVar3 & 0xff, *(u32 *)&((PokeathlonEventRecord *)param_1)->linkMode);
        if (iVar1 != 0xffff) {
            iVar1 = 0;
            do {
                puVar5 = auStack_24;
                puVar5[0] = 0;
                puVar5[1] = 0;
                puVar5[2] = 0;
                iVar2 = iVar1 + iStack_28 + 0x16;
                ov99_021E9464(((PokeathlonEventRecord *)param_1)->data, auStack_24, (s32)((PokeathlonEventRecord *)param_1)->cursor, uVar3, iVar1, *(u32 *)&((PokeathlonEventRecord *)param_1)->linkMode);
                if ((u32)(auStack_24[0] << 23) >> 23 != 0) {
                    ov98_0221E970(*(void **)&((PokeathlonEventRecord *)param_1)->spriteContext, (u8 *)((PokeathlonEventRecord *)param_1)->sprites[iVar2], auStack_24, 0, 1);
                    ManagedSprite_SetDrawFlag(((PokeathlonEventRecord *)param_1)->sprites[iVar2], 1);
                    if (uVar3 == 0) {
                        ManagedSprite_SetAnimSpeed(((PokeathlonEventRecord *)param_1)->sprites[iVar2], FX32_CONST(1.5));
                    }
                }
                iVar1 = iVar1 + 1;
            } while (iVar1 < 3);
        }
        uVar3 = uVar3 + 1;
        iStack_28 = iStack_28 + 3;
    } while ((s32)uVar3 < 5);
    return;
}
