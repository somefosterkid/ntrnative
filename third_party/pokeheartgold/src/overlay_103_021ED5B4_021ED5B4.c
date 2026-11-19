#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

s32 ov103_021EEA24(MailBoxWork *arg0, s32);

s32 ov103_021EDB18(MailBoxWork *, s32, s32);

s32 ov103_021EDA98(MailBoxWork *, s32, s32);

s32 ov103_021ED5B4(MailBoxWork *arg0, s32 arg1);

s32 ov103_021ED5B4(MailBoxWork *arg0, s32 arg1) {
    s32 temp_r0;
    s32 temp_r0_4;
    u16 temp_r0_2;
    u16 temp_r0_5;
    u16 temp_r2;
    u16 temp_r5;
    u32 temp_r0_3;

    temp_r0 = ov103_021EEA24(arg0, arg1);
    switch (temp_r0) {
    case 0:
        temp_r0_2 = arg0->page;
        if (temp_r0_2 != 0) {
            arg0->page = (u16)(temp_r0_2 - 1);
            PlaySE(0x5DCU);
            return ov103_021EDB18(arg0, 0, 0xB);
        }
        return 9;
    case 1:
        temp_r2 = arg0->page;
        if (temp_r2 != arg0->view->maxPage) {
            arg0->page = (u16)(temp_r2 + 1);
            PlaySE(0x5DCU);
            return ov103_021EDB18(arg0, 1, 0xB);
        }
        return 9;
    default:
        temp_r0_3 = GridInputHandler_HandleInput_NoHold(arg0->view->gridInput);
        switch (temp_r0_3) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            if (arg0->view->pageMails[temp_r0_3 + arg0->page * 10] == 0xFF) {
                break;
            }
            arg0->cursor = (u8)temp_r0_3;
            arg0->mailIndex = arg0->view->pageMails[temp_r0_3 + arg0->page * 10];
            PlaySE(0x5DDU);
            return ov103_021EDA98(arg0, temp_r0_3, 12);
        case -2:
        case 10:
            PlaySE(0x5DDU);
            return ov103_021EDB18(arg0, 3, 0xA);
        case -1:
            temp_r0_4 = GridInputHandler_GetNextInput(arg0->view->gridInput);
            if ((0x10 & gSystem.newAndRepeatedKeys) && ((temp_r0_4 == 1) || (temp_r0_4 == 3) || (temp_r0_4 == 5) || (temp_r0_4 == 7) || (temp_r0_4 == 9))) {
                temp_r5 = arg0->page;
                if (temp_r5 != arg0->view->maxPage) {
                    arg0->page = (u16)(temp_r5 + 1);
                    PlaySE(0x5DCU);
                    return ov103_021EDB18(arg0, 1, 0xB);
                }
            }
            if ((gSystem.newAndRepeatedKeys & 0x20) && ((temp_r0_4 == 0) || (temp_r0_4 == 2) || (temp_r0_4 == 4) || (temp_r0_4 == 6) || (temp_r0_4 == 8))) {
                temp_r0_5 = arg0->page;
                if (temp_r0_5 != 0) {
                    arg0->page = (u16)(temp_r0_5 - 1);
                    PlaySE(0x5DCU);
                    return ov103_021EDB18(arg0, 0, 0xB);
                }
            }
            break;
        case -3:
            PlaySE(0x5DCU);
            break;
        case -4:
        default:
            break;
        }
        break;
    }
    return 9;
}
