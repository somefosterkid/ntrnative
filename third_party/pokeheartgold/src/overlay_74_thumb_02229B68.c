#include "global.h"

#include "heap.h"
#include "sys_task_api.h"
#include "unk_02035900.h"

void ov74_02229CB8(SysTask *task, void *data);

void ov74_02229E14(void *arg0);

extern void *ov74_0223D0A4;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov74_02229B68(void *arg0);
#else
char ov74_02229B68(void *arg0);
#endif

void ov74_02229BC0(void *arg0, s16 arg1);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov74_02229B68(void *arg0) {
#else
char ov74_02229B68(void *arg0) {
#endif
    void *dest;
    if (ov74_0223D0A4 == 0L) {
        ov74_02229E14(arg0);
        dest = Heap_Alloc(HEAP_ID_15, 0x3D4U);
        ov74_0223D0A4 = dest;
        MI_CpuFill8(dest, 0U, 0x3D4U);
        *((s16 *)(((u8 *)ov74_0223D0A4) + 0x24)) = 0x32;
        *((SysTask **)(((u8 *)ov74_0223D0A4) + 0x1C)) = SysTask_CreateOnMainQueue(ov74_02229CB8, 0L, 0xAU);
        *((void **)ov74_0223D0A4) = arg0;
        sub_0203778C(ov74_0223D0A4 + 4);
    }
}

void ov74_02229BC0(void *arg0, s16 arg1) {
    *(void **)((u8 *)ov74_0223D0A4 + 0x20) = arg0;
    *(s16 *)((u8 *)ov74_0223D0A4 + 0x24) = arg1;
}
