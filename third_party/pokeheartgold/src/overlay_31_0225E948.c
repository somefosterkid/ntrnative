#include "global.h"

#include "overlay_31.h"
#include "unk_02005D10.h"
#include "pokeathlon/pokeathlon_save.h"
#include "msgdata.h"
#include "overlay_03.h"
#include "player_data.h"


String *ov31_0225E95C(MartData *arg0, MsgData *arg1);

u32 ov31_0225E948(u32 param_1, s32 param_2) {
    if (param_2 == 1) {
        PlaySE(0x643);
    }
    return 0;
}

String *ov31_0225E95C(MartData *data, MsgData *msgData) {
    s32 v0;
    u32 unkAmount;

    if ((u32)(u8)(data->martType + 0xFD) <= 1U) {
        unkAmount = PokeathlonSave_GetAthletePoints(data->pokeathlonSave);
    } else {
        unkAmount = PlayerProfile_GetMoney(data->playerProfile);
    }
    v0 = MartData_BuyMenu_CheckCanPurchaseItem(data, unkAmount);
    if (v0 == 2) {
        return NewString_ReadMsgData(msgData, 0x19);
    }
    if (v0 == 3) {
        return NewString_ReadMsgData(msgData, 0x1A);
    }
    if ((u32)(u8)(data->martType + 0xFD) <= 1U) {
        return NewString_ReadMsgData(msgData, 0x30);
    }
    return NewString_ReadMsgData(msgData, 0xB);
}
