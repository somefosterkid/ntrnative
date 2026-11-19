#include "global.h"

#include "item.h"
#include "mail_misc.h"
#include "overlay_manager.h"
#include "overlay_103_mail_box_work.h"

extern OverlayManagerTemplate ov103_021EEC58;

s32 ov103_021EDD54(MailBoxWork *arg0);

s32 ov103_021EDD54(MailBoxWork *arg0) {
    UseMailArgs *temp_r0;

    temp_r0 = sub_02090E68(arg0->args->saveData, 2U, arg0->partyPos, ItemToMailId(arg0->item), HEAP_ID_156);
    temp_r0->menuInputStatePtr = arg0->args->menuInputStatePtr;
    arg0->childParam = temp_r0;
    arg0->childMan = OverlayManager_New(&ov103_021EEC58, arg0->childParam, HEAP_ID_156);
    return 0;
}
