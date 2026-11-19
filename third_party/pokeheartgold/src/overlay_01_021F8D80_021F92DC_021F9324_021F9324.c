#include "global.h"

void *ObjectEvent_GetGraphicsInfo(u32 spriteId);

extern s32 ov01_02206D00;

void *ov01_021F9324(u32 spriteId);

void *ov01_021F9324(u32 spriteId) {
    void *temp_r0;
    temp_r0 = ObjectEvent_GetGraphicsInfo(spriteId);
    if (temp_r0 == 0L) {
        return 0L;
    }
    return ((u8 *)&ov01_02206D00) + (((u32)((*((u16 *)(((u8 *)temp_r0) + 4))) << 0x1B)) >> 0x1A);
}
