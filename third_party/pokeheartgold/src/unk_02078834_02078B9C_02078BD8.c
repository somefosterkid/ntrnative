#include "global.h"
struct Work;

#include "heap.h"
#include "work.h"

s32 sub_02078BD8(struct Work *arg0);

s32 sub_02078BD8(Work *arg0) {
    unsigned char temp_r1;
    void *temp_r0;
    temp_r0 = arg0->unk10;
    if ((*((u8 *)(((u8 *)temp_r0) + 3))) == 1) {
        temp_r1 = *((u8 *)temp_r0);
        switch (temp_r1) {
        case 1:
            *((s32 *)&arg0->state) = 0x10;
            break;

        case 2:
            *((s32 *)&arg0->state) = 0x14;
            break;

        case 3:
        default:
            *((s32 *)&arg0->state) = 0x18;
            break;
        }

    } else {
        *((s32 *)&arg0->state) = 1;
    }
    *((s32 *)&arg0->unk14) = (s32)(*((u8 *)(((u8 *)temp_r0) + 4)));
    Heap_Free(arg0->unk10);
    return *((s32 *)&arg0->state);
}
