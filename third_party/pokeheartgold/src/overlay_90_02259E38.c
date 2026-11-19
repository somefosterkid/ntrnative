#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "msgdata.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"

struct MsgData;

struct MessageFormat;

void ov90_02259234(UnkStruct_ov114_0225D084 *param0, u32 param1, u32 param2);

void ov90_022592C4(UnkStruct_ov114_0225D084 *param0, u32 param1, Window *param2, u8 param3, u8 param4);

void ov90_02259E38(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, u32 param2, s16 param3, s16 param4, u32 param5);

void ov90_02259E8C(Window *arg0, BgConfig **arg1);

void ov90_02259E38(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, u32 param2, s16 param3, s16 param4, u32 param5) {
    ov90_02259234(param1, param2, param5);

    if (param5 == 5) {
        ov90_022592C4(param1, 14, &param0->unk_00, param3, param4);
    } else {
        ov90_022592C4(param1, 13, &param0->unk_00, param3, param4);
    }
}

void ov90_02259E8C(Window *window, BgConfig **arg1) {
    ScheduleWindowCopyToVram(window);
    ScheduleBgTilemapBufferTransfer(*arg1, 1U);
}
