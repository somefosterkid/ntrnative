#include "global.h"

#include "mail_message.h"

void ov102_021E7934(void *arg0, MailMessage *arg1);

void ov102_021E7934(void *arg0, MailMessage *mailMessage) {
    u8 *new_var;
    s32 v0;
    *((u16 *)arg0) = MailMsg_GetMsgBank(mailMessage);
    *((s8 *)(((u8 *)arg0) + 3)) = 0x14;
    *((s8 *)(((u8 *)arg0) + 2)) = (s8)MailMsg_GetMsgNo(mailMessage);
    v0 = 0;
    do {
        new_var = ((u8 *)arg0) + 4;
        v0 += 1;
        *((s8 *)new_var) = (s8)((*((s8 *)new_var)) + (*((s8 *)(((u8 *)arg0) + 3))));
    } while (v0 < 5);
}
