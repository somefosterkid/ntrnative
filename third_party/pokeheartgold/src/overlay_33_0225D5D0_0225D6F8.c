#include "global.h"

#include "overlay_33.h"

void ov33_0225D6F8(void) {
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    reg_GXS_DB_DISPCNT = 0x10 | (reg_GXS_DB_DISPCNT & 0xFFCFFFEF);
}
