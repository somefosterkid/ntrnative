#include "global.h"
#include "comm_queue_man.h"

s32 sub_02033E88(void *arg0);

s32 sub_02033E88(void *arg0) {
    s32 v0;
    s32 v1;
    s32 v2;

    v1 = *((s32 *)&((CommQueueMan *)arg0)->unk_10);
    if (v1 != 0) {
        return v1;
    }
    v0 = *(s32 *)arg0;
    if (v0 != 0) {
        return v0;
    }
    v2 = *((s32 *)&((CommQueueMan *)arg0)->unk_08.unk_00);
    if (v2 == 0) {
        v2 = 0;
    }
    return v2;
}
