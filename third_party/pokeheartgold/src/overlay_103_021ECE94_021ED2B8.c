#include "global.h"

#include "party.h"
#include "overlay_103_mail_box_work.h"

s32 ov103_021ED2B8(MailBoxWork *arg0);

s32 ov103_021ED2B8(MailBoxWork *arg0) {
    if (Party_GetCount(SaveArray_Party_Get(arg0->args->saveData)) > 0) {
        return 4;
    }
    return 2;
}
