#include "global.h"

#include "heap.h"
#include "poke_overlay.h"

s32 ov45_0222CE94(void *);

void ov45_0222D2E4(void **arg0);

void ov45_0222D2E4(void **arg0) {
    ov45_0222CE94(*arg0);
    Heap_Free(*arg0);
}

s32 ov45_0222CDC4(void *, s32 *, s32 *);

s32 ov45_0222CEB0(s32, s32, s32);

FS_EXTERN_OVERLAY(OVY_90);

extern s32 ov45_02254B34;

void ov45_0222D2F8(void *arg0, s32 *arg1);

void ov45_0222D2F8(void *arg0, s32 *arg1) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    ov45_0222CEB0(*arg1, *(s32 *)((u8 *)arg0 + 0x3C), *(u16 *)((u8 *)arg0 + 0x3A));
    ov45_0222CDC4(arg0, arg1, &ov45_02254B34);
}
