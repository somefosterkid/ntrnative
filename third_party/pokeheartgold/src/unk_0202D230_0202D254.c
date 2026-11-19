#include "global.h"

#include "mail_message.h"
#include "pokemon_types_def.h"

extern MailMessageTemplate _020F682C;

void sub_0202D254(MailMessage *arg0);

void sub_0202D254(MailMessage *arg0) {
    MailMessage *var_r5;
    MailMessageTemplate *var_r4;
    s32 var_r6;

    var_r4 = &_020F682C;
    var_r5 = arg0;
    var_r6 = 0;
    do {
        MailMsg_Init_FromTemplate(var_r5, var_r4);
        var_r6 += 1;
        var_r4 = (void *)((u8 *)var_r4 + (8));
        var_r5 = (void *)((u8 *)var_r5 + (8));
    } while (var_r6 < 4);
}
