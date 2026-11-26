#include "global.h"

/* HDR-OVERLAY {"MailMsg_SetMsgBankAndNum": {"file": "include/mail_message.h", "new": "void MailMsg_SetMsgBankAndNum(MailMessage *mailMessage, s16 msg_bank, u16 msg_no);", "old": "void MailMsg_SetMsgBankAndNum(MailMessage *mailMessage, u16 msg_bank, u16 msg_no);"}} */
#define MailMsg_SetMsgBankAndNum MailMsg_SetMsgBankAndNum__hdr

/* HDR-OVERLAY defines done */
#include "mail_message.h"

/* HDR-OVERLAY end */
#undef MailMsg_SetMsgBankAndNum

#if defined(SDK_PORT) || defined(NONMATCHING)
void MailMsg_SetMsgBankAndNum(MailMessage *mailMessage, u16 msg_bank, u16 msg_no);
#else
void MailMsg_SetMsgBankAndNum(MailMessage *mailMessage, s16 msg_bank, u16 msg_no);
#endif

void ov102_021E79DC(void *arg0, MailMessage *arg1, s32 arg2);

void ov102_021E79DC(void *arg0, MailMessage *arg1, s32 arg2) {
    *(s8 *)((u8 *)arg0 + 2) = (s8)(arg2 % (s8) * (s8 *)((u8 *)arg0 + 3));
    *(s16 *)arg0 = (s16)(arg2 / (s8) * (s8 *)((u8 *)arg0 + 3));
    MailMsg_SetMsgBankAndNum(arg1, *(s16 *)arg0, (u16) * (s8 *)((u8 *)arg0 + 2));
}
