#include "global.h"

#include "unk_0201956C.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

s32 sub_02019688(UnkStruct_0201956C *, s32, s32, s32, s32);

void ov103_021ECD68(MailBoxWork *arg0);

void ov103_021ECD68(MailBoxWork *arg0) {
    s32 var_r4;
    s32 var_r4_2;
    u16 *temp_r6;
    u32 var_r4_3;
    arg0->view->frames = sub_0201956C(arg0->view->bgConfig, 2, 0xB, HEAP_ID_157);
    var_r4 = 0;
    do {
        sub_020195F4(arg0->view->frames, var_r4, 2, 0xE, 4);
        var_r4 += 1;
    } while (((u32)var_r4) < 0xAU);
    sub_020195F4(arg0->view->frames, 0xA, 1, 0xE, 4);
    sub_02019688(arg0->view->frames, 0, 0xFC, 3, 1);
    temp_r6 = sub_02019B08(arg0->view->frames, 0);
    var_r4_2 = 1;
    do {
        MI_CpuCopy8(temp_r6, sub_02019B08(arg0->view->frames, var_r4_2), 0x70U);
        var_r4_2 += 1;
    } while (((u32)var_r4_2) <= 0xAU);
    var_r4_3 = 0;
    do {
        sub_020196E8(arg0->view->frames, (s32)var_r4_3, (s8)((var_r4_3 % 2) * 16 + 1), (s8)((var_r4_3 / 2) * 4));
        var_r4_3 += 1;
    } while (var_r4_3 < 0xAU);
}
