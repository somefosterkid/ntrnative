#include "global.h"

u16 ov74_02233A88(u8 *, u16);

u16 ov74_02233ACC(int);

void *ov74_02233B04(s32);

u32 ov74_02233E50(CTRDGTaskInfo *);

#include "pm_agb_save.h"

extern void *sPmAgbCartridgeSpec;

void ov74_02233DBC(s16 arg0);

void ov74_02233DBC(s16 arg0) {
    u32 temp_r4;
    u8 *temp_r5;

    temp_r5 = ov74_0223D33C.buffer;
    ov74_0223D33C.writeStatus = 1;
    MIi_CpuClear32(0U, temp_r5, 0x1000U);
    temp_r4 = ov74_02233ACC(arg0);
    MIi_CpuCopy32(ov74_02233B04(arg0), temp_r5, temp_r4);
    *(s32 *)((u8 *)temp_r5 + 0xFFC) = (s32) ov74_0223D33C.counter;
    *(s16 *)((u8 *)temp_r5 + 0xFF4) = arg0;
    *(s32 *)((u8 *)temp_r5 + 0xFF8) = 0x08012025;
    *(s16 *)((u8 *)temp_r5 + 0xFF6) = ov74_02233A88(temp_r5, ov74_02233ACC(arg0));
    CTRDG_WriteAndVerifyAgbFlashAsync((u16) (u8) (((s32) (arg0 + ov74_0223D33C.firstSector + 1) % 14) + (0xE * *(s32 *)((u8 *)(&sPmAgbCartridgeSpec) + 4))), temp_r5, 4U, ov74_02233E50);
}
