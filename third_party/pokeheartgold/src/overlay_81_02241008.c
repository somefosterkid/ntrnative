#include "global.h"

#include "overlay_81.h"
#include "player_data.h"
#include "unk_02035900.h"
#include "overlay_81_battle_factory_app.h"


void ov81_02241008(BattleFactoryApp *app, u16 cmd) {
    Save_PlayerData_GetProfile(app->saveData);
    app->commPayload[0] = cmd;
}

void ov81_02241020(int netID, int unused, void *unused2, void *unused3) {
    if (sub_0203769C() == netID) {
        return;
    }
}
