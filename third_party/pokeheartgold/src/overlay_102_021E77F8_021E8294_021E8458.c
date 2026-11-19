#include "global.h"

#include "touchscreen.h"

u32 ov102_021EC5A0(s32, s32);

extern u8 gSystem;

extern u32 ov102_021EC5E0;

u32 ov102_021E8458(void);

u32 ov102_021E8458(void) {
    s32 iVar1;
    u32 uVar2;

    iVar1 = TouchscreenHitbox_FindRectAtTouchNew((TouchscreenHitbox *)&ov102_021EC5E0);
    uVar2 = 0xffffffff;
    if (iVar1 != -1) {
        uVar2 = ov102_021EC5A0((s32)((((u16 *)&gSystem)[0x30] - 0x14) * 0x10000) >> 0x10,
            (s32)((((u16 *)&gSystem)[0x31] - 0x40) * 0x10000) >> 0x10);
    }
    return uVar2;
}
