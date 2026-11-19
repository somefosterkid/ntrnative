#include "global.h"

#include "overlay_112_ir_state.h"

extern OSMutex ov112_021FFB08;

extern OSThread ov112_021FFBD8;

void ov112_021E7464(void);

void ov112_021E7464(void) {
    OS_LockMutex(&ov112_021FFB08);
    OS_KillThread(&ov112_021FFBD8, NULL);
    OS_UnlockMutex(&ov112_021FFB08);
}

void ov112_021E5EC4(u8 *, u8 *, u16);


void ov112_021E7484(s32 arg0);

void ov112_021E7484(s32 arg0) {
    ov112_021FFB24.unk1C = 0x2A;
    if (arg0 != 0) {
        ov112_021FFB24.unk1D = 0x2A;
    } else {
        ov112_021FFB24.unk1D = 0x2C;
    }
    ov112_021E5EC4(0, 0, 0x28);
}
