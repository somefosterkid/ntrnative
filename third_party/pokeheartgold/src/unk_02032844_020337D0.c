#include "global.h"
#include "unk_02032844.h"
#include "wireless_manager.h"

void sub_020331CC(void *wm_portrecvcallback);

void sub_020337D0(WirelessManagerRecvFunc recvFunction, s32 port);

s32 sub_0203314C(u16 *, s32, s32, void *);
u16 sub_020338F4(void);
s32 sub_02033800(void *arg0, s32 arg1, s32 arg2, void *arg3);
s32 sub_02033108(void);
void sub_02033830(void);

void sub_020337D0(WirelessManagerRecvFunc recvFunction, s32 port) {
    _021D4124.sWirelessManager->recvFunc = recvFunction;
    if (WM_SetPortCallback((u16)port, sub_020331CC, NULL) != WM_ERRCODE_SUCCESS) {
        sub_02032844(9);
loop_2:
        goto loop_2;
    }
}

s32 sub_02033800(void *arg0, s32 arg1, s32 arg2, void *arg3) {
    if ((sub_020338F4() == 0) && !(sub_02033250() & 0xFE)) {
        return 0;
    }
    return sub_0203314C(arg0, arg1, arg2, arg3);
}

void sub_02033830(void) {
    if (_021D4124.sWirelessManager->state == 2) {
loop_1:
        goto loop_1;
    }
    if (sub_02033108() == 0) {
        sub_02032844(0xA);
    }
}
