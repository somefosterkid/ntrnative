#include "global.h"

#include "heap.h"
#include "scrcmd_9.h"

void *ov80_02229F04(void *, u32, u32, u32);

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov80_022364A4(FrontierFieldSystem *param_1, void *param_2, u8 param_3, u8 *param_4, u8 param_5, s16 *param_6, s16 *param_7, u32 *param_8, u32 param_9);
#else
BOOL ov80_022364A4(FrontierFieldSystem *param_1, void *param_2, u32 param_3, u8 *param_4, u8 param_5, s16 *param_6, s16 *param_7, u32 *param_8, u32 param_9);
#endif

u32 ov80_02236B04(u8);

u32 ov80_02236450(u8 *param_1, u8 *param_2, u32 param_3, u32 param_4, s16 *param_5, s16 *param_6, u32 *param_7, u32 param_8);

u32 ov80_02236450(u8 *param_1, u8 *param_2, u32 param_3, u32 param_4, s16 *param_5, s16 *param_6, u32 *param_7, u32 param_8) {
    BOOL bVar1;
    u32 uVar2;
    void *ptr;

    uVar2 = ov80_02236B04(*(u8 *)(param_1 + 0xf));
    ptr = (void *)ov80_02229F04(param_2, param_3, param_8, uVar2);
    bVar1 = ov80_022364A4((FrontierFieldSystem *)param_1, (u8 *)ptr, param_3, param_2 + 0x30, (u8)param_4, param_5, param_6, param_7, param_8);
    Heap_Free(ptr);
    return (u32)bVar1;
}
