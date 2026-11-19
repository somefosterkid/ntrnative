#include "global.h"

#include "comm_server_client.h"
#include "system.h"

void sub_02034D78(void);

void sub_02034D60(void *arg, enum WVRResult result);
void sub_02034D8C(void);
s32 sub_02034DB8(void);
s32 sub_02034DCC(void);

void sub_02034D78(void) {
    _021D4134.status = 0;
    Sys_ClearSleepDisableFlag(4);
}

void sub_02034D8C(void) {
    Sys_SetSleepDisableFlag(4);
    _021D4134.status = 1;
    if (WVR_StartUpAsync(GX_VRAM_ARM7_128_D, sub_02034D60, NULL) != WVR_RESULT_OPERATING) {
        OS_Terminate();
    }
}

s32 sub_02034DB8(void) {
    if (_021D4134.status == 2) {
        return 1;
    }
    return 0;
}

s32 sub_02034DCC(void) {
    if (_021D4134.status != 0) {
        return 1;
    }
    return 0;
}
