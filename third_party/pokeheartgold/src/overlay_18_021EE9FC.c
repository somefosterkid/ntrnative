#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "bg_window.h"
#include "dex_mon_measures.h"
#include "msgdata.h"
#include "pm_string.h"

void ov18_021F9648(void *, void *, u32, u32, u32, u32, u32, u32);

void ov18_021EE9FC(PokedexAppData *pokedexApp, s32 param_2);

s32 ov18_021F95FC(void *, String *, s32, s32, s32, s32, s32);
void ov18_021EEA84(PokedexAppData *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

void ov18_021EE9FC(PokedexAppData *pokedexApp, s32 param_2) {
    FillWindowPixelBuffer(&pokedexApp->windows[param_2], 0);
    ov18_021F9648(&pokedexApp->windows[param_2], pokedexApp->msgData, 10, 0x14, 0, 0, 0x20100, 2);
    ScheduleWindowCopyToVram(&pokedexApp->windows[param_2]);
    return;
}

void ov18_021EEA40(PokedexAppData *pokedexApp, s32 param_2);

void ov18_021EEA40(PokedexAppData *pokedexApp, s32 param_2) {
    FillWindowPixelBuffer(&pokedexApp->windows[param_2], 0);
    ov18_021F9648(&pokedexApp->windows[param_2], pokedexApp->msgData, 0xb, 0x14, 0, 0, 0x20100, 2);
    ScheduleWindowCopyToVram(&pokedexApp->windows[param_2]);
    return;
}

void ov18_021EEA84(PokedexAppData *arg0, s32 strno, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    MsgData *msgData;
    MsgData *v0;
    String *string;

    v0 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, GetDexHeightMsgBank(), HEAP_ID_POKEDEX_APP);
    msgData = v0;
    if (arg2 == 2) {
        string = NewString_ReadMsgData(v0, strno);
    } else {
        string = NewString_ReadMsgData(v0, 0);
    }
    ov18_021F95FC(&arg0->windows[arg3], string, arg4, arg5, 0, arg6, arg7);
    String_Delete(string);
    DestroyMsgData(msgData);
}
