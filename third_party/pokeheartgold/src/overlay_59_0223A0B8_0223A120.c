#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "touchscreen.h"

s32 ov59_0223A07C(Ov59ApricornApp *, int);

extern TouchscreenHitbox ov59_0223C924;

s32 ov59_0223A120(Ov59ApricornApp *data, s32 *state);

s32 ov59_0223A120(Ov59ApricornApp *data, s32 *state) {
    int hit = TouchscreenHitbox_FindHitboxAtTouchNew(&ov59_0223C924);
    if (hit == -1 || hit == 7) {
        return 3;
    }
    *state = 1;
    data->menuInputState = MENU_INPUT_STATE_TOUCH;
    data->unk_4C = 0;
    data->unk_4B = data->unk_4C;
    data->unk_4A = 0;
    if (hit == 8) {
        data->unk_49 = 2;
    } else {
        data->unk_49 = hit % 2;
        data->unk_4A = hit / 2;
    }
    return ov59_0223A07C(data, hit);
}
