#include "global.h"
#include "overlay_00_thumb.h"
#include "unk_02033AE0.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

s32 sub_02033F9C(void *, s32);

s32 sub_02037824(s32 arg0);

BOOL sub_020378CC(void);

s32 sub_02037824(s32 arg0) {
    return sub_02033F9C(&_021D4140.sCommunicationSystem->commQueueManSendServer, arg0);
}

s32 sub_02033C50(void *);

void sub_0203783C(void);

void sub_0203783C(void) {
    sub_02033C50(&_021D4140.sCommunicationSystem->commQueueManSendServer);
}

void sub_02037854(void);

void sub_02037854(void) {
    sub_02033C50(&_021D4140.sCommunicationSystem->commQueueManSend);
}

void sub_0203786C(s32 arg0);

void sub_0203786C(s32 arg0) {
    _021D4140.sCommunicationSystem->wifiConnected = (s8)arg0;
}

s32 sub_02037880(void);

s32 sub_02037880(void) {
    return _021D4140.sCommunicationSystem->wifiConnected;
}

void sub_02037894(s8 arg0, s32 arg1);

void sub_02037894(s8 arg0, s32 arg1) {
    if (_021D4140.sCommunicationSystem != NULL) {
        _021D4140.sCommunicationSystem->unk_6A6[arg1] = arg0;
    }
}

s32 sub_020378AC(s32 arg0);

s32 sub_020378AC(s32 arg0) {
    s32 v1;
    u8 v0;

    v1 = arg0;
    if (_021D4140.sCommunicationSystem != NULL) {
        v0 = _021D4140.sCommunicationSystem->unk_6A6[v1];
        if (v0 != 0xFF) {
            v1 = (s32)v0;
        }
    }
    return v1;
}

BOOL sub_020378CC(void) {
    if (!sub_02034044(sub_0203993C())) {
        return 0;
    }

    return ov00_021E7080();
}
