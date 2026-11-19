#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

s32 ov45_0222FB24(void *, s32, s32);

s32 ov45_022301E0(void *, s32, s32);

void ov45_02230008(s32 arg0, s32 *arg1);

void ov45_02230008(s32 arg0, s32 *arg1) {
    s32 v0;

    if (ov45_0222FB24(_022577C0, arg0, 0) != -1) {
        v0 = ov45_022301E0(_022577C0, *arg1, arg0);
        if ((_022577C0->gameKind == *arg1) && (v0 == 1)) {
            _022577C0->unk_69F = 1;
        }
    }
}

void ov45_02230050(PPW_LOBBY_TIME_EVENT arg0);

void ov45_02230050(PPW_LOBBY_TIME_EVENT arg0) {
    _022577C0->unk_0C.unk_0C(arg0, _022577C0->unk_20);
}

void ov45_02230064(s32 arg0, const u8 *arg1, u32 arg2);

void ov45_02230064(s32 arg0, const u8 *arg1, u32 arg2) {
    if (arg0 != 0) {
        _022577C0->unk_133 = 1;
        return;
    }
    _022577C0->unk_0C.unk_10(arg1, arg2, _022577C0->unk_20);
}

s32 ov45_02230090(s32 arg0);

s32 ov45_02230090(s32 arg0) {
    if (arg0 == 0) {
        return 1;
    }
    _022577C0->unk_132 = 1;
    return 0;
}
