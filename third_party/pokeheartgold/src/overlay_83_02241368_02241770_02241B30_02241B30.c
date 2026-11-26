#include "global.h"
struct BattleCastleSelfApp;

#include "battle_castle_app_sprite.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_83_battle_castle_self_app.h"
#include "overlay_80_02229EE0.h"


u8 ov80_02237B24(u8, s32, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02247600(BattleCastleAppSprite *, u8);
#else
void ov83_02247600(s32, s32);
#endif

s32 ov83_0224760C(void *, u8);

u8 ov83_02247768(u8, u8);

void ov83_02241B30(struct BattleCastleSelfApp *arg0, s32 arg1, s32 arg2);

void ov83_02241B30(BattleCastleSelfApp *arg0, s32 arg1, s32 arg2) {
    Pokemon *sp4;
    s32 sp0;
    Pokemon *temp_r0_2;
    s32 temp_r0;
    s32 var_r4;
    u32 *var_r5;
    temp_r0 = ov80_02237B24(arg0->challengeType, 1, arg2);
    var_r4 = 0;
    sp0 = temp_r0;
    if (temp_r0 > 0) {
        var_r5 = (u32 *)arg0;
        do {
            temp_r0_2 = Party_GetMonByIndex(arg0->party, var_r4);
            sp4 = temp_r0_2;
            temp_r0 = GetMonData(temp_r0_2, 0xA3, 0L);
            temp_r0 = ov80_0222A43C((u16)temp_r0, (u16)GetMonData(temp_r0_2, 0xA4, 0L));
            if ((*(s32 *)((u8 *)var_r5 + offsetof(BattleCastleSelfApp, monIconSprites))) != 0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
                ov83_02247600(*(BattleCastleAppSprite **)((u8 *)var_r5 + offsetof(BattleCastleSelfApp, monIconSprites)), temp_r0);
#else
                ov83_02247600(*(s32 *)((u8 *)var_r5 + offsetof(BattleCastleSelfApp, monIconSprites)), temp_r0);
#endif
                if ((var_r4 != ov83_02247768(arg0->numSlots, arg0->selectedMonSlot)) || (((u32)(arg0->selectedMonSlot)) >= ((u32)(arg0->exitSlot)))) {
                    ov83_0224760C(*(void **)((u8 *)var_r5 + offsetof(BattleCastleSelfApp, monIconSprites)), 0);
                } else {
                    ov83_0224760C(*(void **)((u8 *)var_r5 + offsetof(BattleCastleSelfApp, monIconSprites)), 1);
                }
            }
            var_r4 += 1;
            var_r5 += 1;
        } while (var_r4 < sp0);
    }
}
