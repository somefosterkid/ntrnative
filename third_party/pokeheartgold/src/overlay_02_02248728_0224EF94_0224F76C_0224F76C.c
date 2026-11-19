#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "follow_mon_interact_work.h"

void ov02_0224F76C(s8 arg0, FollowMonInteractWork *arg1);

void ov02_0224F76C(s8 arg0, FollowMonInteractWork *arg1) {
    void *temp_r0;

    if ((arg0 <= 0) || (arg0 > 0x1ED)) {
        GF_AssertFail();
        return;
    }
    temp_r0 = AllocAtEndAndReadWholeNarcMemberByIdPair(NARC_a_2_3_1, 0, HEAP_ID_FIELD2);
    arg1->unk_0A = (u8) * (u8 *)(temp_r0 + (arg0 - 1));
    Heap_Free(temp_r0);
}
