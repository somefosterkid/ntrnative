#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_112_pokewalker_app.h"

u8 ov112_021E9FA4(void *, s32, String *, s32, s32);

void ov112_021E9F5C(Ov112PokewalkerApp *arg0, s32 arg1, s32 strno, s32 arg3) {
    String *string;

    string = NewString_ReadMsgData(arg0->msgData, strno);
    StringExpandPlaceholders(arg0->msgFormat, arg0->unk_1E454, string);
    ov112_021E9FA4(arg0, arg1, arg0->unk_1E454, 0, arg3);
    String_Delete(string);
}
