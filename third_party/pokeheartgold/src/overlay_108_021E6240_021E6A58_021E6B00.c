#include "global.h"

#include "yes_no_prompt.h"
#include "overlay_108_safari_area_customizer.h"

struct BgConfig;

struct YesNoPrompt;

struct YesNoPromptTemplate;

void ov108_021E79A8(SafariAreaCustomizer *arg0, u8 arg1, s32 arg2, s32 arg3);

void ov108_021E7BB4(SafariAreaCustomizer *arg0, u8 arg1, u8 arg2);

void ov108_021E767C(SafariAreaCustomizer *param_1, s8 param_2);

extern void ov108_021E7700(void *data, int a1, int a2, int a3);

void ov108_021E6B00(SafariAreaCustomizer *data);

void ov108_021E6B00(SafariAreaCustomizer *data) {
    YesNoPromptTemplate tmpl;

    MI_CpuFill8(&tmpl, 0, sizeof(tmpl));
    tmpl.bgConfig = (data->bgConfig);
    tmpl.bgId = 0;
    tmpl.tileStart = 0x379;
    tmpl.plttSlot = 0xD;
    tmpl.x = 0x1A;
    tmpl.y = 0x10;
    tmpl.ignoreTouchFlag = (data->inTouchMode);
    tmpl.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate((data->yesNoPrompt), &tmpl);
    ov108_021E79A8(data, 1, (data->cursor), 1);
    ov108_021E7BB4(data, (data->areaIdx), (u8)((data->cursor) + (data->page) * 6));
    ov108_021E767C(data, 1);
    ov108_021E7700(data, 1, 3, 1);
}
