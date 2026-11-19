#include "global.h"
struct BattleCastleOpponentApp;

#include "party.h"
#include "pokemon.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "overlay_80_02229EE0.h"


u8 ov80_02237B24(u8, s32, s32);

s32 ov83_02247600(s32, s32);

s32 ov83_0224760C(void *, u8);

u8 ov83_02247768(u8, u8);

void ov83_022459AC(struct BattleCastleOpponentApp *arg0, s32 arg1, s32 arg2);

void ov83_022459AC(BattleCastleOpponentApp *arg0, s32 arg1, s32 arg2) {
    Pokemon *sp4;
    s32 sp0;
    Pokemon *temp_r0_2;
    s32 temp_r0;
    s32 temp_r1;
    s32 var_r4;
    unsigned int temp_r7;
    void *var_r5;
    temp_r0 = ov80_02237B24(arg0->challengeType, 1, arg2);
    var_r4 = 0;
    sp0 = temp_r0;
    if (temp_r0 > 0) {
        var_r5 = arg0;
        do {
            temp_r0_2 = Party_GetMonByIndex(arg0->opponentsParty, var_r4);
            sp4 = temp_r0_2;
            temp_r7 = GetMonData(temp_r0_2, 0xA3, 0L);
            temp_r1 = ov80_0222A43C((u16)temp_r7, (u16)GetMonData(temp_r0_2, 0xA4, 0L));
            if ((*((s32 *)(((u8 *)var_r5) + 0x4E4))) != 0) {
                ov83_02247600(*((s32 *)(((u8 *)var_r5) + 0x4E4)), temp_r1);
                if ((var_r4 != ov83_02247768(arg0->numSlots, arg0->selectedMonSlot)) || (((u32)(arg0->selectedMonSlot)) >= ((u32)(arg0->exitSlot)))) {
                    ov83_0224760C(*((void **)(((u8 *)var_r5) + 0x4E4)), 0);
                } else {
                    ov83_0224760C(*((void **)(((u8 *)var_r5) + 0x4E4)), 1);
                }
            }
            var_r4 += 1;
            var_r5 += 4;
        } while (var_r4 < sp0);
    }
}
