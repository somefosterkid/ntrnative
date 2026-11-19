#include "global.h"

#include "player_data.h"
#include "unk_02034354.h"
#include "unk_02034354_internal.h"

s32 sub_02034960(s32);

void sub_02034AEC(SaveData *arg0);

void sub_02034AEC(SaveData *saveData) {
    sub_02034960(2);
    sub_02034A20(saveData);
}

void sub_02034B00(PlayerProfile *arg0);

void sub_02034B00(PlayerProfile *arg0) {
    _021D4130->personalTrainerInfo = arg0;
}
