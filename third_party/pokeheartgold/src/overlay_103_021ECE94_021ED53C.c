#include "global.h"
#include "overlay_103_mail_box_work.h"

s32 ov103_021ED144(MailBoxWork *arg0);

s32 ov103_021ED53C(MailBoxWork *arg0);

s32 ov103_021ED53C(MailBoxWork *arg0) {
    if (ov103_021ED144(arg0) == 0) {
        return arg0->nextState;
    }
    return 5;
}
