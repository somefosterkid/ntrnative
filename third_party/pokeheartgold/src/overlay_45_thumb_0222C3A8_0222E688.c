#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

#include "error_handling.h"
#include "heap.h"

s32 ov45_0222F878(void *);

s32 ov45_0222FA10(void *);

void ov45_022303FC(UnkStruct_ov66_022336C4_sub5 *);

void ov45_0222E688(void);

void ov45_0222E688(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    ov45_022303FC(&_022577C0->unk_6A0);
    ov45_0222FA10(_022577C0);
    ov45_0222F878(_022577C0);
    Heap_Free(_022577C0);
    _022577C0 = NULL;
}
