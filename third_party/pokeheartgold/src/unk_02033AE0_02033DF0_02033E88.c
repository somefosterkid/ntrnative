#include "global.h"
#include "comm_queue_man.h"

struct UnkStruct_020322D8_t *sub_02033E88(CommQueueMan *arg0) {
    struct UnkStruct_020322D8_t *v0;
    struct UnkStruct_020322D8_t *v1;
    struct UnkStruct_020322D8_t *v2;

    v1 = arg0->unk_10;
    if (v1 != NULL) {
        return v1;
    }
    v0 = arg0->unk_00.unk_00;
    if (v0 != NULL) {
        return v0;
    }
    v2 = arg0->unk_08.unk_00;
    if (v2 == NULL) {
        v2 = NULL;
    }
    return v2;
}
