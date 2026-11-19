#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_96_ov96_digit_work.h"

void ov96_02219BC4(Window *, MessageFormat *, MsgData *, u32, enum HeapID);

void ov96_02219EE0(Ov96DigitWork *work, s32 arg1);

void ov96_02219EE0(Ov96DigitWork *work, s32 arg1) {
    arg1 = (((s32)(arg1 + 1)) % 3) << 0x18;
    BufferString(work->msgFormat, 0U, work->names[((u32)arg1) >> 0x18], 2, 1, 2);
    ov96_02219BC4(&work->windows[1], work->msgFormat, work->msgData, 0x9A, work->heapId);
}
