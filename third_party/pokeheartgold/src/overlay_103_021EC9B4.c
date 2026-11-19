#include "global.h"

#include "overlay_103_mail_box_view.h"
#include "overlay_103_mail_box_work.h"

void ov103_021EC9B4(void *arg);

void SpriteSystem_TransferOam(void);

void ov103_021EC9B4(void *arg) {
    MailBoxWork *work = arg;
    DoScheduledBgGpuUpdates(work->view->bgConfig);
    SpriteSystem_TransferOam();
    OS_SetIrqCheckFlag(0x1);
}
