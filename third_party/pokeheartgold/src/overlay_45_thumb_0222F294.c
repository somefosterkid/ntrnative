#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

#include "error_handling.h"
#include "overlay_45_thumb.h"

s32 ov45_0222F294(s32 arg0);

s32 ov45_0223021C(void *, s32);
void *ov45_022302B0(void *, s32);
s32 ov45_0222F2D4(s32 arg0);
s32 ov45_0222F314(s32 arg0);

s32 ov45_0222F294(s32 arg0) {
    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    if (ov45_0222F274(arg0) == 0) {
        return 0;
    }
    if (_022577C0->unk_5B8[arg0].lobbyRecruitInfo.paramVal == 0) {
        return 1;
    }
    return 0;
}

s32 ov45_0222F2D4(s32 arg0) {
    void *v0;

    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    if (arg0 >= 3) {
        GF_AssertFail();
    }
    if (ov45_0223021C(_022577C0, arg0) == 0) {
        return 0;
    }
    v0 = ov45_022302B0(_022577C0, arg0);
    return *(s32 *)((u8 *)v0 + 4) - *(s32 *)((u8 *)v0 + 8);
}

s32 ov45_0222F314(s32 arg0) {
    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    if (arg0 >= 3) {
        GF_AssertFail();
    }
    if (ov45_0223021C(_022577C0, arg0) == 0) {
        return 0;
    }
    return *(s32 *)((u8 *)(ov45_022302B0(_022577C0, arg0)) + 8);
}
