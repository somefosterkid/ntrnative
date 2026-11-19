#include "global.h"
#include "unk_02005D10.h"

#include "overlay_96_app_02210C98.h"

void ov96_02213FB4(void *arg0);

void ov96_02213FF4(Ov96App_02210C98 *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov96_02213FB4(void *arg0) {
    u16 v0;

    v0 = *(u16 *)((u8 *)arg0 + 0x5C);
    switch (v0) {
    case 1:
        *(u16 *)((u8 *)arg0 + 0x5C) = 4U;
        return;
    case 4:
        *(u16 *)((u8 *)arg0 + 0x5C) = 2U;
        return;
    case 2:
        *(u16 *)((u8 *)arg0 + 0x5C) = 3U;
        return;
    case 3:
        *(u16 *)((u8 *)arg0 + 0x5C) = 1U;
    default:
        return;
    }
}

void ov96_02213FF4(Ov96App_02210C98 *param_1, s32 param_2, s32 param_3, s32 param_4) {
    if (param_4 < 0xc0) {
        param_1->unk_7B4[param_2] = 0;
        return;
    }
    if (param_4 < 0xd8) {
        if ((param_1->unk_7B4[param_2] & 0xFFu) == 0) {
            param_1->unk_7B4[param_2] = 1;
            if ((param_2 == 0) && (param_3 != 0)) {
                PlaySE(0x8ca);
                return;
            }
            PlaySE(0x8c9);
            return;
        }
    } else {
        param_1->unk_7B4[param_2] = 0;
    }
    return;
}
