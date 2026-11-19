#include <dwc.h>

#include "global.h"

#include "overlay_72_wfc_connect.h"

s32 ov72_0223A41C(void *, s32, s32);

s32 ov72_0223A588(NintendoWfcConnect *wfc);

s32 sub_0202D720(void *, u8, u8, RTCDate *);

s32 ov72_02239784(NintendoWfcConnect *arg0);

s32 ov72_02239784(NintendoWfcConnect *arg0) {
    RTCTime sp10;
    RTCDate sp0;
    s32 temp_r0;

    temp_r0 = ov72_0223A588(arg0);
    if (temp_r0 != 0) {
        if (temp_r0 > 0) {
            DWC_GetDateTime(&sp0, &sp10);
            arg0->room = temp_r0;
            if (sub_0202D720(arg0->args->downloadData, (u8) arg0->rank, (u8) arg0->room, &sp0) != 0) {
                arg0->state = NINTENDO_WFC_STATE_PRINT_ROOM_UNAVAILABLE;
            } else {
                arg0->state = NINTENDO_WFC_STATE_PRINT_RECEIVING_TRAINERS;
            }
        } else {
            ov72_0223A41C(arg0, 0, 0);
            arg0->state = NINTENDO_WFC_STATE_ASK_CANCEL_CHALLENGE;
        }
    }
    return 3;
}
