#include "global.h"
#include "overlay_59_apricorn_app.h"

u8 ov59_02239EA8(Ov59ApricornApp *arg0);

u8 ov59_02239EA8(Ov59ApricornApp *arg0) {
    u8 temp_r1;

    temp_r1 = arg0->unk_49;
    switch (temp_r1) {
    case 0:
        return (u8) (arg0->unk_4A * 2);
    case 1:
        return (u8) ((arg0->unk_4A * 2) + 1);
    case 2:
        if (arg0->mode == 1) {
            return 7U;
        }
        return 8U;
    case 3:
        return 8U;
    default:
        return 0U;
    }
}
