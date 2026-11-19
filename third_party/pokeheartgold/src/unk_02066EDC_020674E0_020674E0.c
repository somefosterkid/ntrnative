#include "global.h"

#include "frontier_data.h"
#include "unk_0202D230.h"
#include "unk_02066EDC.h"

u32 sub_020674B0(u32 rng);

u32 sub_020674E0(SaveData *saveData) {
    u32 a2;
    FrontierData *frontierData;
    u32 a0;

    frontierData = Save_FrontierData_Get(saveData);
    a0 = sub_020674B0(sub_0202D63C(frontierData));
    sub_0202D638(frontierData, a0);
    a2 = sub_020674A4(a0);
    sub_0202D308(sub_0202D908(saveData), 0xAU, &a2);
    return a2;
}
