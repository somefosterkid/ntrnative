#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"

s32 ov18_021F95FC(void *, String *, s32, s32, s32, s32, s32);

void ov18_021EE3AC(PokedexAppData *arg0, MsgData *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void ov18_021EE3AC(PokedexAppData *arg0, MsgData *msgData, s32 arg2, s32 strno, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    StringExpandPlaceholders(arg0->msgFormat, arg0->strBufFormatted, string);
    ov18_021F95FC(&arg0->windows[arg2], arg0->strBufFormatted, arg4, arg5, arg6, arg7, arg8);
    String_Delete(string);
}

s32 ov18_021EE35C(PokedexAppData *, s32 *, s32);

s32 ov18_021EE508(PokedexAppData *, s32, s32);

extern s32 ov18_021F9F3C;

void ov18_021EE3FC(PokedexAppData *pokedexApp) {
    s32 species;
    u32 a2;

    ov18_021EE35C(pokedexApp, &ov18_021F9F3C, 0x14);
    ov18_021EE508(pokedexApp, 0, 0);
    ov18_021EE508(pokedexApp, 1, 1);
    ov18_021EE5FC(pokedexApp);
    a2 = ov18_021F8824(pokedexApp);
    species = (s32)ov18_021F8838(pokedexApp);
    ov18_021EE6BC(pokedexApp, species, (s32)a2);
    ov18_021EE8B8(pokedexApp, (u32)species, (s32)a2);
}
