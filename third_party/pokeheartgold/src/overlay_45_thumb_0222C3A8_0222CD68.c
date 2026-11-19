#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_manager.h"

s32 ov45_0222CE0C(void *, s32);

void ov45_0222CD68(void *arg0);

void ov45_0222CD68(void *ptr) {
    s32 v0;

    v0 = 0;
    do {
        ov45_0222CE0C(ptr, v0);
        v0 += 1;
    } while (v0 < 0xD);
    Heap_Free(ptr);
}

s32 ov45_0222CE2C(void *, s32);

void ov45_0222CD84(void *arg0);

void ov45_0222CD84(void *arg0) {
    ov45_0222CE2C(arg0, 5);
}

s32 ov45_0222CE54(void *, u32);

s32 ov45_0222CD90(void *arg0);

s32 ov45_0222CD90(void *arg0) {
    OverlayManager *man;
    s32 v0;

    man = *(OverlayManager **)arg0;
    v0 = 0;
    if ((man != NULL) && (OverlayManager_Run(man) != 0)) {
        OverlayManager_Delete(*(OverlayManager **)arg0);
        *(OverlayManager **)arg0 = NULL;
        v0 = ov45_0222CE54(arg0, *(u8 *)((u8 *)arg0 + 0x38));
    }
    return v0;
}

void ov45_0222CDC0(void);

void ov45_0222CDC0(void) {
}

void ov45_0222CDC4(void *arg0, void **arg1, OverlayManagerTemplate *arg2);

void ov45_0222CDC4(void *arg0, void **arg1, OverlayManagerTemplate *template) {
    if (*(OverlayManager **)arg0 != NULL) {
        GF_AssertFail();
    }
    *(OverlayManager **)arg0 = OverlayManager_New(template, *arg1, (enum HeapID) * (u16 *)((u8 *)arg0 + 0x3A));
}
