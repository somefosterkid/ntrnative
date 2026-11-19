#include "global.h"

void ov29_0225D620(void);

void ov29_0225D620(void) {
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    reg_GXS_DB_DISPCNT = 0x10 | (reg_GXS_DB_DISPCNT & 0xFFCFFFEF);
}
