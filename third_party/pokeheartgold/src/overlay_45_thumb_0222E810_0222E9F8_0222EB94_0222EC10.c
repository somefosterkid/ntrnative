#include "global.h"

#include "error_handling.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"
#include "unk_struct_ov66_02232B20.h"

void ov45_0222EC10(UnkStruct_ov66_02232B20 *arg0);

void ov45_0222EC10(UnkStruct_ov66_02232B20 *arg0) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    arg0->unk_00 = _022577C0->unk_11C[0].unk_00;
    arg0->unk_04 = _022577C0->unk_11C[0].unk_04;
}

void ov45_0222EC3C(UnkStruct_ov66_02232B20 *arg0);

void ov45_0222EC3C(UnkStruct_ov66_02232B20 *arg0) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    arg0->unk_00 = _022577C0->unk_11C[1].unk_00;
    arg0->unk_04 = _022577C0->unk_11C[1].unk_04;
}
