#include "global.h"

#include "frontier_data.h"
#include "overlay_86_frontier_records_app.h"
#include "save.h"
#include "unk_0202D230.h"

u32 ov86_021E6DD8(FrontierRecordsApp *param_1, u32 param_2);

u32 ov86_021E6DD8(FrontierRecordsApp *param_1, u32 param_2) {
    FrontierData *frontierData;
    u32 uVar1;
    u16 a1;

    frontierData = Save_FrontierData_Get(param_1->saveData);
    switch (param_2) {
    case 0:
        a1 = 8;
        break;
    case 1:
        a1 = 9;
        break;
    case 2:
        a1 = 10;
        break;
    case 3:
        a1 = 0xb;
        break;
    case 4:
        a1 = 0xc;
        break;
    case 6:
        a1 = 0xf;
        break;
    default:
        a1 = 8;
        break;
    }
    uVar1 = sub_0202D5DC(frontierData, a1, 0);
    if (uVar1 == 1) {
        return 0x1e;
    }
    return 0x1d;
}
