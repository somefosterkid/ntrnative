#include "global.h"

#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

s32 ov112_021F1624(void *, s32, s32);

void ov112_021F1F3C(Ov112SpriteRec *, s32);

void ov112_021F1F54(Ov112SpriteRec *, s16, s16, s32);

s32 ov112_021F2204(void *, s32, s32);

s32 ov112_021F22B0(ManagedSprite *, s32, s32);

s32 ov112_021F22D0(void *, s32, s32, s32);

void ov112_021F2A78(Ov112SpriteApp *arg0, u32 arg1);

void ov112_021F2A78(Ov112SpriteApp *arg0, u32 arg1) {
    s32 v1;
    u16 v0;

    v0 = arg0->records[arg0->curIdx]->unk_78;
    if ((u32)v0 >= 0x12CU) {
        ov112_021F2204(arg0, 0, 1);
        ov112_021F22B0(arg0->sprites[5], (u8) arg0->playerGender, 3);
        if ((u32)v0 >= 0xFA0U) {
            v1 = 0x4F;
        } else if ((u32)v0 >= (u32)(0xFA0U >> 1)) {
            v1 = 0x50;
        } else if ((u32)v0 >= (u32)(0xFA0U >> 2)) {
            v1 = 0x51;
        } else {
            v1 = 0x52;
        }
    } else {
        ov112_021F2204(arg0, 0, 1);
        ov112_021F1F54(&arg0->spriteRecs[0], 0x80, 0x48, 0);
        ov112_021F1F3C(&arg0->spriteRecs[0], 0);
        ov112_021F22D0(arg0, 0, 8, 0xA);
        if (((arg1 >= 0x14U) && (arg1 <= 0x18U)) || (arg1 <= 7U)) {
            v1 = 0x53;
        } else if ((arg1 >= 0xCU) && (arg1 <= 0x10U)) {
            v1 = 0x54;
        } else {
            v1 = 0x55;
        }
    }
    ov112_021F1624(arg0, v1, 0);
}

void ov112_021F2B2C(Ov112SpriteApp *arg0);

void ov112_021F2B2C(Ov112SpriteApp *arg0) {
    ov112_021F2204(arg0, 0, 1);
    ManagedSprite_SetDrawFlag(arg0->sprites[5], 1);
    ManagedSprite_SetAnim(arg0->sprites[5], 0x1B);
    ManagedSprite_SetDrawFlag(arg0->spriteRecs[1].sprites[2], 1);
    ManagedSprite_SetDrawFlag(arg0->sprites[7], 1);
    ManagedSprite_SetAnim(arg0->sprites[7], 7);
    ov112_021F1624(arg0, 0x19, 0);
}
