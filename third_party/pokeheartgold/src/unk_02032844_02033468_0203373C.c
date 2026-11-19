#include "global.h"

#include "unk_02037C94.h"
#include "wireless_manager.h"

s32 sub_02032E9C(void);

void sub_02033664(void *arg);

s32 sub_020340C4(int cmd);

s32 sub_0203373C(s32 arg0, void *arg1);

s32 sub_0203373C(s32 arg0, void *src) {
    if (sub_020340C4(sub_0203993C()) != 0) {
        WM_SetLifeTime(sub_02033664, 0xFFFFU, 0x64U, 5U, 0x64U);
    }
    _021D4124.sWirelessManager->recvBufferSize = 0x200;
    _021D4124.sWirelessManager->sendBufferSize = 0x40;
    _021D4124.sWirelessManager->connectionType = arg0;
    sub_02032844(3);
    if ((arg0 == 1) || (arg0 == 3) || (arg0 == 5)) {
        MI_CpuCopy8(src, &_021D4124.sWirelessManager->bssDesc, 0xC0U);
        DC_FlushRange(&_021D4124.sWirelessManager->bssDesc, 0xC0U);
        DC_WaitWriteBufferEmpty();
        return sub_02032E9C();
    }
    return 0;
}
