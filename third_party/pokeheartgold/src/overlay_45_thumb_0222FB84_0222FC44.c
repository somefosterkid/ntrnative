#include "global.h"

#include "error_handling.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

s32 ov45_0222EB94(void);

s32 ov45_0222F74C();

s32 ov45_0222F7A8(s32);

s32 ov45_0222FA74(void *, s32, s32);

s32 ov45_0222FB84(void *, s32);

s32 ov45_02232170(void *, void *);

s32 ov45_02232304(s32, s32, void *, s32);

s32 ov45_02232580();

extern void *ov45_02254EF0;

void ov45_0222FC44(s32 arg0, s32 arg1);

void ov45_0222FC44(s32 arg0, s32 arg1) {
    UnkStruct_ov66_022336C4 *v0;

    if (arg1 == 0) {
        _022577C0->unk_5B4 = 0x400;
        ov45_02232170(_022577C0->unk_1B4, &_022577C0->unk_5B4);
        if (ov45_0222F74C() == 0) {
            GF_AssertFail();
        }
        ov45_02232304(-1, 0, ov45_02254EF0, 0);
        return;
    }
    v0 = _022577C0;
    if (v0->unk_12D == ov45_0222F7A8(arg1)) {
        if (arg0 == 1) {
            v0->unk_12F = 1;
            ov45_0222FB84(_022577C0, _022577C0->unk_12D);
            ov45_0222FA74(_022577C0, ov45_02232580(), 1);
            return;
        }
        v0->unk_12F = 2;
        return;
    }
    GF_AssertFail();
    ov45_0222EB94();
}

s32 ov45_0222F898(void *, s32, const void *, s32);

const void *ov45_0222F8D8(void *, s32);

s32 ov45_0222F928(void *, s32, void *);

s32 ov45_022320C4(void *);

void ov45_0222FCE0(s32 arg0, void *arg1, void *arg2, s32 arg3);

void ov45_0222FCE0(s32 arg0, void *arg1, void *arg2, s32 arg3) {
    s32 v0;
    s32 v2;
    UnkStruct_ov66_022336C4 *v1;

    v0 = arg3;
    if (arg0 == ov45_02232580()) {
        ov45_022320C4(&_022577C0->timeInfo);
        v2 = 1;
    } else {
        v2 = 0;
    }
    ov45_0222FA74(_022577C0, arg0, 0);
    ov45_0222F898(_022577C0, arg0, arg2, v0);
    ov45_0222F928(_022577C0, arg0, arg1);
    v1 = _022577C0;
    _022577C0->unk_0C.unk_00(arg0, ov45_0222F8D8(v1, arg0), v1->unk_20, v2);
}

s32 ov45_0222F8FC(void *, s32);

s32 ov45_0222F978(void *, s32);

s32 ov45_0222FAF8(void *, s32, s32);

s32 ov45_0222FB24(void *, s32, s32);

s32 ov45_02230008(s32, void *);

void ov45_0222FD50(s32 arg0);

void ov45_0222FD50(s32 arg0) {
    s32 v0;
    s32 v1;

    if (ov45_0222FB24(_022577C0, arg0, 0) != -1) {
        v1 = 0;
        v0 = 0;
        do {
            if (arg0 == _022577C0->unk_5B8[v1].unk_48) {
                ov45_02230008(arg0, &_022577C0->unk_5B8[v1]);
            }
            v1 += 1;
            v0 += 0x4C;
        } while (v1 < 3);
        _022577C0->unk_0C.unk_04(arg0, _022577C0->unk_20);
        ov45_0222F978(_022577C0, arg0);
        ov45_0222F8FC(_022577C0, arg0);
        ov45_0222FAF8(_022577C0, arg0, 0);
        ov45_0222FAF8(_022577C0, arg0, 1);
    }
}

void ov45_0222FDD4(void);

void ov45_0222FDD4(void) {
}
