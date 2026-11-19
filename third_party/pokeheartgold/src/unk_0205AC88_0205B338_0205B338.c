#include "global.h"

#include "mail_message.h"
#include "pm_string.h"

void sub_0205B338(void *arg0);

void sub_0205B338(void *arg0) {
    *(String **)arg0 = String_New(8U, HEAP_ID_87);
    *(s32 *)((u8 *)arg0 + 4) = 0;
    *(s32 *)((u8 *)arg0 + 8) = 0;
    MailMsg_Init_WithBank(arg0 + 0x14, 0U);
    *(s32 *)((u8 *)arg0 + 0x10) = 0;
    *(s32 *)((u8 *)arg0 + 0xC) = 0;
}
