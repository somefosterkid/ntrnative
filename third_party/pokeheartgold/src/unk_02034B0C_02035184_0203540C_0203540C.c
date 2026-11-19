#include "global.h"

#include "assert.h"
#include "comm_server_client.h"
#include "heap.h"
#include "mail_message.h"
#include "save_link_ruleset.h"
#include "system.h"
#include "unk_02033AE0.h"
#include "unk_02034B0C.h"
#include "unk_02037C94.h"

int sub_02033298(void);

BOOL sub_020347CC(void);

void sub_020353B8(void);

u16 sub_020338F4(void);

BOOL sub_0203567C(void);

s32 sub_020332AC(void);

void sub_020350D4(void);

s32 sub_020338D0(void);

u16 sub_02033468(void);

void sub_020352D8(void);

u16 sub_02035724(u16 param0);

BOOL sub_02033668(int connectionType, u16 tgid, u16 channel, u16 maxEntry, u16 beaconPeriod, BOOL entryFlag);

void sub_0203540C(u16 a0);

void sub_0203540C(u16 a0) {
    int state = sub_02033298();
    int v1 = sub_020347CC();

    sub_020353B8();

    if (sub_020338F4() == 0 && !sub_0203567C()) {
        if (_021D4134.sCommServerClient->unk_D95_2) {
            _021D4134.sCommServerClient->unk_D95_0 = 1;
        }
    }

    if (_021D4134.sCommServerClient->unk_D8E == 0xFFFF) {
        _021D4134.sCommServerClient->unk_D8E = a0;
    }

    if (_021D4134.sCommServerClient->unk_D95_1) {
        if (_021D4134.sCommServerClient->unk_D8E > a0) {
            _021D4134.sCommServerClient->unk_D95_0 = 1;
        }

        if (v1) {
            _021D4134.sCommServerClient->unk_D95_0 = 1;
        }
    }

    if (sub_020332AC() == 25) {
        sub_020399DC(0);
    }

    switch (state) {
    case 0:
        if (_021D4134.sCommServerClient->unk_D92 == 1) {
            sub_020350D4();
            return;
        }

        if (_021D4134.sCommServerClient->unk_D92 == 2) {
            _021D4134.sCommServerClient->unk_D92 = 3;
            return;
        }
        break;
    case 1:
        if (_021D4134.sCommServerClient->unk_D92 == 1) {
            if (sub_020338D0()) {
                return;
            }
        }

        if (_021D4134.sCommServerClient->unk_D92 == 2) {
            if (sub_020338D0()) {
                return;
            }
        }
        break;
    case 8:
    case 9:
        if (_021D4134.sCommServerClient) {
            _021D4134.sCommServerClient->unk_D95_0 = 1;
        }
        break;
    case 7: {
        u16 channel;

        channel = sub_02033468();

        if (_021D4134.sCommServerClient->unk_D91 == 0) {
            _021D4134.sCommServerClient->unk_D8C = channel;
            _021D4134.sCommServerClient->unk_D91 = 5;
        } else {
            _021D4134.sCommServerClient->unk_D91--;
        }

        channel = _021D4134.sCommServerClient->unk_D8C;

        if (_021D4134.sCommServerClient->unk_D95_3) {
            _021D4134.tgid = WM_GetNextTgid();
        }

        sub_020352D8();
        sub_02033668(0, _021D4134.tgid, channel, sub_02033FC4((int)sub_0203993C()), sub_02035724((int)sub_0203993C()), _021D4134.sCommServerClient->unk_D95_5);
        _021D4134.sCommServerClient->unk_D90 = channel;
    } break;
    default:
        break;
    }
}
