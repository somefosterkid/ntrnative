#include "global.h"

#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

s32 ov112_021F1624(void *, s32, s32);

s32 ov112_021F2204(void *, s32, s32);

void ov112_021F25FC(Ov112SpriteApp *arg0);

void ov112_021F2768(Ov112SpriteApp *arg0);

void ov112_021F25FC(Ov112SpriteApp *arg0) {
    s32 v3;
    u16 v1;
    u8 v0;
    Ov112Record *v2;

    v2 = arg0->records[arg0->curIdx];
    ov112_021F2204(arg0, 0, 1);
    ManagedSprite_SetDrawFlag(arg0->sprites[5], 1);
    ManagedSprite_SetAnim(arg0->sprites[5], 0xB);
    v0 = arg0->unk_13F;
    if ((v0 != 0) && ((u32)v0 <= (u32) arg0->curIdx)) {
        v3 = 0x1B;
    } else {
        v1 = v2->unk_78;
        if ((u32)v1 >= 0xFA0U) {
            v3 = 0x1C;
        } else if ((u32)v1 >= 0xBB8U) {
            v3 = 0x1D;
        } else if ((u32)v1 >= (u32)(0xFA0U >> 1)) {
            v3 = 0x1E;
        } else if ((u32)v1 >= (u32)(0xFA0U >> 2)) {
            v3 = 0x1F;
        } else {
            v3 = 0x20;
        }
    }
    ov112_021F1624(arg0, 0x1A, v3);
}

s32 ov112_021F22B0(ManagedSprite *, s32, s32);

void ov112_021F2688(Ov112SpriteApp *arg0);

void ov112_021F2688(Ov112SpriteApp *arg0) {
    s32 v1;
    Ov112Record *v0;

    v0 = arg0->records[arg0->curIdx];
    ManagedSprite_SetDrawFlag(arg0->sprites[5], 1);
    ManagedSprite_SetAnim(arg0->sprites[5], 0xD);
    if (v0->speciesA != 0) {
        v1 = 0x21;
        ov112_021F2204(arg0, 0, 1);
    } else {
        v1 = 0x22;
        ov112_021F22B0(arg0->sprites[4], (u8) arg0->playerGender, 3);
    }
    ov112_021F1624(arg0, v1, 0);
}

s32 ov112_021F24C4(void *);

void ov112_021F26E8(Ov112SpriteApp *arg0);

void ov112_021F26E8(Ov112SpriteApp *arg0) {
    s32 v3;
    u16 v1;
    u8 v0;
    Ov112Record *v2;

    v2 = arg0->records[arg0->curIdx];
    ov112_021F2204(arg0, 0, 1);
    ov112_021F2204(arg0, 1, 0);
    v0 = arg0->unk_140;
    if ((v0 != 0) && ((u32)v0 <= (u32) arg0->curIdx)) {
        v3 = 0x24;
    } else {
        v1 = v2->unk_78;
        if ((u32)v1 >= 0xFA0U) {
            v3 = 0x25;
        } else if ((u32)v1 >= 0xBB8U) {
            v3 = 0x26;
        } else if ((u32)v1 >= (u32)(0xFA0U >> 1)) {
            v3 = 0x27;
        } else if ((u32)v1 >= (u32)(0xFA0U >> 2)) {
            v3 = 0x28;
        } else {
            v3 = 0x29;
        }
    }
    ov112_021F1624(arg0, 0x23, v3);
    ov112_021F24C4(arg0);
}

void ov112_021F2768(Ov112SpriteApp *arg0) {
    s32 v0;

    if (arg0->records[arg0->curIdx]->speciesA != 0) {
        v0 = 0x2A;
        ov112_021F2204(arg0, 0, 1);
    } else {
        v0 = 0x2B;
        ov112_021F22B0(arg0->sprites[4], (u8) arg0->playerGender, 3);
    }
    ov112_021F2204(arg0, 1, 0);
    ov112_021F1624(arg0, v0, 0);
}
