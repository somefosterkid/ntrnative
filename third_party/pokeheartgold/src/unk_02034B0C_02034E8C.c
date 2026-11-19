#include "global.h"
#include "comm_server_client.h"
#include "unk_02035900.h"

void sub_02034E8C(void);

typedef void (*WirelessManagerRecvFunc)(u16, u16 *, u16);
int sub_02033298(void);
BOOL sub_020332C0(void);
void sub_020337D0(WirelessManagerRecvFunc recvFunction, int port);
void sub_02033A68(void);
void sub_02034E64(BOOL param0);
s32 sub_02034EF0(s32 param0, s32 param1, s32 param2);

void sub_02034E8C(void) {
    _021D4134.sCommServerClient->unk_D74 = 0;
    _021D4134.sCommServerClient->unk_D95_0 = 0;
    _021D4134.sCommServerClient->unk_D95_2 = 0;
    _021D4134.sCommServerClient->unk_D92 = 0;
    _021D4134.sCommServerClient->unk_D95_4 = 0;
    _021D4134.sCommServerClient->unk_D94 = 0;
    _021D4134.sCommServerClient->unk_D93 = 0;
}

s32 sub_02034EF0(s32 param0, s32 param1, s32 param2) {
    sub_02034E8C();
    sub_02034E64(param1);
    sub_02033A68();
    if (_021D4134.sCommServerClient->unk_D93 == 0) {
        sub_020337D0(sub_02036904, 0xE);
        _021D4134.sCommServerClient->unk_D93 = 1U;
    }
    _021D4134.sCommServerClient->unk_D95_5 = param2;
    if ((sub_02033298() == 1) && (sub_020332C0() != 0)) {
        return 1;
    }
    return 0;
}
