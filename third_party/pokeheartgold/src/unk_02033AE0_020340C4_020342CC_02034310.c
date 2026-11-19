#include "global.h"

#include "comm_cmd_manager.h"
#include "unk_02035900.h"

void sub_02034310(s32 param0, s32 param1, void *param2, void *param3);

void sub_02034310(s32 param0, s32 param1, void *param2, void *param3) {
    _021D412C->cmdTable = NULL;
    _021D412C->unk_04 = 0;
    _021D412C->unk_08 = NULL;
    _021D412C->unk_14 = 1;
    sub_020376E0(0xF, (u8 *)param2);
}
