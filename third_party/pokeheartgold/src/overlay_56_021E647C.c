#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "msgdata.h"
#include "overlay_56.h"
#include "overlay_56_mail_viewer_app.h"

struct Menu;

struct MsgData;

typedef struct Menu Menu;

void ov56_021E6514(MailViewerApp *mailViewerApp);

void ov56_021E6650(MailViewerApp *mailViewerApp);

void ov56_021E696C(MailViewerApp *mailViewerApp);

void ov56_021E6AA4(MailViewerApp *mailViewerApp);

void ov56_021E6BB4(MailViewerApp *mailViewerApp);

BOOL ov56_021E647C(MailViewerApp *mailViewerApp);

void ov56_021E660C(MailViewerApp *mailViewerApp);
void ov56_021E692C(MailViewerApp *mailViewerApp);
void ov56_021E6A7C(MailViewerApp *mailViewerApp);
s32 ov56_021E6D90(MailViewerApp *arg0);

BOOL ov56_021E647C(MailViewerApp *mailViewerApp) {
    switch (mailViewerApp->subState) {
    case 0:
        ov56_021E6514(mailViewerApp);
        break;
    case 1:
        ov56_021E6650(mailViewerApp);
        break;
    case 2:
        ov56_021E696C(mailViewerApp);
        ov56_021E6AA4(mailViewerApp);
        break;
    case 3:
        ov56_021E6BB4(mailViewerApp);
        mailViewerApp->subState = 0;
        return TRUE;
    }

    mailViewerApp->subState++;
    return FALSE;
}

s32 ov56_021E64C8(void *arg0) {
    MailViewerApp *mailViewerApp = arg0;

    ov56_021E6D90(mailViewerApp);
    if (&mailViewerApp->dummy != NULL) {
        DestroyMsgData(mailViewerApp->loader);
    }
    ov56_021E6A7C(mailViewerApp);
    ov56_021E692C(mailViewerApp);
    ov56_021E660C(mailViewerApp);
    return 1;
}
