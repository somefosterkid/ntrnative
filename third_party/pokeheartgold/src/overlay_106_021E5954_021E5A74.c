#include "global.h"

void ov106_021E5A74(GXRgb *param_1);

void ov106_021E5A74(GXRgb *param_1) {
    if (param_1 == NULL) {
        reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & 0xcfdf;
        return;
    }
    reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & 0xFFFFCFFF | 0x20;
    G3X_SetEdgeColorTable(param_1);
    return;
}
