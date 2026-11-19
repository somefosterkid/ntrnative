#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "pm_string.h"

void ov05_0221E624(UnkStruct_ov10_0221FB28 *, MsgData *, MessagePrinter *, MessageFormat *, String *, u32);

void ov05_0221E714(UnkStruct_ov10_0221FB28 *, MsgData *, MessagePrinter *, MessageFormat *, String *, u32);

void ov05_0221E74C(UnkStruct_ov10_0221FB28 *, MsgData *, MessagePrinter *, MessageFormat *, String *, u32);

void ov05_0221E7B8(UnkStruct_ov10_0221FB28 *, u32);

void ov05_0221E8A8(UnkStruct_ov10_0221FB28 *arg0, MsgData *arg1, MessagePrinter *arg2, MessageFormat *arg3, String *arg4, u32 arg5);

void ov05_0221E8A8(UnkStruct_ov10_0221FB28 *arg0, MsgData *arg1, MessagePrinter *arg2, MessageFormat *arg3, String *arg4, u32 arg5) {
    MessageFormat *v0;
    MessagePrinter *v2;
    MsgData *v1;
    Window *sum;

    v1 = arg1;
    v2 = arg2;
    v0 = arg3;
    sum = &arg0->unk_10[arg5 * 4];
    FillWindowPixelBuffer(&sum[0], 0U);
    FillWindowPixelBuffer(&sum[1], 0U);
    FillWindowPixelBuffer(&sum[2], 0U);
    FillWindowPixelBuffer(&sum[3], 0U);
    ov05_0221E624(arg0, arg1, arg2, arg3, arg4, arg5);
    ov05_0221E714(arg0, arg1, arg2, arg3, arg4, arg5);
    ov05_0221E74C(arg0, arg1, arg2, arg3, arg4, arg5);
    ov05_0221E7B8(arg0, arg5);
    ScheduleWindowCopyToVram(&sum[0]);
    ScheduleWindowCopyToVram(&sum[1]);
    ScheduleWindowCopyToVram(&sum[2]);
    ScheduleWindowCopyToVram(&sum[3]);
}
