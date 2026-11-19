#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "player_avatar.h"
#include "save_frontier.h"
#include "unk_02030A98.h"

s32 ov86_021E6484(FrontierRecordsApp *arg0, s32 arg1);

s32 ov86_021E6484(FrontierRecordsApp *arg0, s32 arg1) {
    FrontierSave *frontierSave;
    u32 v1;
    u32 param0;
    u8 v0;

    if (arg1 == 0) {
        param0 = 0x66;
    } else {
        param0 = 0x68;
    }
    v0 = arg0->challengeType;
    if (v0 == 3) {
        frontierSave = Save_Frontier_GetStatic(arg0->saveData);
        v1 = FrontierSave_GetStat(frontierSave, (s32)param0, (s32)sub_0205C268(param0));
    } else {
        v1 = sub_02030AD4(sub_02030AE8(arg0->saveData), 0xA, (u8)(v0 + (arg1 * 4)), 0);
    }
    if ((u8)v1 == 1) {
        return 0x1E;
    }
    return 0x1D;
}
