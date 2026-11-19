#include "global.h"

#include "constants/heap.h"

#include "overlay_86_frontier_records_app.h"
#include "screen_fade.h"

s32 ov86_021E71FC(void *arg0);

void ov86_021E720C(void *arg0);

s32 ov86_021E60B8(void *);

s32 ov86_021E5B38(FrontierRecordsApp *arg0);

s32 ov86_021E5B38(FrontierRecordsApp *arg0) {
    u8 temp_r1;
    temp_r1 = arg0->state;
    switch (temp_r1) {
    case 0:
        if (ov86_021E71FC(arg0) == 1) {
            ov86_021E720C(arg0);
        } else {
            ov86_021E60B8(arg0);
        }
        arg0->state = (u8)((arg0->state) + 1);
        break;

    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 3, HEAP_ID_121);
        arg0->state = (u8)(arg0->state + 1);
        break;

    case 2:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }
    return 0;
}
