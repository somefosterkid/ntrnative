#include "global.h"

#include "error_handling.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

s32 ov45_0222F74C();

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov45_022301E0(void *, u32, s32);
#else
s32 ov45_022301E0(void *, u16, s32);
#endif

s32 ov45_02232580();

s32 ov45_02232CA4();

void ov45_0222F154(void);

void ov45_0222F154(void) {
    UnkStruct_ov66_022336C4 *v0;

    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if ((_022577C0->gameKind != 3) && (_022577C0->unk_69E != 0)) {
        ov45_02232CA4();
        if (ov45_0222F74C() == 0) {
            GF_AssertFail();
        }
        v0 = _022577C0;
        if (ov45_022301E0(v0, v0->gameKind, ov45_02232580()) == 0) {
            GF_AssertFail();
        }
        _022577C0->unk_69E = 0U;
    }
}

s32 ov45_02232BB0(void *);

void ov45_0222F1BC(void);

void ov45_0222F1BC(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if (_022577C0->unk_69E != 0) {
        _022577C0->unk_5B8[_022577C0->gameKind].lobbyRecruitInfo.paramVal = 1;
        ov45_02232BB0(&_022577C0->unk_5B8[_022577C0->gameKind]);
        if (ov45_0222F74C() == 0) {
            GF_AssertFail();
        }
    }
}

s32 ov45_0222F218(void);

s32 ov45_0222F218(void) {
    u16 v0;

    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    v0 = _022577C0->gameKind;
    if (v0 == 3) {
        return 0;
    }
    if (_022577C0->unk_5B8[v0].lobbyRecruitInfo.paramVal == 1) {
        return 1;
    }
    return 0;
}

s32 ov45_0222F254(void);

s32 ov45_0222F254(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    return (s32) _022577C0->unk_69F;
}
