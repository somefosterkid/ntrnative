#include "global.h"

#include "field/fieldmap.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "overlay_01_unk_struct_ov5_02202120.h"
#include "unk_02014DA0.h"

void ov01_021F0E74(void) {
    if (ov01_02209B64 == NULL) {
        GF_AssertFail();
    }
    ov01_02209B64->unk_03 = 1;
}

u8 ov01_021F0E90(void) {
    if (ov01_02209B64 == NULL) {
        GF_AssertFail();
    }
    return ov01_02209B64->unk_02;
}

void ov01_021F0EAC(void) {
    if (ov01_02209B64 != 0) {
        sub_0201543C();
    }
}

s32 ov01_021F1210(FieldSystem *);

s32 sub_02014F84(void);

void ov01_021F0EC0(void) {
    if (ov01_02209B64 == NULL) {
        GF_AssertFail();
    }
    sub_02014F84();
    ov01_021F1210(ov01_02209B64->fieldSystem);
    ov01_021E631C(ov01_02209B64->fieldSystem, 1);
    ov01_021E6340(ov01_02209B64->fieldSystem, 1);
    Heap_Free(ov01_02209B64);
    ov01_02209B64 = NULL;
}
