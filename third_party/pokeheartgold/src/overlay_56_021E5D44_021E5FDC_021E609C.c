#include "global.h"

#include "bg_window.h"
#include "overlay_56.h"
#include "overlay_56_mail_viewer_app.h"
#include "palette.h"
#include "yes_no_prompt.h"

void ov56_021E609C(MailViewerApp *mailViewerApp) {
    YesNoPromptTemplate YStack_20;

    MI_CpuFill8(&YStack_20, 0, sizeof(YesNoPromptTemplate));
    YStack_20.bgConfig = mailViewerApp->bgConfig;
    YStack_20.bgId = 0;
    YStack_20.tileStart = 0x1bb;
    YStack_20.plttSlot = 3;
    YStack_20.x = 0x18;
    YStack_20.y = 0xa;
    YStack_20.ignoreTouchFlag = mailViewerApp->unkA;
    YStack_20.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplateWithPalette(mailViewerApp->yesNoPrompt, &YStack_20, mailViewerApp->paletteData);
    return;
}
