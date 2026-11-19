#include "global.h"

#include "overlay_32.h"
#include "overlay_32_name_picker_data.h"

u8 ov32_0225DF80(NamePickerData *data) {
    if (ov32_0225E048(data) == 0) {
        return data->nextState;
    }
    return 1U;
}

s32 ov32_0225DF9C(NamePickerData *data, s32 dir) {
    u16 v0;
    u16 v1;

    data->page = (s8)(data->page + dir);
    if ((s32)data->page < 0) {
        data->page = 3;
    }
    if ((s32)data->page > 3) {
        data->page = 0;
    }
    if (dir > 0) {
        v0 = 3;
        v1 = 6;
    } else {
        v0 = 2;
        v1 = 4;
    }
    return ov32_0225E0A8(data, v0, (u8)(v1 + 1), v1, 2);
}
