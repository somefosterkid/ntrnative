#include "global.h"

#include "error_handling.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

s32 ov45_0222E8F4(void);

s32 ov45_0222E8F4(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if (_022577C0->unk_12C == 2) {
        return 1;
    }
    return 0;
}

s32 ov45_0222F74C();

s32 ov45_0222F78C(void);

s32 ov45_02231490();

void ov45_0222E91C(void);

void ov45_0222E91C(void) {
    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    ov45_0222F78C();
    ov45_02231490();
    if (ov45_0222F74C() == 0) {
        GF_AssertFail();
    }
}

s32 ov45_0222E944(void);

s32 ov45_0222E944(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if (_022577C0->unk_12C == 0) {
        return 1;
    }
    return 0;
}

u8 ov45_0222E96C(void);

u8 ov45_0222E96C(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if ((_022577C0->unk_12D != 4) && (_022577C0->unk_12E == 4)) {
        return 4U;
    }
    if (_022577C0->unk_133 != 0) {
        return 4U;
    }
    if (_022577C0->unk_132 != 0) {
        return 4U;
    }
    return _022577C0->unk_12C;
}

u8 ov45_0222E9BC(void);

u8 ov45_0222E9BC(void) {
    u8 v0;

    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    v0 = _022577C0->unk_131;
    _022577C0->unk_131 = 0U;
    return v0;
}
