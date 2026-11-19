#include "global.h"

#include "frontier_data.h"
#include "save.h"
#include "unk_0202D230.h"
#include "unk_02066EDC.h"

u32 sub_0206751C(SaveData *saveData) {
    FrontierData *record = Save_FrontierData_Get(saveData);
    struct BattleTowerSave *save = sub_0202D908(saveData);

    int i;
    u32 rng = sub_020674A4(sub_0202D63C(record));
    int round = sub_0202D57C(record, sub_0202D284(save, 0, 0L), 0);
    round *= 24;

    for (i = 0; i < round; i++) {
        rng = sub_020674A4(rng);
    }

    sub_0202D308(sub_0202D908(saveData), 10, &rng);

    return rng;
}
