#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "render_window.h"
#include "text.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_0223A4F4(GTSApplicationState *param_1, s32 param_2, u32 param_3, u32 param_4, u32 param_5, Pokemon *param_6);

void ov70_0223A4F4(GTSApplicationState *param_1, s32 param_2, u32 param_3, u32 param_4, u32 param_5, Pokemon *param_6) {
    u8 bVar1;
    BoxPokemon *boxmon;
    String *src;

    boxmon = Mon_GetBoxMon(param_6);
    BufferBoxMonSpeciesName(param_1->stringTemplate, 0, boxmon);
    src = NewString_ReadMsgData(param_1->gtsMessageLoader, param_2);
    StringExpandPlaceholders(param_1->stringTemplate, param_1->genericMessageBuffer, src);
    FillWindowPixelBuffer((Window *)(&param_1->bottomInstructionWindow), 0xf);
    DrawFrameAndWindow2((Window *)(&param_1->bottomInstructionWindow), 0, 1, 0xe);
    bVar1 = AddTextPrinterParameterized((Window *)(&param_1->bottomInstructionWindow), FONT_MESSAGE, param_1->genericMessageBuffer, 0, 0, param_3, (PrinterCallback_t)0x0);
    param_1->textPrinter = (u32)bVar1;
    String_Delete(src);
    return;
}
