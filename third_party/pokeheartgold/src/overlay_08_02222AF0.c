#include "global.h"
#include "battle/battle_bag_context.h"

#include "unk_02005D10.h"
#include "overlay_08_battle_bag.h"

u8 ov08_02222B8C(void *);

s32 ov08_02223368(void *, s32 *);

u16 ov08_02223CD4(void *, u32);

s32 ov08_0222417C(void *);

s32 ov08_02224938(void *, u8, u8);

s32 ov08_02224C94(void *);

extern s32 ov08_02225ADC;

s32 ov08_02222AF0(void *arg0);

s32 ov08_02222AF0(void *arg0) {
    s32 v0;

    v0 = ov08_02223368(arg0, &ov08_02225ADC);
    if (v0 == -1) {
        v0 = ov08_02224C94(*((void **)&((BattleBag *)arg0)->cursor));
        if (v0 == -2) {
            v0 = 1;
        }
    } else {
        ov08_0222417C(arg0);
    }
    switch (v0) {
    case 0:
        PlaySE(0x5DDU);
        ((BattleBag *)arg0)->context->selectedBattleBagItem = ov08_02223CD4(arg0, ((BattleBag *)arg0)->context->pocketCurrentPagePositions[((BattleBag *)arg0)->currentBattlePocket]);
        ((BattleBag *)arg0)->context->selectedBattleBagPocket = (u8) ((BattleBag *)arg0)->currentBattlePocket;
        ov08_02224938(arg0, 0xF, 0);
        return ov08_02222B8C(arg0);
    case 1:
        PlaySE(0x5DDU);
        *((s8 *)&((BattleBag *)arg0)->queuedState) = 5;
        ov08_02224938(arg0, 0x10, 0);
        return 0xB;
    default:
        return 3;
    }
}
