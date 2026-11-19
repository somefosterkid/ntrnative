#include "global.h"

#include "unk_02095EAC.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_02035900.h"

s32 ov37_021E68AC(UnkStruct_02095EAC *);

s32 ov37_021E762C(UnkStruct_02095EAC *, s32, s32);

s32 ov37_021E7844(UnkStruct_02095EAC *, s32);

s32 ov37_021E68D0(UnkStruct_02095EAC *arg0, s32 arg1);

s32 ov37_021E68D0(UnkStruct_02095EAC *arg0, s32 arg1) {
    if (arg0->unk_384 == sub_0203769C()) {
        ov37_021E762C(arg0, 0xD, 0);
    } else {
        ov37_021E762C(arg0, 0, 0);
    }
    ov37_021E7844(arg0, 2);
    PlaySE(0x5E4U);
    G2_SetBlendBrightness(0xE, -6);
    ov37_021E68AC(arg0);
    return arg1;
}

s32 ov37_021E6928(void *arg0, s32 arg1);

s32 ov37_021E6928(void *arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    void *v2;

    v1 = 0;
    v2 = arg0;
    v0 = arg1;
    do {
        Sprite_SetDrawFlag(*(Sprite **)((u8 *)v2 + 0x1D8), 0);
        v1 += 1;
        v2 += 4;
    } while (v1 < 5);
    return v0;
}

void ov37_021E76D0(UnkStruct_02095EAC *param0, int param1);

int ov37_021E694C(UnkStruct_02095EAC *param0, int param1);

int ov37_021E694C(UnkStruct_02095EAC *param0, int param1) {
    G2_BlendNone();

    ov37_021E76D0(param0, 1);
    ov37_021E7844(param0, 4);
    ov37_021E68AC(param0);

    param0->unk_9468 = 0;

    return param1;
}
