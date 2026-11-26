#include "global.h"

#include "sys_task.h"
#include "error_handling.h"
#include "overlay_99.h"

void Heap_Free(void *ptr);

void ov99_021E5B74(void *ptr);

void ov99_021E5B74(void *ptr) {
    Heap_Free(ptr);
}

void *ov99_021E5B7C(PokeathlonCourseRecordArgs *param_1, s32 param_2, s32 param_3) {
    return (u8 *)param_1->pokeathlon + param_2 * 0x2c + 8 + param_3 * 0xc;
}

u16 ov99_021E5B90(PokeathlonCourseRecordArgs *arg0, s32 arg1) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg1 >= 5) {
        GF_AssertFail();
    }
    return *(u16 *)((u8 *)arg0->pokeathlon + (0x2C * arg1) + 6);
}
