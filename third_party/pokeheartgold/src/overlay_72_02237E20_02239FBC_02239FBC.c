#include "global.h"
#include "overlay_72_wfc_state.h"
#include "overlay_72_wfc_connect.h"
struct NintendoWfcConnect;

void ov72_022387D0(struct NintendoWfcConnect *, u32, u32);

void DWC_CleanupInet();

u32 ov72_02239FBC(NintendoWfcConnect *param_1);

u32 ov72_02239FBC(NintendoWfcConnect *param_1) {
    DWC_CleanupInet();
    ov72_022387D0(param_1, 0, 0);
    param_1->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTED;
    return 3;
}
