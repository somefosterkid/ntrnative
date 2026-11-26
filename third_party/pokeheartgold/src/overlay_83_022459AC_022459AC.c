#include "global.h"
struct BattleCastleOpponentApp;

#include "battle_castle_app_sprite.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "overlay_80_02229EE0.h"


u8 ov80_02237B24(u8, s32, s32);

void ov83_02247600(BattleCastleAppSprite *, u8);

void ov83_0224760C(BattleCastleAppSprite *, u8);

u8 ov83_02247768(u8, u8);

void ov83_022459AC(struct BattleCastleOpponentApp *arg0, s32 arg1, s32 arg2);

#if defined(SDK_PORT) || defined(NONMATCHING)
#define SPR (*var_r5)
#else
#define SPR (*(BattleCastleAppSprite **)((u8 *)var_r5 + offsetof(BattleCastleOpponentApp, monSprites)))
#endif

void ov83_022459AC(BattleCastleOpponentApp *arg0, s32 arg1, s32 arg2) {
    Pokemon *sp4;
    s32 sp0;
    Pokemon *temp_r0_2;
    s32 temp_r0;
    u8 temp_r1;
    s32 var_r4;
    unsigned int temp_r7;
#if defined(SDK_PORT) || defined(NONMATCHING)
    BattleCastleAppSprite **var_r5;
#else
    u32 *var_r5;
#endif
    temp_r0 = ov80_02237B24(arg0->challengeType, 1, arg2);
    var_r4 = 0;
    sp0 = temp_r0;
    if (temp_r0 > 0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
        var_r5 = arg0->monSprites;
#else
        var_r5 = (u32 *)arg0;
#endif
        do {
            temp_r0_2 = Party_GetMonByIndex(arg0->opponentsParty, var_r4);
            sp4 = temp_r0_2;
            temp_r7 = GetMonData(temp_r0_2, 0xA3, 0L);
            temp_r1 = ov80_0222A43C((u16)temp_r7, (u16)GetMonData(temp_r0_2, 0xA4, 0L));
            if (SPR != NULL) {
                ov83_02247600(SPR, temp_r1);
                if ((var_r4 != ov83_02247768(arg0->numSlots, arg0->selectedMonSlot)) || (((u32)(arg0->selectedMonSlot)) >= ((u32)(arg0->exitSlot)))) {
                    ov83_0224760C(SPR, 0);
                } else {
                    ov83_0224760C(SPR, 1);
                }
            }
            var_r4 += 1;
            var_r5++;
        } while (var_r4 < sp0);
    }
}
