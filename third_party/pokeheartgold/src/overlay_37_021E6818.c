#include "global.h"
struct Sprite;

#include "sprite.h"
#include "unk_02035900.h"
#include "unk_02095EAC.h"

extern const u16 ov37_021E7A80[12][3];

void ov37_021E6818(Sprite **param0, int param1);

void ov37_021E6848(struct Sprite *param_1, s32 param_2);

s32 ov37_021E65EC(void *param0, s32 arg1);
s32 ov37_021E72E8(void *, void *, void *, s32);
s32 ov37_021E73B4(u8 *);
s32 ov37_021E745C(void);
s32 ov37_021E755C(u8 *);
s32 ov37_021E6860(u8 *arg0, s32 arg1);

void ov37_021E6818(Sprite **param0, int param1) {
    int v0;

    for (v0 = 0; v0 < 8; v0++) {
        if (v0 == param1) {
            Sprite_SetAnimCtrlSeq(param0[v0], ov37_021E7A80[v0][2] + 1);
        } else {
            Sprite_SetAnimCtrlSeq(param0[v0], ov37_021E7A80[v0][2]);
        }
    }
}

void ov37_021E6848(Sprite *param_1, s32 param_2) {
    if (param_2 == 1) {
        Sprite_SetAnimCtrlSeq(*((Sprite * *)&param_1->scale.z), 0x16);
        return;
    }
    Sprite_SetAnimCtrlSeq(*((Sprite * *)&param_1->scale.z), 0x15);
    return;
}

s32 ov37_021E6860(u8 *arg0, s32 arg1) {
    ov37_021E65EC(arg0, arg1);
    if (sub_0203769C() == 0) {
        if (ov37_021E745C() != 1) {
            ov37_021E755C(arg0);
        }
    } else {
        ov37_021E755C(arg0);
    }
    ov37_021E73B4(arg0);
    ov37_021E72E8(arg0 + 0x2C8, arg0 + 0x4382, arg0 + 0x43B4, 1);
    return arg1;
}
