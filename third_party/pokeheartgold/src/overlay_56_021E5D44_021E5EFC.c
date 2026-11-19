#include "global.h"

#include "bg_window.h"
#include "overlay_55.h"
#include "overlay_56.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_56_mail_viewer_app.h"

s32 ov56_021E5CE0(void *);

extern TouchscreenHitbox ov56_021E6E20;

static s32 ov56_021E5EFC(MailViewerApp *arg0) {
    u16 src;
    s32 v0;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(&ov56_021E6E20);
    if (v0 == -1) {
        return 0;
    }
    src = 1;
    if (DoesPixelAtScreenXYMatchPtrVal(arg0->bgConfig, 2U, gSystem.touchX, gSystem.touchY, &src) == 1) {
        return 0;
    }
    if (v0 == 3) {
        PlaySE(0x5DDU);
        *((s8 *)&arg0->mode) = 3;
        return 0;
    }
    if (v0 < 3) {
        arg0->args->mailMessageIdx = (u8)v0;
        arg0->args->result = arg0->args->mailMessageIdx;
        arg0->args->horizontalSelectionIndex = arg0->horizontalSelectionIndex;
        PlaySE(0x5DDU);
        return 1;
    }
    if (ov56_021E5CE0(arg0) != 0) {
        PlaySE(0x5DDU);
        *((s8 *)&arg0->mode) = 2;
        return 0;
    }
    arg0->args->result = 3;
    PlaySE(0x5E3U);
    arg0->args->mailMessageIdx = 0;
    arg0->args->horizontalSelectionIndex = 0;
    return 1;
}

s32 ov56_021E5DDC(void *);

s32 ov56_021E5FB4(MailViewerApp *arg0) {
    if (ov56_021E5D44(arg0) != 0) {
        return 0;
    }
    if (arg0->unkA == 0) {
        return ov56_021E5DDC(arg0);
    }
    return ov56_021E5EFC(arg0);
}
