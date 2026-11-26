#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "text.h"
#include "unk_0200A090.h"
#include "render_window.h"
#include "gf_gfx_planes.h"
#include "yes_no_prompt.h"
#include "overlay_48_unk_struct_ov69_0225ddc8.h"

struct YesNoPrompt;

void YesNoPrompt_InitFromTemplate(YesNoPrompt *menu, const YesNoPromptTemplate *params);


extern const YesNoPromptTemplate ov48_0225B1C4;

u32 ov48_02259F8C(UnkStruct_ov69_0225DDC8 *param0);

void YesNoPrompt_Reset(YesNoPrompt *menu);
void ov48_02259FEC(UnkStruct_ov69_0225DDC8 *param0);

u32 ov48_02259F8C(UnkStruct_ov69_0225DDC8 *param0) {
    u32 v0;

    switch (param0->unk_00) {
    case 0:
        v0 = 0;

        if (TextPrinterCheckActive(param0->unk_04) == 0) {
            YesNoPromptTemplate v1;

            v1 = ov48_0225B1C4;
            v1.bgConfig = GetWindowBgConfig(&param0->unk_10);
            YesNoPrompt_InitFromTemplate(param0->unk_20, &v1);
            param0->unk_00++;
        }
        break;
    case 1:
        v0 = YesNoPrompt_HandleInput(param0->unk_20);
        break;
    }

    return v0;
}

void ov48_02259FEC(UnkStruct_ov69_0225DDC8 *param0) {
    YesNoPrompt_Reset(param0->unk_20);
    ClearFrameAndWindow2(&param0->unk_10, 1);
    ClearWindowTilemapAndScheduleTransfer(&param0->unk_10);
}
