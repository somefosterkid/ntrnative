#include "global.h"

#include "comm_player_manager.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_0205FD20.h"

s32 sub_02057C5C(s32, s32);

s32 sub_02057EEC(int);

s32 sub_02057E50(void);

s32 sub_02057E50(void) {
    s32 sp4;
    s32 sp0;
    s32 temp_r0;
    s32 var_r0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    s32 var_r5;
    CommPlayerPosition *temp_r6;
#else
    s32 var_r4;
    s32 var_r5;
    void *temp_r6;
    void *temp_r7;
#endif

    sp0 = sub_0203774C((u32)sub_0203993C());
    for (var_r0 = 0; var_r0 < sp0; var_r0++) {
        if ((_021D41C4->active[var_r0] == 0) || (_021D41C4->positionChanged[var_r0] == 1)) {
            return 0;
        }
    }
    var_r5 = 0;
    if (sp0 > 0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
        do {
            temp_r6 = &_021D41C4->hostPositions[var_r5];
            temp_r0 = sub_02057EEC(temp_r6->facing);
            sp4 = temp_r0;
            temp_r6->x = temp_r6->x + GetDeltaXByFacingDirection(temp_r0);
            temp_r6->z = (u16)(temp_r6->z + GetDeltaYByFacingDirection(temp_r0));
            temp_r6->speed = 2;
            _021D41C4->positionChanged[var_r5] = 1;
            sub_02057C5C(var_r5, 1);
            var_r5 += 1;
#else
        var_r4 = 0;
        do {
            temp_r7 = (u8 *)_021D41C4 + 0x34;
            temp_r6 = temp_r7 + var_r4;
            temp_r0 = sub_02057EEC(*(s8 *)((u8 *)temp_r6 + 4));
            sp4 = temp_r0;
            *(u16 *)(temp_r7 + var_r4) = *(u16 *)(temp_r7 + var_r4) + GetDeltaXByFacingDirection(temp_r0);
            *(u16 *)((u8 *)temp_r6 + 2) = (u16)(*(u16 *)((u8 *)temp_r6 + 2) + GetDeltaYByFacingDirection(temp_r0));
            *(s8 *)((u8 *)temp_r6 + 5) = 2;
            _021D41C4->positionChanged[var_r5] = 1;
            sub_02057C5C(var_r5, 1);
            var_r5 += 1;
            var_r4 += 8;
#endif
        } while (var_r5 < sp0);
    }
    return 1;
}
