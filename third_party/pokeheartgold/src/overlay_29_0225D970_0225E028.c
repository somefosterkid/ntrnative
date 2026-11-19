#include "global.h"

#include "overlay_29.h"
#include "overlay_29_number_entry_data.h"

void ov29_0225E028(NumberEntryData *data, u8 arg1, u8 arg2, u8 arg3, u16 arg4) {
    data->blink.isBg = 0;
    data->blink.timer = 0;
    data->blink.phase = 0;
    data->blink.target = arg1;
    data->blink.seqA = arg2;
    data->blink.seqB = arg3;
    data->nextState = arg4;
    data->state = 1;
}

void ov29_0225E078(NumberEntryData *data, u8 arg1, u8 arg2, s16 arg3) {
    data->blink.isBg = 1;
    data->blink.timer = 0;
    data->blink.phase = 0;
    data->blink.target = 5;
    data->blink.seqA = 1;
    data->blink.seqB = 0;
    data->blink.x = arg1;
    data->blink.y = arg2;
    data->blink.width = 6;
    data->blink.height = 6;
    data->nextState = arg3;
    data->state = 1;
}
