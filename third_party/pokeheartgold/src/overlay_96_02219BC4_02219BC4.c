#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"

void ov96_02219BDC(Window *param_1, MessageFormat *param_2, MsgData *param_3, u32 param_4, enum HeapID param_5, s16 param_6, s16 param_7);

void ov96_02219BC4(Window *param_1, MessageFormat *param_2, MsgData *param_3, u32 param_4, enum HeapID param_5);

void ov96_02219BC4(Window *param_1, MessageFormat *param_2, MsgData *param_3, u32 param_4, enum HeapID param_5) {
    ov96_02219BDC(param_1, param_2, param_3, param_4, param_5, 0, 0);
}
