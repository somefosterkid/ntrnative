#include "global.h"

#include "overlay_103_mail_box_work.h"
#include "overlay_manager.h"

u32 ov103_021ED568(MailBoxWork *param_1);

u32 ov103_021ED568(MailBoxWork *param_1) {
    BOOL BVar1;
    u32 uVar2;

    BVar1 = OverlayManager_Run(param_1->childMan);
    if (BVar1 == 0) {
        return 7;
    }
    OverlayManager_Delete(param_1->childMan);
    uVar2 = ov103_021EEC78[param_1->childProc].end(param_1);
    param_1->nextState = uVar2;
    return ov103_021EEC78[param_1->childProc].nextState;
}
