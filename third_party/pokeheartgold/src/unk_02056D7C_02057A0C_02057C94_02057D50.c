#include "global.h"

#include "comm_player_manager.h"

typedef unsigned char undefined1;

typedef unsigned int undefined4;

void sub_02057D50(int param_1, int param_2, void *param_3, void *unused3);

void sub_02057D50(int param_1, int param_2, void *param_3, void *unused3)

{
    if (*(u8 *)param_3 == '\x03') {
        _021D41C4->inputLocked[param_1] = 0;
        return;
    }
    _021D41C4->lockOnSpot[param_1] = *(u8 *)param_3;
    return;
}
