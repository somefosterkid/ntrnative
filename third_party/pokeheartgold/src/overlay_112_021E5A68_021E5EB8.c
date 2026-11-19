#include "global.h"

#include "overlay_112_ir_state.h"


void ov112_021E5EB8(u8 *arg0, u8 *arg1);

void ov112_021E5EB8(u8 *arg0, u8 *arg1) {
    ov112_021FFAA4.unk_20 = arg0;
    ov112_021FFAA4.unk_24 = arg1;
}


void ov112_021E5EC4(u8 *arg0, u8 *arg1, u16 arg2);

void ov112_021E5EC4(u8 *arg0, u8 *arg1, u16 arg2) {
    s32 v0;

    ov112_021FFB24.unk20 = arg0;
    ov112_021FFB24.unk24 = arg0;
    ov112_021FFB24.unk28 = arg1;
    ov112_021FFB24.unk2C = arg1;
    ov112_021FFB24.unk30 = arg2;
    v0 = arg2 + 0x7F;
    ov112_021FFB24.unk32 = (u16) ov112_021FFB24.unk30;
    ov112_021FFB24.unk36 = 0;
    ov112_021FFB24.unk34 = (s16)((s32)(v0 + ((u32)(v0 >> 6) >> 0x19)) >> 7);
}
