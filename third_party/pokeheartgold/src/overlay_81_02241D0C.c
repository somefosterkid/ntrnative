#include "global.h"
struct BattleFactoryApp;
#include "overlay_81.h"
#include "unk_02019BA4.h"
#include "overlay_81_battle_factory_app.h"

u32 ov81_02241D0C(BattleFactoryApp *app);

s32 ov81_02241C84(u32, s32, s32 *, s32 *);
void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);
void ov81_02242D94(BattleFactoryAppPanelSprite *panelSprite, int x, int y);

u32 ov81_02241D0C(BattleFactoryApp *app) {
    u32 uVar1;
    u16 *iVar2;

    uVar1 = 0;
    iVar2 = (u16 *)app;
    do {
        if ((u32)app->unk_468 == (u32) * (u16 *)((u8 *)iVar2 + offsetof(BattleFactoryApp, selectedIndices))) {
            return uVar1;
        }
        uVar1 = uVar1 + 1;
        iVar2 = iVar2 + 1;
    } while (uVar1 < 3);
    return (u32)app->numMonsSelected;
}

void ov81_02241D38(BattleFactoryApp *app) {
    s32 v1;
    s32 v0;
    u32 v2;

    v2 = (u32)GridInputHandler_GetNextInput(*((GridInputHandler * *)&app->inputHandler));
    if (v2 < 4U) {
        if (v2 == 3) {
            ov81_02242D88(app->unk_390, 0);
            return;
        }
        ov81_02242D88(app->unk_390, 1);
        ov81_02241C84(v2, *((s32 *)&app->numMonsToFade), &v1, &v0);
        ov81_02242D94(app->unk_390, v1, v0);
    }
}

void ov81_02241D94(BattleFactoryApp *app) {
    u32 v0;

    v0 = (u32)GridInputHandler_GetNextInput(app->inputHandler);
    if (v0 < 6U) {
        if (v0 >= 4U) {
            ov81_02242D88(app->unk_390, 0);
            return;
        }
        ov81_02242D88(app->unk_390, 1);
        ov81_02241EDC(app, *((u16 *)&app->selectedIndices[0]), v0, 0);
    }
}
