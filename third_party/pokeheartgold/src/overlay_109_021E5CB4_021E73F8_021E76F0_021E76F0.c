#include "global.h"

#include "yes_no_prompt.h"
#include "overlay_109_photo_album_app.h"

void ov109_021E76F0(PhotoAlbumApp *app) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0U, sizeof(YesNoPromptTemplate));
    template.bgConfig = app->bgConfig;
    template.bgId = 0;
    template.tileStart = 0x7A;
    template.plttSlot = 0xE;
    template.x = 0x19;
    template.y = 0x10;
    template.ignoreTouchFlag = (u8)(app->menuInputState);
    template.initialCursorPos = 1;
    YesNoPrompt_InitFromTemplate(app->yesNoPrompt, &template);
}
