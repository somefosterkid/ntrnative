#include "global.h"

#include "overlay_32.h"
#include "overlay_32_name_picker_data.h"

s32 ov32_0225E0FC(NamePickerData *data, u8 x, u8 y, u8 nextState) {
    data->blink.isBg = 1;
    data->blink.timer = 0;
    data->blink.phase = 0;
    data->blink.target = 5;
    data->blink.seqA = 1;
    data->blink.seqB = 0;
    data->blink.x = x;
    data->blink.y = y;
    data->blink.width = 12;
    data->blink.height = 4;
    data->nextState = nextState;
    return 1;
}
