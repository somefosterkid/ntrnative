#include "global.h"

#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

s32 ov112_021F1624(void *, s32, s32);

s32 ov112_021F2204(void *, s32, s32);

void ov112_021F2810(Ov112SpriteApp *arg0);

s32 ov112_021F22B0(ManagedSprite *, s32, s32);
void ov112_021F2874(Ov112SpriteApp *arg0);

void ov112_021F28DC(Ov112SpriteApp *arg0);

void ov112_021F2810(Ov112SpriteApp *arg0) {
    s32 v2;
    u8 v0;
    Ov112Record *v1;

    v2 = 0;
    v1 = arg0->records[arg0->curIdx];
    ov112_021F2204(arg0, 0, 1);
    ManagedSprite_SetDrawFlag(arg0->sprites[5], 1);
    ManagedSprite_SetAnim(arg0->sprites[5], 0xC);
    v0 = v1->unk_77;
    if ((u32)v0 >= 0xFAU) {
        v2 = 0x2F;
    } else if ((u32)v0 >= 0xC8U) {
        v2 = 0x30;
    } else if ((u32)v0 >= 0x96U) {
        v2 = 0x31;
    } else if ((u32)v0 >= 0x5AU) {
        v2 = 0x32;
    }
    ov112_021F1624(arg0, 0x2E, v2);
}

void ov112_021F2874(Ov112SpriteApp *arg0) {
    s32 v2;
    u8 v1;
    Ov112Record *v0;
    v0 = arg0->records[arg0->curIdx];
    v1 = v0->unk_77;
    v2 = (((v0->kind) - 0x12) * 3) + 0x33;
    if (((s32)v1) < 0xC8) {
        if (((s32)v1) >= 0x3C) {
            v2 += 1;
        } else if (((s32)(v0->unk_77)) >= 0) {
            v2 += 2;
        }
    }
    if (v2 == 0x35) {
        v2 = 0x34;
    }
    ov112_021F2204(arg0, 0, 1);
    ov112_021F22B0(arg0->sprites[5], (u8)arg0->playerGender, 1);
    ov112_021F1624(arg0, v2, 0);
}

void ov112_021F28DC(Ov112SpriteApp *arg0) {
    ov112_021F2204(arg0, 0, 1);
    ov112_021F22B0(arg0->sprites[5], (u8) arg0->playerGender, 3);
    ManagedSprite_SetDrawFlag(arg0->sprites[7], 1);
    ManagedSprite_SetAnim(arg0->sprites[7], 8);
    ov112_021F1624(arg0, 0x42, 0);
}
