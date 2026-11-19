#include "global.h"

#include "comm_server_client.h"
#include "unk_02034B0C.h"
#include "wireless_manager.h"

void sub_02034D78(void *param0, WVRResult param1);

s32 sub_02033234(s32);
void sub_02034DF0(s32 arg0);

void sub_02034DE0(void) {
    WVR_TerminateAsync(sub_02034D78, 0L);
}

void sub_02034DF0(s32 arg0) {
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    u32 v0;
#endif

    _021D4134.sCommServerClient->unk_D70 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    // the next 32-byte boundary strictly above the buffer, as the pointer it is
    sub_02033528((u8 *)__builtin_align_down(_021D4134.sCommServerClient->unk_D64, 32) + 0x20, arg0);
#else
    v0 = (u32)_021D4134.sCommServerClient->unk_D64;
    sub_02033528((void *)(v0 + (0x20 - (v0 & 0x1F))), arg0);
#endif
    sub_02033234(_021D4134.sCommServerClient->unk_D80);
}
