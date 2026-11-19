#include "global.h"
#include "bg_window.h"
#include "overlay_56.h"
#include "palette.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "overlay_56_mail_viewer_app.h"
#include "msgdata.h"

struct Menu;

struct MsgData;

typedef struct Menu Menu;

void ov56_021E63FC(SysTask *task, void *param);

void ov56_021E63C0(void *data) {
    MailViewerApp *mailViewerApp = (MailViewerApp *)data;

    if (mailViewerApp->paletteData != NULL) {
        PaletteData_PushTransparentBuffers(mailViewerApp->paletteData);
    }

    if (mailViewerApp->spriteSys != NULL) {
        SpriteSystem_TransferOam();
    }

    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(mailViewerApp->bgConfig);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void ov56_021E63FC(SysTask *task, void *param) {
    MailViewerApp *arg1 = param;
    u8 v0;

    if (arg1->paletteData == NULL) {
        SysTask_Destroy(task);
        return;
    }
    if ((arg1->pauseGlowEffect == 0) && (arg1->unkA != 1)) {
        if (arg1->prevBlendIndex != arg1->blendIndex) {
            ov56_021E5D08(arg1);
            arg1->prevBlendIndex = (u8) arg1->blendIndex;
        }
        PaletteData_BlendPalette(arg1->paletteData, PLTTBUF_MAIN_BG, (u16)(arg1->blendIndex + 0x22), 1U, (u8)(s32) arg1->blendFraction, 0x7FFFU);
        v0 = arg1->blendFraction;
        if (arg1->blendDirection != 0) {
            arg1->blendFraction = (u8)(v0 - 1);
            if (v0 == 1) {
                arg1->blendDirection = (u8)(1 ^ arg1->blendDirection);
            }
        } else {
            arg1->blendFraction = (u8)(v0 + 1);
            if (v0 == 0xC) {
                arg1->blendDirection = (u8)(1 ^ arg1->blendDirection);
            }
        }
    }
}
