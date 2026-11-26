#include "global.h"
struct UnkStruct_0208C06C;

#include "sprite.h"
#include "sprite_system.h"
#include "unk_02013534.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

void ov40_0222D6D0(void *);

void ov40_0222D7DC(void *);

void ov40_02242084(u8 *param_1);

void ov40_022420B4(struct UnkStruct_0208C06C *arg0, s32 arg1);
s32 ov40_0222D5AC(void *, void *, s32);
s32 ov40_0222D66C(void *, void *, s32);
s32 ov40_0222D6EC(void *, s32);
ManagedSprite *ov40_0222D800(void *, s32);
void ov40_02242110(UnkStruct_0208C06C *arg0);

void ov40_02242084(u8 *param_1) {
    u8 *iVar1;

    iVar1 = *(u8 **)(param_1 + 0x860);
    ov40_0222D6D0(iVar1 + 0x10);
    ov40_0222D6D0(iVar1 + 0x2c);
    Sprite_DeleteAndFreeResources(*(ManagedSprite **)(iVar1 + 0x14));
    Sprite_DeleteAndFreeResources(*(ManagedSprite **)(iVar1 + 0x30));
    ov40_0222D7DC(param_1);
    return;
}

void ov40_022420B4(UnkStruct_0208C06C *arg0, s32 arg1) {
    ManagedSprite *managedSprite;
    void *v0;

    v0 = arg0->unk_860;
    managedSprite = *(ManagedSprite **)((u8 *)v0 + 0x30);
    if (arg1 == 0) {
        ManagedSprite_SetDrawFlag(managedSprite, 1);
        TextOBJ_SetSpritesDrawFlag(*(TextOBJ **)((u8 *)v0 + 0x34), 1);
        ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)v0 + 0x14), 0x20, 0xE8);
    } else {
        ManagedSprite_SetDrawFlag(managedSprite, 0);
        TextOBJ_SetSpritesDrawFlag(*(TextOBJ **)((u8 *)v0 + 0x34), 0);
        ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)v0 + 0x14), 0x50, 0xE8);
    }
    sub_020136B4(*(TextOBJ **)((u8 *)v0 + 0x18), 0x24, -8);
    sub_020136B4(*(TextOBJ **)((u8 *)v0 + 0x34), 0x24, -8);
}

void ov40_02242110(UnkStruct_0208C06C *arg0) {
    void *v0;

    v0 = arg0->unk_860;
    ov40_0222D6EC(arg0, 2);
    *(ManagedSprite **)((u8 *)v0 + 0x14) = ov40_0222D800(arg0, 2);
    *(ManagedSprite **)((u8 *)v0 + 0x30) = ov40_0222D800(arg0, 2);
    ov40_0222D5AC(v0 + 0x10, &arg0->unk_14, 2);
    ov40_0222D5AC(v0 + 0x2C, &arg0->unk_14, 2);
    if (arg0->unk_86C == 0xD2) {
        ov40_0222D66C(v0 + 0x10, &arg0->unk_14, 3);
        ov40_0222D66C(v0 + 0x2C, &arg0->unk_14, 0x6F);
        ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)v0 + 0x14), 0);
        ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)v0 + 0x30), 1);
    } else {
        ov40_0222D66C(v0 + 0x10, &arg0->unk_14, 3);
        ov40_0222D66C(v0 + 0x2C, &arg0->unk_14, 0x5E);
        ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)v0 + 0x14), 0);
        ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)v0 + 0x30), 3);
    }
    ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)v0 + 0x14), 0x20, 0xE8);
    ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)v0 + 0x30), 0x80, 0xE8);
    sub_020136B4(*(TextOBJ **)((u8 *)v0 + 0x18), 0x24, -8);
    sub_020136B4(*(TextOBJ **)((u8 *)v0 + 0x34), 0x24, -8);
    TextOBJ_SetSpritesDrawFlag(*(TextOBJ **)((u8 *)v0 + 0x18), 1);
    TextOBJ_SetSpritesDrawFlag(*(TextOBJ **)((u8 *)v0 + 0x34), 1);
    ov40_022420B4(arg0, 0);
}
