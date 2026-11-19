#include "global.h"

#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "message_format.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208C7F8(PokemonSummary *arg0, s32 arg1, s32 strno, s32 arg3, s32 arg4) {
    s32 v1;
    s8 v0;

    v1 = 0xE0F00;
    v0 = gNatureStatMods[arg0->mon.nature][arg3];
    if ((s32)v0 < 0) {
        v1 = 0xE0800;
    } else if ((s32)v0 > 0) {
        v1 = 0xE0700;
    }
    ReadMsgDataIntoString(arg0->msgData, strno, arg0->buffer);
    sub_0208C778(arg0, &arg0->windows[arg1], v1, arg4);
}

void sub_0208C850(PokemonSummary *arg0, s32 arg1, s32 strno, s32 arg3, s32 arg4) {
    ReadMsgDataIntoString(arg0->msgData, strno, arg0->buffer);
    sub_0208C778(arg0, &arg0->windows[arg1], arg3, arg4);
}

void sub_0208C87C(PokemonSummary *param_1, s32 param_2, s32 param_3, u8 param_4, u8 param_5) {
    String *src;

    src = NewString_ReadMsgData(param_1->msgData, param_2);
    BufferIntegerAsString(param_1->msgFormat, 0, param_3, param_4, (PrintingMode)((u32)param_5), 1);
    StringExpandPlaceholders(param_1->msgFormat, param_1->buffer, src);
    String_Delete(src);
    return;
}
