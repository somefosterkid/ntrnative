#include "global.h"

#include "unk_02035900.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_0224143C(BattleCastleSelfApp *arg0, u16 arg1, u16 arg2);

void ov83_0224143C(BattleCastleSelfApp *arg0, u16 arg1, u16 arg2) {
    arg0->commPayload[0] = arg1;
    arg0->commPayload[1] = arg2;
    if ((sub_0203769C() == 0) && (arg0->slotID == 0xFF)) {
        arg0->slotID = (u8)arg2;
    }
    arg0->commPayload[2] = (s16) * &arg0->slotID;
    arg0->commPayload[4] = (u16) * &arg0->selectedRentalItem;
    arg0->commPayload[5] = (s16) * &arg0->selectedMenuEntry;
}
