#include "global.h"
#include "overlay_81_battle_factory_app.h"

void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);

void ov81_02242DCC(BattleFactoryAppPanelSprite *panelSprite, VecFx32 *position);

const VecFx32 *ov81_02242F30(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_0224218C(BattleFactoryApp *app);

void ov81_0224218C(BattleFactoryApp *app) {
    VecFx32 sp0;
    const VecFx32 *ballPos;
    u16 var_r4;
    u8 temp_r1;

    var_r4 = 3;
    do {
        temp_r1 = var_r4 - 3;
        if ((u32)temp_r1 < (u32)app->numMonsSelected) {
            ballPos = ov81_02242F30(app->ballSprites[app->selectedIndices[temp_r1]]);
            sp0 = *ballPos;
            sp0.x -= 0x4000;
            sp0.y += 0x4000;
            ov81_02242DCC(app->unk_3A8[var_r4], &sp0);
            ov81_02242D88(app->unk_3A8[var_r4], 1);
        } else {
            ov81_02242D88(app->unk_3A8[var_r4], 0);
        }
        var_r4 += 1;
    } while ((u32)var_r4 < 6U);
}
