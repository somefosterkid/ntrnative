#include "global.h"
#include "overlay_96_ov96_0220_data.h"

void ov96_02209E70(Ov96_0220Data *);

void ov96_02209F40(Window *, MessageFormat *, MsgData *, u32, enum HeapID);

void ov96_02209F14(Ov96_0220Data *arg0);

void ov96_02209F14(Ov96_0220Data *arg0) {
    ov96_02209F40(&arg0->windows[0], arg0->msgFormat, arg0->msgData, 0x12D, arg0->heapId);
    ov96_02209E70(arg0);
}
