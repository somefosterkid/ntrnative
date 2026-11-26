#include "global.h"

#include "constants/heap.h"

#include "unk_020379A0.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_02240290(void *);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov83_02240FAC(void *, u8, u8);
#else
s32 ov83_02240FAC(void *, s32, u8);
#endif

s32 ov83_02241354(void *);

s32 ov83_02241368(BattleCastleSelfApp *, u16, u16);

s32 ov83_022418E8(BattleCastleSelfApp *, u32, s32 arg2);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02241BC4(BattleCastleSelfApp *app, u8 slot, u8 menuOption);
#else
void ov83_02241BC4(BattleCastleSelfApp *app, u8 slot, u8 menuOption);
#endif

s32 ov83_0224776C(u32, u32);

s32 ov83_022477EC(s32, s32, void *, s32);

s32 ov83_0223EEA0(BattleCastleSelfApp *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov83_0223EEA0(BattleCastleSelfApp *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 temp_r1;
    u8 temp_r2;

    temp_r1 = arg0->subState;
    switch (temp_r1) {
    case 0:
        arg0->syncIncreaseRank = 0;
        ov83_022477EC(2, 0, &arg0->unk_868, arg3);
        if (ov83_02241368(arg0, 0xB, arg0->selectedMonSlot) == 1) {
            arg0->syncPurchaseAction = 0;
            arg0->subState = (u8)(arg0->subState + 1);
        }
        break;
    case 1:
        if (arg0->slotID != 0xFF) {
            *((s8 *)&arg0->msgsReceived) = 0;
            temp_r2 = arg0->selectedMenuEntry;
            if ((temp_r2 == 4) || (temp_r2 == 8)) {
                ov83_02241BC4(arg0, arg0->slotID, temp_r2);
            } else {
                ov83_022418E8(arg0, arg0->slotID, temp_r2);
            }
            arg0->subState = (u8)(arg0->subState + 1);
        }
        break;
    case 2:
        if (ov83_02240FAC(arg0, ov83_0224776C(arg0->exitSlot, arg0->slotID), arg0->selectedMenuEntry) == 1) {
            arg0->syncDelay = 0x1EU;
            arg0->subState = (u8)(arg0->subState + 1);
        }
        break;
    case 3:
        arg0->syncDelay = (u8)(arg0->syncDelay - 1);
        if (arg0->syncDelay == 0) {
            sub_02037BEC();
            sub_02037AC0(0x82U);
            arg0->subState = (u8)(arg0->subState + 1);
        }
        break;
    case 4:
        if (sub_02037B38(0x82U) == 1) {
            sub_02037BEC();
            sub_020379A0(HEAP_ID_107);
            arg0->slotID = 0xFF;
            if (arg0->syncIncreaseRank == 0) {
                ov83_02241354(&arg0->windows[6]);
                ov83_02240290(arg0);
            }
            *((s8 *)&arg0->partnerIsExiting) = 0;
            return 1;
        }
        break;
    }
    return 0;
}
