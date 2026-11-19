#include "global.h"

#include "bg_window.h"
#include "menu_input_state.h"
#include "unk_02035900.h"
#include "yes_no_prompt.h"

void ov73_021E71E4(u32 *param_1, s32 param_2, s8 param_3);

void ov73_021E6830(u32 *param_1);

void ov73_021E6830(u32 *param_1) {
    u16 uVar1;
    MenuInputState MVar2;

#if defined(SDK_PORT) || defined(NONMATCHING)
    if (*(u8 *)((u8 *)param_1 + 0x4a15) != 0) {
#else
    if (*(u8 *)((u32)param_1 + 0x4a15) != 0) {
#endif
        MVar2 = YesNoPrompt_IsInTouchMode((YesNoPrompt *)param_1[0xba]);
        param_1[0xc5] = MVar2;
        YesNoPrompt_Reset((YesNoPrompt *)param_1[0xba]);
        *(u8 *)((u8 *)param_1 + 0x4a15) = 0;
        uVar1 = sub_0203769C();
        if (uVar1 == 0) {
            ov73_021E71E4(param_1, 0, 0);
        }
        param_1[0x128e] = 0xffffffff;
        ov73_021E71E4(param_1, 1, 0);
        ScheduleBgTilemapBufferTransfer((BgConfig *)*param_1, 1);
    }
    return;
}
