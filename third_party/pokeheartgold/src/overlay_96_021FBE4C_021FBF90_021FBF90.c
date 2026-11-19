#include "global.h"

#include "error_handling.h"

s32 ov96_021FBE44(s32 param_1, s32 param_2);

extern const VecFx32 ov96_0221C5B8;

void ov96_021FBE54(u32 *param_1, s32 param_2);

u32 ov96_021FBE9C(u32 *param_1);

void ov96_021FBEDC(u8 *param_1, VecFx32 *param_2);

u32 *ov96_021FBF90(u8 *param_1, s32 param_2, s32 param_3);

u32 *ov96_021FBF90(u8 *param_1, s32 param_2, s32 param_3) {
    u32 *puVar1;
    s32 iVar2;
    s32 iVar4;
    VecFx32 VStack_24;

    iVar4 = param_2 * 8;
    puVar1 = (u32 *)
        ov96_021FBE44(*(s32 *)(param_1 + iVar4), *(u32 *)(param_1 + iVar4 + 4) & 0xff);
    iVar2 = ov96_021FBE9C(puVar1);
    if (iVar2 != 0) {
        GF_AssertFail();
        return NULL;
    }
    *(s32 *)(param_1 + 4 + iVar4) = (*(s32 *)(param_1 + 4 + iVar4) + 1) % 4;
    VStack_24 = ov96_0221C5B8;
    VStack_24.y += param_3 * 0x1000;
    switch (param_2) {
    case 0:
        VStack_24.x = 0x30000;
        break;
    case 1:
        VStack_24.x = 0x80000;
        break;
    case 2:
        VStack_24.x = 0xd0000;
        break;
    }
    ov96_021FBEDC((u8 *)puVar1, &VStack_24);
    ov96_021FBE54(puVar1, 1);
    return puVar1;
}
