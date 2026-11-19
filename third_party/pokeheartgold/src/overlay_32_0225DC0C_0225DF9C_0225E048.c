#include "global.h"

#include "overlay_32.h"
#include "overlay_32_name_picker_data.h"

s32 ov32_0225E048(NamePickerData *data) {
    NamePickerBlink *blink = &data->blink;

    switch (blink->phase) {
    case 0:
        ov32_0225DFE8(data, blink->seqA);
        blink->phase++;
        break;
    case 1:
        blink->timer++;
        if (blink->timer == 4) {
            ov32_0225DFE8(data, blink->seqB);
            blink->timer = 0U;
            blink->phase++;
        }
        break;
    case 2:
        blink->timer++;
        if (blink->timer == 2) {
            return 0;
        }
        break;
    }
    return 1;
}
