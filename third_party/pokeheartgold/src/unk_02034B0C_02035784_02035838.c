#include "global.h"

#include "comm_server_client.h"

void sub_02035838(u32 param_1);

void sub_02035838(u32 param_1) {
    (*(void (*)()) & MI_CpuCopy8)(param_1, &_021D4134.sCommServerClient->unk_D68, 8);
    return;
}
