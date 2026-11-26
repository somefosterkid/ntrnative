#include "global.h"

#include "overlay_103_mail_box_work.h"

void ov103_021EDA40(void *, s32 arg1);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov103_021EDC68(MailBoxWork *, s8, s16, u32);
#else
void ov103_021EDC68(MailBoxWork *, u32, u16, u32);
#endif

void ov103_021EDE5C(MailBoxWork *param_1);

void ov103_021EDE5C(MailBoxWork *param_1) {
    ov103_021EDC68(param_1, 9, param_1->item, 0);
    u32 temp_r1;
    temp_r1 = 1;
    param_1->childProc = temp_r1;
    param_1->nextState = 6;
    ov103_021EDA40(param_1, temp_r1);
    return;
}
