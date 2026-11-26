#include "global.h"

#include "heap.h"
#include "overlay_44.h"
#include "work.h"

s32 sub_02078BD8(Work *work);

s32 sub_02078BD8(Work *work) {
    UnkStruct_ov44_args *args = work->unk10;
    if (args->unk3 == 1) {
        switch (args->unk0) {
        case 1:
            work->state = 0x10;
            break;

        case 2:
            work->state = 0x14;
            break;

        case 3:
        default:
            work->state = 0x18;
            break;
        }

    } else {
        work->state = 1;
    }
    work->unk14 = args->unk4;
    Heap_Free(work->unk10);
    return work->state;
}
