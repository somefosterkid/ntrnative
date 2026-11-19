#include "global.h"

#include "menu_input_state.h"
#include "yes_no_prompt.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

YesNoResponse ov41_02247B7C(u8 *param_1);

void ov41_02247BB8(void *param_1, s32 param_2, u8 param_3, u8 param_4, u8 param_5, u8 param_6);

YesNoResponse ov41_02247B7C(u8 *param_1) {
    YesNoResponse YVar1;
    MenuInputState MVar2;

    YVar1 = YesNoPrompt_HandleInput(*((YesNoPrompt **)&((UnkStruct_ov22_0225A0E4 *)param_1)->unk_6B8));
    switch (YVar1) {
    case YESNORESPONSE_WAIT:
        return YESNORESPONSE_NO_TOUCH;
    case YESNORESPONSE_YES:
        YVar1 = (YesNoResponse)(8);
        break;
    case YESNORESPONSE_NO:
        YVar1 = (YesNoResponse)(9);
        break;
    }
    MVar2 = YesNoPrompt_IsInTouchMode(*((YesNoPrompt **)&((UnkStruct_ov22_0225A0E4 *)param_1)->unk_6B8));
    *(MenuInputState *)(param_1 + 0x6ec) = MVar2;
    return YVar1;
}

void ov41_02247BB8(void *param_1, s32 param_2, u8 param_3, u8 param_4, u8 param_5, u8 param_6) {
    u32 uVar1;
    MsgData *msgData;
    String *string;

    uVar1 = Options_GetFrame(*(Options **)(param_1 + 0x6dc));
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_7_OFFSET, HEAP_ID_14);
    AddWindowParameterized(*(BgConfig **)(param_1 + 0x40), *(Window **)(param_1 + 0x6bc), 3, param_3, param_4, param_5, param_6, 7, 0x5a);
    FillWindowPixelBuffer(*(Window **)(param_1 + 0x6bc), 0xf);
    LoadUserFrameGfx2(*(BgConfig **)(param_1 + 0x40), GF_BG_LYR_MAIN_3, 0x3c, 8, (u8)uVar1, HEAP_ID_14);
    DrawFrameAndWindow2(*(Window **)(param_1 + 0x6bc), 0, 0x3c, 8);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xd7, HEAP_ID_13);
    string = NewString_ReadMsgData(msgData, param_2);
    AddTextPrinterParameterizedWithColor(*(Window **)(param_1 + 0x6bc), FONT_MESSAGE, string, 0, 0, 0, 0x1020f, (PrinterCallback_t)0x0);
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(*(Window **)(param_1 + 0x6bc));
    return;
}
