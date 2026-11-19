#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

ManagedSprite *ov98_0221E6E0(u32, u8 *);

s32 ov98_0221EF64(u32);

u16 ov99_021E5B90(u32, s32);

s32 ov99_021E5BB4(u32, s32);

extern u32 ov99_021E9840;

void ov99_021E6438(u32 *param_1);

void ov99_021E6438(u32 *param_1) {
    u32 uVar1;
    ManagedSprite *managedSprite;
    s32 iVar2;
    s32 iVar5;
    u8 *puVar3;
    u32 *puVar4;
    u32 *q;
    s32 j;
    puVar3 = (unsigned char *)(&ov99_021E9840);
    iVar5 = 0;
    puVar4 = param_1;
    do {
        managedSprite = ov98_0221E6E0(param_1[5], puVar3);
        puVar4[6] = (u32)managedSprite;
        ManagedSprite_SetAnimateFlag(managedSprite, 1);
        puVar3 = puVar3 + 0x34;
        puVar4 = puVar4 + 1;
        iVar5 = iVar5 + 1;
    } while (iVar5 < 0x1a);
    j = 0;
    q = param_1;
    do {
        iVar2 = ov99_021E5BB4(*param_1, j);
        if (iVar2 == 0) {
            ManagedSprite_SetDrawFlag((ManagedSprite *)q[7], 0);
        }

        iVar2 = ov98_0221EF64(ov99_021E5B90(*param_1, j));
        uVar1 = j >> 0x1f;
        if (iVar2 == 2) {
            if ((j % 2) == 0) {
                ManagedSprite_SetAnim((ManagedSprite *)q[0xc], 5);
            } else {
                ManagedSprite_SetAnim((ManagedSprite *)q[0xc], 6);
            }
        } else if (iVar2 == 1) {
            if ((j % 2) == 0) {
                ManagedSprite_SetAnim((ManagedSprite *)q[0xc], 2);
            } else {
                ManagedSprite_SetAnim((ManagedSprite *)q[0xc], 3);
            }
        } else {
            ManagedSprite_SetDrawFlag((ManagedSprite *)q[0xc], 0);
        }
        j = j + 1;
        q = q + 1;
    } while (j < 5);
    return;
}
