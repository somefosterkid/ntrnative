#include "global.h"

#include "overlay_112_ir_state.h"

void ov112_021E5EC4(u8 *, u8 *, u16);



void ov112_021E7620(u8 *arg0, u8 *arg1);

void ov112_021E7620(u8 *arg0, u8 *arg1) {
    s32 temp_r2;

    ov112_021FFAA4.unk_38 = arg0;
    ov112_021FFAA4.unk_30 = arg1;
    ov112_021FFB24.unk1C = 0x60;
    ov112_021FFB24.unk1D = 0x20;
    ov112_021E5EC4(0, 0, 0xD4C);
    ov112_021FFB24.unk3C = 0x24D8U;
    ov112_021FFB24.unk3A = 0;
    temp_r2 = ov112_021FFB24.unk3C + 0x7F;
    ov112_021FFB24.unk38 = (s16)(temp_r2 / 0x80);
}
