#include "global.h"

#include "sprite.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "bg_window.h"

typedef struct UnkStruct_ov72_0223B2C0 {
    u8 unk_0[0x4];
    Sprite *unk_4;
    u8 unk_8[0x27];
    u8 unk_2F;
    u8 unk_30[0x2];
    u8 unk_32;
} UnkStruct_ov72_0223B2C0;

typedef struct UnkStruct_ov72_0223AF7C {
    u8 unk_0[0x4];
    Sprite *unk_4;
    u8 unk_8[0x25];
    s8 unk_2D;
    u8 unk_2E[0x5];
    u8 unk_33;
} UnkStruct_ov72_0223AF7C;

s32 ov72_0223B2FC(void *, s32);

extern TouchscreenHitbox ov72_0223B4C8;

s32 ov72_0223AF7C(UnkStruct_ov72_0223AF7C *arg0);

s32 ov72_0223B084(void);
s32 ov72_0223B2C0(void *);
s32 ov72_0223B088(UnkStruct_ov72_0223B2C0 *arg0);

s32 ov72_0223AF7C(UnkStruct_ov72_0223AF7C *arg0) {
    s32 v0;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(&ov72_0223B4C8);
    if (v0 != -1) {
        PlaySE(0x5DCU);
        if (v0 == 0) {
            arg0->unk_33 = 0U;
            ov72_0223B2FC(arg0, arg0->unk_33);
            Sprite_SetAnimCtrlSeq(arg0->unk_4, 3);
            arg0->unk_2D = 3;
        } else if (v0 == 1) {
            arg0->unk_33 = 1U;
            ov72_0223B2FC(arg0, arg0->unk_33);
            Sprite_SetAnimCtrlSeq(arg0->unk_4, 3);
            arg0->unk_2D = 3;
        }
    } else if (0x40 & gSystem.newKeys) {
        arg0->unk_33 = 0U;
        ov72_0223B2FC(arg0, arg0->unk_33);
        PlaySE(0x5DCU);
    } else if (0x80 & gSystem.newKeys) {
        arg0->unk_33 = 1U;
        ov72_0223B2FC(arg0, arg0->unk_33);
        PlaySE(0x5DCU);
    } else if (gSystem.newKeys & 1) {
        Sprite_SetAnimCtrlSeq(arg0->unk_4, 3);
        arg0->unk_2D = 3;
        PlaySE(0x5DCU);
    } else if (gSystem.newKeys & 2) {
        arg0->unk_33 = 1U;
        ov72_0223B2FC(arg0, arg0->unk_33);
        arg0->unk_2D = 3;
        Sprite_SetAnimCtrlSeq(arg0->unk_4, 3);
        PlaySE(0x5DCU);
    }
    return 0;
}

s32 ov72_0223B084(void) {
    return 0;
}

s32 ov72_0223B088(UnkStruct_ov72_0223B2C0 *arg0) {
    if (Sprite_IsAnimated(arg0->unk_4) != 0) {
        return 0;
    }
    ToggleBgLayer(arg0->unk_2F, 0U);
    ToggleBgLayer(arg0->unk_32, 0U);
    ov72_0223B2C0(arg0);
    Sprite_SetDrawFlag(arg0->unk_4, 0);
    return 1;
}
