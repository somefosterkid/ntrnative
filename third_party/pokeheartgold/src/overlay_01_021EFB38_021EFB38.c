#include "global.h"
#include "sys_task.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
// DS: ov01_02209B64 is one 0xC-byte .bss block holding three objects: the ov5 effect work pointer (+0, 4 bytes),
// the encounter effect state (+4) and its tick counter (+8). The pointer is 8 bytes on the host and would cover the
// state, so the host keeps state and counter in objects of their own (ov01_021EFC94 sets them).
s32 ov01_02209B68;
s32 ov01_02209B6C;
#else
typedef struct {
    s32 unk0;
    s32 state;
    s32 count;
} Ov01Data;

extern Ov01Data ov01_02209B64;
#endif

extern void (*ov01_022068C4[])(void *, void *);

void ov01_021EFB38(SysTask *a0, void *a1);

void ov01_021EFB38(SysTask *a0, void *a1) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    ov01_022068C4[ov01_02209B68](a0, a1);
    ov01_02209B6C++;
#else
    ov01_022068C4[ov01_02209B64.state](a0, a1);
    ov01_02209B64.count++;
#endif
    OS_GetTick();
    OS_SetTick(0);
}
