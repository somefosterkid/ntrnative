#include "global.h"
struct PokeathlonCourseRecord;

#include "sprite_system.h"
#include "sprite.h"
#include "overlay_99_pokeathlon_course_record.h"

void ov99_021E64E0(struct PokeathlonCourseRecord *arg0, s32 arg1);

void ov98_0221E7E8(u32, u32, u32, u32, u32);
void ov98_0221E970(u32, u32, u32, u32, u32);
u32 ov98_0221F120(u32, u32);
u32 ov99_021E5B7C(u32, u32, u32);
u16 ov99_021E5B90(u32);
s32 ov99_021E5BB4(u32, u32);
extern u32 ov99_021E9628;
void ov99_021E6530(u32 *param_1, u32 param_2);

void ov99_021E64E0(PokeathlonCourseRecord *arg0, s32 flag) {
    ManagedSprite_SetDrawFlag(arg0->sprites[14], flag);
    ManagedSprite_SetDrawFlag(arg0->sprites[15], flag);
    ManagedSprite_SetDrawFlag(arg0->sprites[16], flag);
    ManagedSprite_SetDrawFlag(arg0->sprites[11], flag);
    ManagedSprite_SetDrawFlag(arg0->sprites[12], flag);
    ManagedSprite_SetDrawFlag(arg0->sprites[13], flag);
    ManagedSprite_SetDrawFlag(arg0->sprites[17], flag);
    ManagedSprite_SetDrawFlag(arg0->sprites[18], flag);
    ManagedSprite_SetDrawFlag(arg0->sprites[19], flag);
}

void ov99_021E6530(u32 *param_1, u32 param_2) {
    u32 uVar1;
    s32 iVar2;
    u32 uVar3;
    u32 uVar4;
    unsigned long long new_var;
    uVar1 = ov99_021E5B90(*param_1);
    uVar4 = 0;
    do {
        iVar2 = ov99_021E5BB4(*param_1, param_2);
        if (iVar2 == 1) {
            uVar3 = ov98_0221F120(uVar1, (uVar4 + 1) & 0xff);
            new_var = uVar3 & 0xffff;
            ManagedSprite_SetAnim((ManagedSprite *)param_1[uVar4 + 0x1a], uVar3 & 0xffff);
            ManagedSprite_SetAnim((ManagedSprite *)param_1[uVar4 + 0x14], *((s32 *)((&ov99_021E9628) + new_var)));
            ManagedSprite_SetDrawFlag((ManagedSprite *)param_1[uVar4 + 0x14], 1);
            ManagedSprite_SetDrawFlag((ManagedSprite *)param_1[uVar4 + 0x1a], 1);
        } else {
            ManagedSprite_SetDrawFlag((ManagedSprite *)param_1[uVar4 + 0x14], 0);
            ManagedSprite_SetDrawFlag((ManagedSprite *)param_1[uVar4 + 0x1a], 0);
        }
        uVar4 = (uVar4 + 1) & 0xff;
    } while (uVar4 < 3);
    uVar4 = 0;
    do {
        uVar1 = ov99_021E5B7C(*param_1, param_2, uVar4);
        iVar2 = ov99_021E5BB4(*param_1, param_2);
        if (iVar2 == 1) {
            ov98_0221E7E8(param_1[uVar4 + 0x11], uVar1, 1, 0, param_1[3]);
            ManagedSprite_SetDrawFlag((ManagedSprite *)param_1[uVar4 + 0x11], 1);
            ov98_0221E970(param_1[5], param_1[uVar4 + 0x1d], uVar1, 0, 1);
            ManagedSprite_SetDrawFlag((ManagedSprite *)param_1[uVar4 + 0x1d], 1);
        } else {
            ManagedSprite_SetDrawFlag((ManagedSprite *)param_1[uVar4 + 0x1d], 0);
        }
        uVar4 = (uVar4 + 1) & 0xff;
    } while (uVar4 < 3);
    uVar4 = 0;
    do {
        iVar2 = ov99_021E5BB4(*param_1, param_2);
        ManagedSprite_SetDrawFlag((ManagedSprite *)param_1[uVar4 + 0x17], iVar2);
        uVar4 = (uVar4 + 1) & 0xff;
    } while (uVar4 < 3);
    return;
}
