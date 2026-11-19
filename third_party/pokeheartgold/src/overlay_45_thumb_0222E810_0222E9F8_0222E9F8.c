#include "global.h"

#include "error_handling.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

void ov45_0222F88C(struct UnkStruct_ov66_022336C4 *, s32, const void *);

s32 ov45_02232580();

s32 ov45_022325B0(const void *, s32, s32);

void ov45_0222E9F8(const void *arg0);

void ov45_0222E9F8(const void *arg0) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    ov45_0222F88C(_022577C0, ov45_02232580(), arg0);
    ov45_022325B0(arg0, _022577C0->unk_28, 0);
}
