#include "global.h"

#include "save.h"
#include "unk_02034354.h"

s32 sub_02034960(s32, s32);

void sub_02034AC0(SaveData *saveData, int a1) {
    if (a1 == 1) {
        sub_02034960(0, 1);
    } else if (a1 == -1) {
        sub_02034960(1, 1);
    }
    sub_02034A20(saveData);
}
