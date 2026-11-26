#include "global.h"

#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"

BOOL ov96_021E8988(Ov96Handlers *handlers, s32 cmd, void *data, s32 size) {
    s32 expected;
    void *copy;
    s32 i;
    Ov96Msg *msg;
    s32 idx;

    msg = NULL;
    for (i = 0; i < 8; i++) {
        if (handlers->msgs[i].active == 0) {
            msg = &handlers->msgs[i];
            break;
        }
    }
    if (msg == NULL) {
        GF_AssertFail();
    }
    idx = cmd - 0x16;
    expected = handlers->cmdTable[idx].unk_04();
    if (expected != 0xffff && expected != size) {
        GF_AssertFail();
    }
    if (handlers->cmdTable[idx].unk_08 != NULL) {
        copy = handlers->cmdTable[idx].unk_08(0, handlers->userData, size);
        if (size > 0) {
            memcpy(copy, data, size);
        }
        msg->ptr = copy;
    } else {
        msg->ptr = data;
    }
    msg->cmd = cmd - 0x16;
    msg->size = size;
    msg->active = 1;
    return TRUE;
}
