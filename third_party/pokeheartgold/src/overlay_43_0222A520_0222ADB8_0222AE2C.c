#include "global.h"

#include "brightness.h"
#include "menu_input_state.h"
#include "yes_no_prompt.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_palpad.h"

enum YesNoResponse ov43_0222AE2C(enum MenuInputState *arg0, PalPadGfx *arg1);

enum YesNoResponse ov43_0222AE2C(enum MenuInputState *arg0, PalPadGfx *arg1) {
    enum YesNoResponse v0;

    v0 = YesNoPrompt_HandleInput(arg1->yesNoPrompt);
    if ((u32)(v0 - 1) <= 1U) {
        *arg0 = YesNoPrompt_IsInTouchMode(arg1->yesNoPrompt);
        YesNoPrompt_Reset(arg1->yesNoPrompt);
        SetBlendBrightness((fx32)(0), (GXBlendPlaneMask)(0x3D), 1U);
    }
    return v0;
}

void ov43_0222B0A0(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222AE5C(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222AE5C(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    ov43_0222B0A0(menu, shared, gfx, heapID);
}
