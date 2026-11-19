#include "global.h"

#include "unk_02095EAC.h"
#include "sprite.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "math_util.h"

void ov37_021E657C(Sprite *param0, int param1, int param2);

void ov37_021E73B4(UnkStruct_02095EAC *param0);

void ov37_021E741C(u16 *arg0);
u32 ov37_021E745C(void);

void ov37_021E73B4(UnkStruct_02095EAC *param0) {
    int v0;
    UnkStruct_ov58_021D2820 *v1 = param0->unk_43E6;

    for (v0 = 0; v0 < 5; v0++) {
        if (v0 != sub_0203769C()) {
            if ((v1[v0].unk_08_3 != 0) && (sub_02034818(v0) != NULL)) {
                Sprite_SetDrawFlag(param0->unk_23C[v0], TRUE);
                ov37_021E657C(param0->unk_23C[v0], v1[v0].unk_00[v1[v0].unk_08_3 - 1], v1[v0].unk_04[v1[v0].unk_08_3 - 1]);
            } else {
                Sprite_SetDrawFlag(param0->unk_23C[v0], FALSE);
            }
        }
    }
}

void ov37_021E741C(u16 *arg0) {
    s16 pSrc;
    s32 v0;

    *arg0 += 0x14;
    if ((u32)*arg0 > 0x168U) {
        *arg0 = 0;
    }
    v0 = 0xA * GF_SinDeg(*arg0);
    pSrc = ((((s32)(v0 + ((u32)(v0 >> 0xB) >> 0x14)) >> 0xC) + 0xF) << 5) | 0x1D;
    GX_LoadOBJPltt(&pSrc, 0x18U, 2U);
}

u32 ov37_021E745C(void) {
    u32 a0;
    u32 v0;

    v0 = 0;
    a0 = 0;
    do {
        if (sub_02034818(a0) != NULL) {
            v0 += 1;
        }
        a0 += 1;
    } while ((s32)a0 < 5);
    return v0;
}
