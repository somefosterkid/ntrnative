#include "global.h"
#include "overlay_80_frontier_opponent.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

typedef u16 charcode_t;

int sub_0202D230(void);
void sub_0202D234(BattleTowerSave *save);
void sub_0202D240(WifiBattleTowerRecord *record);

int sub_0202D230(void) {
    return sizeof(WifiPlayerProfile);
}

void sub_0202D234(BattleTowerSave *save) {
    MI_CpuClear8(save, sizeof(BattleTowerSave));
}

void sub_0202D240(WifiBattleTowerRecord *record) {
    MI_CpuClear8(record, sizeof(WifiBattleTowerRecord));
    record->rank = 1;
}
