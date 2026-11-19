#include "global.h"

#include "comm_player_manager.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02056D7C.h"

u16 sub_02057B4C(s32);

u16 sub_02057B9C(s32);

extern s32 _0210FAB4;

extern s32 _0210FABC;

void sub_02057D74(void);

/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u16 sub_02057A34(s32 arg0);
#else
s32 sub_02057A34(s32 arg0);
#endif
s32 sub_02058024(void);
s32 sub_0205DFD4(PlayerAvatar *, s32);
s32 sub_0206234C(s32, s32);
void sub_02057E08(void);

void sub_02057D74(void) {
    u8 v1;
    s32 *v0;
    s32 v2;
    s32 v3;

    v2 = sub_0203774C((u32)sub_0203993C());
    if (v2 == 2) {
        v0 = &_0210FAB4;
    } else {
        v0 = &_0210FABC;
    }
    v1 = 0;
    if (v2 > 0) {
        do {
            if (_021D41C4->lockOnSpot[v1] != 0) {
                v3 = 0;
                if (v2 > 0) {
                    s32 *v4;
                    v4 = v0;
                    do {
                        if ((*(u16 *)v4 == sub_02057B4C(v1)) && (*(u16 *)((u8 *)v4 + 2) == sub_02057B9C(v1))) {
                            _021D41C4->inputLocked[v1] = 1;
                            sub_02037184(0x5F, &v1);
                        }
                        v3 += 1;
                        v4 += 1;
                    } while (v3 < v2);
                }
            }
            v1 += 1;
        } while ((s32)v1 < v2);
    }
}

void sub_02057E08(void) {
    s32 v1;
    u16 v0;

    v0 = sub_0203769C();
    if (sub_02057A34(v0) > 8) {
        sub_02057F28(2U);
        v1 = sub_0206234C(2, 0x24);
    } else {
        sub_02057F28(3U);
        v1 = sub_0206234C(3, 0x24);
    }
    sub_0205DFD4(_021D41C4->avatars[v0], v1);
    sub_02058024();
}
