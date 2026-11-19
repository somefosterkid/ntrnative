#include "global.h"

#include "pokemon_types_def.h"

/* HDR-OVERLAY {"MailMsg_SetMsgBankAndNum": {"file": "include/mail_message.h", "new": "void MailMsg_SetMsgBankAndNum(MailMessage *mailMessage, s16 msg_bank, u16 msg_no);", "old": "void MailMsg_SetMsgBankAndNum(MailMessage *mailMessage, u16 msg_bank, u16 msg_no);"}} */
#define MailMsg_SetMsgBankAndNum MailMsg_SetMsgBankAndNum__hdr

/* HDR-OVERLAY defines done */
#include "mail_message.h"

/* HDR-OVERLAY end */
#undef MailMsg_SetMsgBankAndNum

void MailMsg_SetMsgBankAndNum(MailMessage *mailMessage, s16 msg_bank, u16 msg_no);

void ov102_021E7968(void *arg0, MailMessage *arg1);

void ov102_021E7968(void *arg0, MailMessage *arg1) {
    *(s8 *)((u8 *)arg0 + 2) = (s8)(*(s8 *)((u8 *)arg0 + 2) + 1);
    if ((s32) * (s8 *)((u8 *)arg0 + 2) >= (s32) * (s8 *)((u8 *)arg0 + 3)) {
        *(s8 *)((u8 *)arg0 + 2) = 0;
        *(s16 *)arg0 = (s16)(*(s16 *)arg0 + 1);
        if ((s32) * (s16 *)arg0 >= 5) {
            *(s16 *)arg0 = 0;
        }
    }
    MailMsg_SetMsgBankAndNum(arg1, (s16) * (s16 *)arg0, (u16) * (s8 *)((u8 *)arg0 + 2));
}

void ov102_021E79A0(void *arg0, MailMessage *arg1);

void ov102_021E79A0(void *arg0, MailMessage *arg1) {
    *(s8 *)((u8 *)arg0 + 2) = (s8)(*(s8 *)((u8 *)arg0 + 2) - 1);
    if ((s32) * (s8 *)((u8 *)arg0 + 2) < 0) {
        *(s16 *)arg0 = (s16)(*(s16 *)arg0 - 1);
        if ((s32) * (s16 *)arg0 < 0) {
            *(s16 *)arg0 = 4;
        }
        *(s8 *)((u8 *)arg0 + 2) = (s8)(*(s8 *)((u8 *)arg0 + 3) - 1);
    }
    MailMsg_SetMsgBankAndNum(arg1, (s16) * (s16 *)arg0, (u16) * (s8 *)((u8 *)arg0 + 2));
}
