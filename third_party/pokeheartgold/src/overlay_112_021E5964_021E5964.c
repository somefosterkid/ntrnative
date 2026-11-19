#include "global.h"

#include "overlay_112_ir_state.h"

void CARD_SetSpiReadWaitCycles(u32);

void CARD_SetSpiWriteWaitCycles(u32);

void CARD_SpiWaitInit(void);

void CARD_SpiWaitReadRange(u8 *);

void ov112_021E594C(void);

extern u8 _021FF500;


extern void *ov112_021FFA18;

void ov112_021E5964(void);

void ov112_021E5964(void) {
    OSTick OVar1;

    CARD_SpiWaitInit();
    CARD_SetSpiWriteWaitCycles(0x32);
    CARD_SetSpiReadWaitCycles(0x32);
    CARD_SpiWaitReadRange((u8 *)(&ov112_021FFA18));
    ov112_021E594C();
    _021FF500 = 0xff;
    ((u8 *)&_021FF500)[1] = 0xff;
    OVar1 = OS_GetTick();
    _021FF9E0.unk_20 = (s32)OVar1;
    _021FF9E0.unk_04 = (s32)OVar1;
    _021FF9E0.unk_0C = (void *)(0);
    _021FF9E0.unk_1C = (void *)(0);
    return;
}
