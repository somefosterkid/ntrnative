#include "global.h"

#include "player_data.h"
#include "unk_02034354.h"
#include "unk_02034354_internal.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02034960(int param0, int val);
#else
void sub_02034960(int param0, int val);
#endif

void sub_02034AEC(SaveData *arg0, int val);

void sub_02034AEC(SaveData *saveData, int val) {
    sub_02034960(2, val);
    sub_02034A20(saveData);
}

void sub_02034B00(PlayerProfile *arg0);

void sub_02034B00(PlayerProfile *arg0) {
    _021D4130->personalTrainerInfo = arg0;
}
