#include "global.h"

#include "comm_player_manager.h"
#include "error_handling.h"

void sub_02057C5C(int netId, BOOL movementEnabled);

void sub_020573AC(int netId, int param1, void *param2, void *unused);

void sub_020573AC(int netId, int param1, void *param2, void *unused) {
    u8 *buffer = (u8 *)param2;

    if (_021D41C4) {
        ((param1 == 1) ? (void)0 : GF_AssertFail());
        ((netId < 8) ? (void)0 : GF_AssertFail());
        (((buffer[0] == 1) || (buffer[0] == 0)) ? (void)0 : GF_AssertFail());

        sub_02057C5C(netId, buffer[0]);
    }
}
