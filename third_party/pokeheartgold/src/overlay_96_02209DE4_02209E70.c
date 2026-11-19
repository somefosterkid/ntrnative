#include "global.h"

#include "error_handling.h"
#include "message_format.h"
#include "sprite.h"
#include "overlay_96_ov96_0220_data.h"

void ov96_02209F40(Window *, MessageFormat *, MsgData *, u32, enum HeapID);

u8 ov96_0220B7B4(void *);

void ov96_02209E70(Ov96_0220Data *arg0);

void ov96_02209E70(Ov96_0220Data *arg0) {
    s32 v1;
    u8 v0;
    u8 v2;
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg0->unk_4C == NULL) {
        GF_AssertFail();
    }
    v1 = arg0->unk_260 / 30;

    if (v1 < 0) {
        v1 = 0;
    }
    v0 = (u8)(v1 % 100);
    Sprite_SetAnimCtrlSeq(arg0->unk_38[0], ((u8)(((s32)v0) / 10)) + 1);
    Sprite_SetAnimCtrlSeq(arg0->unk_38[1], ((u8)(((s32)v0) % 10)) + 1);
    v2 = (ov96_0220B7B4(arg0->unk_4C) + 1) % 3;
    BufferString(arg0->msgFormat, 0U, arg0->names[v2], 2, 1, 2);
    ov96_02209F40(&arg0->windows[3], arg0->msgFormat, arg0->msgData, 0x9A, arg0->heapId);
}
