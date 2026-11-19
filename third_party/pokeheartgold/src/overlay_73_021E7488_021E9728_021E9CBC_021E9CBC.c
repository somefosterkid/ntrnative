#include "global.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

void ov73_021E844C(void *, u32, u32);

void DWC_CleanupInet();

u32 ov73_021E9CBC(WifiBattleTowerAppState *param_1);

u32 ov73_021E9CBC(WifiBattleTowerAppState *param_1) {
    DWC_CleanupInet();
    ov73_021E844C(param_1, 0, 0);
    param_1->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTED;
    return 3;
}
