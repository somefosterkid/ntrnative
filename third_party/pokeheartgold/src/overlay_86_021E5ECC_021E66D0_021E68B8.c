#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "player_avatar.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"

u32 ov86_021E68B8(FrontierRecordsApp *param_1);

u32 ov86_021E68B8(FrontierRecordsApp *param_1) {
    u8 uVar1;
    u16 uVar2;
    FrontierSave *pFVar3;
    u32 uVar4;
    u32 uVar5;

    uVar1 = param_1->challengeType;
    if (uVar1 == 3) {
        pFVar3 = Save_Frontier_GetStatic(param_1->saveData);
        uVar4 = sub_0205C268(0x6c);
        uVar2 = FrontierSave_GetStat(pFVar3, 0x6c, uVar4);
        uVar5 = (u32)uVar2;
    } else {
        uVar5 = sub_02030E58(sub_02030E08(param_1->saveData), 9, uVar1, 0, 0);
    }
    if ((uVar5 & 0xff) == 1) {
        return 0x1e;
    }
    return 0x1d;
}
