#include "global.h"

#include "error_handling.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"
#include "overlay_44.h"

s32 ov45_0222EDA8(void);

s32 ov45_0222EDA8(void) {
    if (_022577C0->unk_130 == 0) {
        return 1;
    }
    return 0;
}

u16 ov45_0222EDC4(u32 arg0);

u16 ov45_0222EDC4(u32 arg0) {
    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    if (arg0 >= 0x14U) {
        GF_AssertFail();
    }
    return _022577C0->unk_134[arg0].country;
}

u8 ov45_0222EDF0(u32 arg0);

u8 ov45_0222EDF0(u32 arg0) {
    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    if (arg0 >= 0x14U) {
        GF_AssertFail();
    }
    return _022577C0->unk_134[arg0].region;
}

void ov45_0222EE20(void *arg0, s32 arg1, void *arg2);

void ov45_0222EE20(void *arg0, s32 arg1, void *arg2) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    _022577C0->unk_184[0].unk_00 = arg0;
    _022577C0->unk_184[0].unk_04 = arg1;
    _022577C0->unk_184[0].unk_08 = arg2;
}

void ov45_0222EE50(const UnkStruct_ov66_022589B4 *arg0, u32 arg1, void *arg2);

void ov45_0222EE50(const UnkStruct_ov66_022589B4 *arg0, u32 arg1, void *arg2) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    _022577C0->unk_184[1].unk_00 = arg0;
    _022577C0->unk_184[1].unk_04 = arg1;
    _022577C0->unk_184[1].unk_08 = arg2;
}

void ov45_0222EE80(void);

void ov45_0222EE80(void) {

    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    memset(&_022577C0->unk_184[0], 0, sizeof(_022577C0->unk_184[0]));
}

void ov45_0222EEB8(void);

void ov45_0222EEB8(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    memset(&_022577C0->unk_184[1], 0, sizeof(_022577C0->unk_184[1]));
}

s32 ov45_0222F74C();

s32 ov45_0223247C(s32, u32, s32, s32);

void ov45_0222EEF0(u32 arg0, s32 arg1, s32 arg2);

void ov45_0222EEF0(u32 arg0, s32 arg1, s32 arg2) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if (_022577C0->unk_184[0].unk_04 <= arg0) {
        GF_AssertFail();
    }
    if (arg2 != _022577C0->unk_184[0].unk_00[arg0].unk_04) {
        GF_AssertFail();
    }
    ov45_0223247C(0, arg0, arg1, arg2);
    if (ov45_0222F74C() != 1) {
        GF_AssertFail();
    }
}

s32 ov45_02232500(s32, u32, void *, s32);

void ov45_0222EF4C(u32 arg0, s32 arg1, void *arg2, s32 arg3);

void ov45_0222EF4C(u32 arg0, s32 arg1, void *arg2, s32 arg3) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if (_022577C0->unk_184[0].unk_04 <= arg0) {
        GF_AssertFail();
    }
    if (arg3 != _022577C0->unk_184[0].unk_00[arg0].unk_04) {
        GF_AssertFail();
    }
    ov45_02232500(arg1, arg0, arg2, arg3);
    if (ov45_0222F74C() != 1) {
        GF_AssertFail();
    }
}

s32 ov45_0222F7B0(s32);

void ov45_0222EFAC(u32 arg0, s32 arg1, s32 arg2);

void ov45_0222EFAC(u32 arg0, s32 arg1, s32 arg2) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if ((u32) _022577C0->unk_184[1].unk_04 <= arg0) {
        GF_AssertFail();
    }
    if (arg2 != _022577C0->unk_184[1].unk_00[arg0].unk_04) {
        GF_AssertFail();
    }
    if (_022577C0->unk_12D == 4) {
        GF_AssertFail();
    }
    ov45_0223247C(ov45_0222F7B0(_022577C0->unk_12D), arg0, arg1, arg2);
    if (ov45_0222F74C() != 1) {
        GF_AssertFail();
    }
}
