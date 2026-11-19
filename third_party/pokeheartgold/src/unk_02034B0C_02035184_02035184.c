#include "global.h"

#include "comm_server_client.h"
#include "player_data.h"
#include "unk_02033AE0.h"
#include "unk_02037C94.h"

u8 sub_02035184(void);

u8 sub_02035184(void) {
    return _021D4134.sCommServerClient->unk_D74;
}

void sub_02035198(void);

void sub_02035198(void) {
    _021D4134.sCommServerClient->unk_D74 = 0;
}

s32 sub_020351AC(s32 arg0);

s32 sub_020351AC(s32 arg0) {
    u8 v0;

    if (_021D4134.sCommServerClient->unk_D44[arg0] != 0) {
        v0 = ((CommGameInfo *)_021D4134.sCommServerClient->unk_114[arg0].gameInfo.userGameInfo)->unk_06;
        if (v0 == 0) {
            return 1;
        }
        return (s32)v0;
    }
    v0 = 0;
    return (s32)v0;
}

PlayerProfile *sub_02035798(s32);

void sub_020351DC(u16 cursorPos, PlayerProfile *profile);

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_020351DC(u16 cursorPos, PlayerProfile *profile) {
    s32 v0;
    s32 v1;
    u16 *v2;

    v1 = 0;
    v2 = _021D4134.sCommServerClient->unk_D44;
    v0 = 0;
loop_1:
    if (*v2 != 0) {
        if (cursorPos == v1) {
            PlayerProfile_Copy(sub_02035798(v0), profile);
            return;
        }
        v1 += 1;
        goto block_5;
    }
block_5:
    v0 += 1;
    v2 += 1;
    if (v0 >= 0x10) {
        return;
    }
    goto loop_1;
}
#else
void sub_020351DC(u16 cursorPos, PlayerProfile *profile) {
    s32 v0;
    s32 v1;
    void *v2;

    v1 = 0;
    v2 = *(void **)((u8 *)(&_021D4134) + 8);
    v0 = 0;
loop_1:
    if (*(u16 *)((u8 *)v2 + 0xD44) != 0) {
        if (cursorPos == v1) {
            PlayerProfile_Copy(sub_02035798(v0), profile);
            return;
        }
        v1 += 1;
        goto block_5;
    }
block_5:
    v0 += 1;
    v2 += 2;
    if (v0 >= 0x10) {
        return;
    }
    goto loop_1;
}
#endif

s32 sub_02032B84(s32, void *, u16);

s32 sub_02032E24(void);

s32 sub_02033298(void);

s32 sub_0203373C(s32, void *);

s32 sub_02035218(s32 arg0);

s32 sub_02035218(s32 arg0) {
    u8 v0;

    if (sub_02033298() == 2) {
        sub_02032E24();
        return 0;
    }
    if (sub_02033298() == 1) {
        v0 = sub_0203993C();
        _021D4134.sCommServerClient->unk_D90 = (s8)_021D4134.sCommServerClient->unk_114[arg0].channel;
        if (sub_0203401C((s32)v0) != 0) {
            sub_02032B84(1, _021D4134.sCommServerClient->unk_114[arg0].bssid, 0);
        } else {
            sub_0203373C(1, &_021D4134.sCommServerClient->unk_114[arg0]);
        }
        return 1;
    }
    return 0;
}
