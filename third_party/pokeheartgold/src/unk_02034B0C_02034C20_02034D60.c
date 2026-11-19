#include "global.h"

#include "comm_server_client.h"

void sub_02034D60(void *param_1, s32 param_2);

void sub_02034D60(void *param_1, s32 param_2) {
    if (param_2 != 0) {
        OS_Terminate();
    }
    _021D4134.status = 2;
    return;
}
