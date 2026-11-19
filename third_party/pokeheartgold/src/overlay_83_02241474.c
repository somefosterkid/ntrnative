#include "global.h"
struct BattleCastleSelfApp;

#include "unk_02035900.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02241474(s32 netID, s32 unused, void *data, void *context);

void ov83_022414CC(BattleCastleSelfApp *arg0, u16 arg1);

void ov83_02240DE0(BattleCastleSelfApp *arg0, u32 arg1, s32 arg2);
void ov83_022414DC(u32 param_1, u32 param_2, void *param_3, struct BattleCastleSelfApp *param_4);

void ov83_02241474(s32 netID, s32 unused, void *data, void *context) {
    BattleCastleSelfApp *app = context;
    u16 *payload = data;

    app->msgsReceived++;
    if (netID != sub_0203769C()) {
        app->partnerSlot = payload[1];
        if (sub_0203769C() == 0) {
            if (app->slotID != 0xFF) {
                app->partnerSlot = 0;
                return;
            }
            app->slotID = app->partnerSlot + app->exitSlot;
            app->selectedRentalItem = payload[4];
            app->selectedMenuEntry = payload[5];
            return;
        }
        app->slotID = payload[2];
        app->selectedRentalItem = payload[4];
        app->selectedMenuEntry = payload[5];
    }
}

void ov83_022414CC(BattleCastleSelfApp *arg0, u16 arg1) {
    arg0->commPayload[0] = arg1;
    arg0->commPayload[1] = arg0->selectedMonSlot;
}

void ov83_022414DC(u32 param_1, u32 param_2, void *param_3, BattleCastleSelfApp *param_4) {
    u16 uVar1;

    uVar1 = sub_0203769C();
    if (param_1 != uVar1) {
        param_4->partnersSelectedSlot = ((u16 *)param_3)[1];
        ov83_02240DE0(param_4, param_4->partnersSelectedSlot, 1);
    }
    return;
}
