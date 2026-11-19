#include "global.h"

#include "overlay_32.h"
#include "overlay_32_name_picker_data.h"

s32 ov32_0225E0A8(NamePickerData *data, u8 target, u8 seqA, u8 seqB, u8 nextState) {
    data->blink.isBg = 0;
    data->blink.timer = 0;
    data->blink.phase = 0;
    data->blink.target = target;
    data->blink.seqA = seqA;
    data->blink.seqB = seqB;
    data->nextState = nextState;
    return 1;
}
