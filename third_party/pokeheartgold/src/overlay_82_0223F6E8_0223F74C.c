#include "global.h"

#include "overlay_82.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "unk_02035900.h"
#include "overlay_82_battle_hall_app.h"

void ov82_0223F74C(void *arg0, s16 arg1) {
    Save_PlayerData_GetProfile(((BattleHallApp *)arg0)->saveData);
    *((s16 *)&((BattleHallApp *)arg0)->commPayload[0]) = arg1;
}

void ov82_0223F764(int netID, int unused, void *unused2, void *unused3) {
    if (sub_0203769C() == netID) {
        return;
    }
}

void ov82_0223F770(void *arg0, s16 arg1, s16 arg2) {
    *((s16 *)&((BattleHallApp *)arg0)->commPayload[0]) = arg1;
    *((s16 *)&((BattleHallApp *)arg0)->commPayload[1]) = arg2;
    if ((sub_0203769C() == 0) && (((BattleHallApp *)arg0)->selectionID == 0xFF)) {
        ((BattleHallApp *)arg0)->selectionID = (u8)arg2;
    }
    *((s16 *)&((BattleHallApp *)arg0)->commPayload[2]) = (s16) ((BattleHallApp *)arg0)->selectionID;
    *((s16 *)&((BattleHallApp *)arg0)->commPayload[3]) = (s16)GetMonData(Party_GetMonByIndex(((BattleHallApp *)arg0)->party, 0), 0xA1, NULL);
}
