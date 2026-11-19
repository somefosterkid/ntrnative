#include "global.h"

#include "overlay_112_ir_state.h"

#include "sys_task.h"

s32 ov112_021E59A8(void);

s32 ov112_021E7668(void);

u8 ov112_021E7670(void);
u8 ov112_021E768C(void);

s32 ov112_021E7668(void) {
    return ov112_021E59A8();
}

u8 ov112_021E7670(void) {
    return (u8)((s32)(0xFF * ov112_021FFB24.unk36) / (s32) ov112_021FFB24.unk34);
}

u8 ov112_021E768C(void) {
    return (u8)((s32)(0xFF * ov112_021FFB24.unk3A) / (s32) ov112_021FFB24.unk38);
}
