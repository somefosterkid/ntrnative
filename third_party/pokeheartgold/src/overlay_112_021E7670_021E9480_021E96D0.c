#include "global.h"
#include "bg_window.h"

#include "msgdata.h"
#include "pm_string.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E7CA4(Ov112PokewalkerApp *, enum GFBgLayer, s32);

u8 ov112_021E9FA4(void *, s32, String *, s32, s32);

void ov112_021E96D0(Ov112PokewalkerApp *arg0, s32 arg1) {
    String *string;
    String *v0;

    string = NewString_ReadMsgData(arg0->msgData, 0x96);
    v0 = NewString_ReadMsgData(arg0->msgData, 0x97);
    ov112_021E7CA4(arg0, GF_BG_LYR_MAIN_2, 0xD);
    ov112_021EA51C(arg0, 0);
    ov112_021E9FA4(arg0, 3, string, 4, 0xF0100);
    ov112_021E9FA4(arg0, 4, v0, 4, 0xF0100);
    ov112_021EA51C(arg0, arg1);
    arg0->unk_1EC50 = arg1;
    arg0->unk_1F2E2 = 0;
    String_Delete(v0);
    String_Delete(string);
}
