#include "global.h"

#include "map_header.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "trainer_memo.h"

void ov80_0222A334(SaveData *arg0, Pokemon *arg1);

void ov80_0222A334(SaveData *saveData, Pokemon *mon) {
    u32 ptr;
    MsgData *msgData;
    PlayerProfile *profile;
    String *string;

    profile = Save_PlayerData_GetProfile(saveData);
    ptr = GetMonData(mon, 7, NULL);
    sub_0207217C(mon, Save_PlayerData_GetProfile(saveData), 4U, 0U, 0U, HEAP_ID_FIELD2);
    MonSetTrainerMemo(mon, profile, 0, (s32)MapHeader_GetMapSec(0x113U), HEAP_ID_FIELD2);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xC1, HEAP_ID_FIELD2);
    string = NewString_ReadMsgData(msgData, 0);
    SetMonData(mon, 0x91, string);
    SetMonData(mon, 7, &ptr);
    String_Delete(string);
    DestroyMsgData(msgData);
}
