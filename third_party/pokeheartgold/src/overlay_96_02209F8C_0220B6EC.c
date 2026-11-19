#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"

typedef struct UnkStruct_ov96_0220B788 {
    u8 unk_0[0x44];
    s32 unk_44;
} UnkStruct_ov96_0220B788;

typedef struct UnkStruct_ov96_0220B774 {
    u8 unk_0[0x44];
    s32 unk_44;
} UnkStruct_ov96_0220B774;

typedef struct UnkStruct_ov96_0220B758 {
    u8 unk_0[0x34];
    u8 unk_34;
    u8 unk_35[0xB];
    s32 unk_40;
} UnkStruct_ov96_0220B758;

typedef struct UnkStruct_ov96_0220B744 {
    u8 unk_0[0x40];
    s32 unk_40;
} UnkStruct_ov96_0220B744;

typedef struct UnkStruct_ov96_0220B730 {
    u8 unk_0[0x40];
    s32 unk_40;
} UnkStruct_ov96_0220B730;

typedef struct UnkStruct_ov96_0220B6EC {
    u8 unk_0[0x40];
    s32 unk_40;
} UnkStruct_ov96_0220B6EC;

s32 ov96_0220C578(void *, s32);

void ov96_0220B6EC(UnkStruct_ov96_0220B6EC *arg0);

void ov96_0220B6EC(UnkStruct_ov96_0220B6EC *arg0) {
    if (((u32)(arg0->unk_40 << 0x16) >> 0x18) == 3) {
        GF_AssertFail();
    }
    ov96_0220C578(arg0, 3);
}

void ov96_0220B708(u8 *param_1, s32 param_2);

void ov96_0220B708(u8 *param_1, s32 param_2) {
    s32 iVar1;

    if (param_1 == NULL) {
        GF_AssertFail();
    }
    iVar1 = 0;
    do {
        if (*(ManagedSprite **)(param_1 + 0x10) != NULL) {
            ManagedSprite_SetAnimateFlag(*(ManagedSprite **)(param_1 + 0x10), param_2);
        }
        iVar1 = iVar1 + 1;
        param_1 = param_1 + 4;
    } while (iVar1 < 9);
    return;
}

u32 ov96_0220B730(UnkStruct_ov96_0220B730 *arg0);

u32 ov96_0220B730(UnkStruct_ov96_0220B730 *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    return (u32)(arg0->unk_40 << 0x16) >> 0x18;
}

s32 ov96_0220B744(UnkStruct_ov96_0220B744 *arg0);

s32 ov96_0220B744(UnkStruct_ov96_0220B744 *arg0) {
    s32 v1;
    u32 v0;

    v1 = 1;
    v0 = (u32)(arg0->unk_40 << 0x16) >> 0x18;
    if ((v0 != 0) && (v0 != 1)) {
        v1 = 0;
    }
    return v1;
}

void *ov96_0220B758(UnkStruct_ov96_0220B758 *arg0);

void *ov96_0220B758(UnkStruct_ov96_0220B758 *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    return (void *)&arg0->unk_34 + (((u32)(arg0->unk_40 << 0x1E) >> 0x1E) * 4);
}

u32 ov96_0220B774(UnkStruct_ov96_0220B774 *arg0);

u32 ov96_0220B774(UnkStruct_ov96_0220B774 *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    return (u32)(arg0->unk_44 << 0x17) >> 0x1F;
}

u32 ov96_0220B788(UnkStruct_ov96_0220B788 *arg0);

u32 ov96_0220B788(UnkStruct_ov96_0220B788 *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    return (u32)(arg0->unk_44 << 0x16) >> 0x1F;
}
