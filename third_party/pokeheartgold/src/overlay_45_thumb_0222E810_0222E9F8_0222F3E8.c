#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

#include "error_handling.h"

s32 ov45_0223021C(void *, s32);

u16 ov45_0222F3E8(s32 arg0);

u16 ov45_0222F3E8(s32 arg0) {
    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    if (arg0 >= 3) {
        GF_AssertFail();
    }
    if (ov45_0223021C(_022577C0, arg0) == 0) {
        return 0x708U;
    }
    return _022577C0->unk_5B8[arg0].unk_46;
}

s32 ov45_0222F430(void);

s32 ov45_0222F430(void) {
    u16 v0;

    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    v0 = _022577C0->gameKind;
    if (v0 == 3) {
        return -1;
    }
    return _022577C0->unk_5B8[v0].unk_48;
}

s32 ov45_0222F464(void);

s32 ov45_0222F464(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    return (s32) _022577C0->unk_69E;
}

s32 ov45_02230434(const UnkStruct_ov66_022336C4_sub5 *, s32);

void ov45_0222F484(s32 arg0);

void ov45_0222F484(s32 arg0) {
    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    ov45_02230434(&_022577C0->unk_6A0, arg0);
}

s32 ov45_0223045C(const UnkStruct_ov66_022336C4_sub5 *, s32);

void ov45_0222F4AC(s32 arg0);

void ov45_0222F4AC(s32 arg0) {
    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    ov45_0223045C(&_022577C0->unk_6A0, arg0);
}

s32 ov45_0222F74C();

s32 ov45_02232E60(s32);

void ov45_0222F4D4(s32 arg0);

void ov45_0222F4D4(s32 arg0) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if (arg0 >= 3) {
        GF_AssertFail();
    }
    if (_022577C0->unk_6A8.unk_2D8 == 1) {
        GF_AssertFail();
    }
    ov45_02232E60(arg0);
    if (ov45_0222F74C() != 1) {
        GF_AssertFail();
    }
    _022577C0->unk_6A8.unk_2D8 = 1;
}

s32 ov45_0222F524(void);

s32 ov45_0222F524(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    return _022577C0->unk_6A8.unk_2D8;
}
