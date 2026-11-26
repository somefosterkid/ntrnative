#include "global.h"

#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"

BOOL ov96_021E8914(Ov96Handlers *handlers, s32 cmd, void *data, s32 size) {
    void *src;
    CommCmdSizeFunc sizeFunc;
    Ov96Msg *msg;
    s32 i;

    msg = NULL;
    src = data;
    for (i = 0; i < 8; i++) {
        if (handlers->msgs[i].active == 0) {
            msg = &handlers->msgs[i];
            break;
        }
    }
    if (msg == NULL) {
        GF_AssertFail();
    }
    {
        s32 idx = cmd - 0x16;
        sizeFunc = handlers->cmdTable[idx].unk_04;
    }
    if (sizeFunc != NULL) {
        s32 got = sizeFunc();
        if (got != size) {
            GF_AssertFail();
        }
    }
    if (size > 0x26) {
        GF_AssertFail();
    }
    msg->cmd = cmd - 0x16;
    if (size > 0) {
        memcpy(msg->data, src, size);
    }
    msg->size = size;
    msg->active = 1;
    return TRUE;
}
