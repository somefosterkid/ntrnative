#include "global.h"

#include "error_handling.h"
#include "sprite_system.h"

typedef struct UnkStruct_ov96_0220C680 {
    u8 unk0[0x14];
    ManagedSprite *sprite;
    u8 unk18[0x28];
    u32 unk40_0 : 2;
    u32 unk40_2 : 8;
    u32 unk40_10 : 11;
    u32 timer : 11;
    u32 unk44_0 : 8;
    u32 unk44_8 : 1;
    u32 unk44_9 : 23;
} UnkStruct_ov96_0220C680;

s32 ov96_0220C7FC(UnkStruct_ov96_0220C680 *);

s32 ov96_0220C8B8(ManagedSprite *, u16, s32);

void ov96_0220C680(UnkStruct_ov96_0220C680 *arg0);

void ov96_0220C680(UnkStruct_ov96_0220C680 *arg0) {
    if (arg0->unk40_2 == 3) {
        return;
    }
    if (arg0->unk44_8) {
        arg0->timer++;
        if (arg0->timer >= 90) {
            ov96_0220C7FC(arg0);
        }
    } else if (arg0->unk44_0 != 0) {
        arg0->timer++;
        if (arg0->timer >= 2) {
            arg0->timer = 0;
            arg0->unk44_0--;
        }
        if (arg0->unk44_0 >= 50) {
            ov96_0220C8B8(arg0->sprite, arg0->unk44_0, 1);
        }
    } else {
        ManagedSprite_SetDrawFlag(arg0->sprite, 0);
    }
}

s32 ov96_0220C7C4(void *);

typedef struct {
    u32 val : 8;
    u32 flag : 1;
    u32 unk_9 : 23;
} UnkStruct_ov96_0220C714;

void ov96_0220C714(void *arg0, s32 arg1);

void ov96_0220C714(void *arg0, s32 arg1) {
    if (((UnkStruct_ov96_0220C714 *)((u8 *)arg0 + 0x44))->flag) {
        GF_AssertFail();
    }
    if (arg1 != 0) {
        ((UnkStruct_ov96_0220C714 *)((u8 *)arg0 + 0x44))->val += 30;
    }
    if (((UnkStruct_ov96_0220C714 *)((u8 *)arg0 + 0x44))->val > 100) {
        ((UnkStruct_ov96_0220C714 *)((u8 *)arg0 + 0x44))->val = 100;
    }
    if (((UnkStruct_ov96_0220C714 *)((u8 *)arg0 + 0x44))->val >= 100) {
        ov96_0220C7C4(arg0);
    }
}
