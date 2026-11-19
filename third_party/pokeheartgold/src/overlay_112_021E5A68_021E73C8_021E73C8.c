#include "global.h"

#include "overlay_112_ir_state.h"

s32 CARD_SetSpiReadWaitCycles(s32);

s32 CARD_SetSpiWriteWaitCycles(s32);

s32 CARD_SpiWaitGetStatus();

s32 ov112_021E5964(void);

s32 ov112_021E5D38(s32 *);

s32 ov112_021E5D44(s32 *);

s32 ov112_021E5D50(s32 *);

s32 ov112_021E5D5C(s8);

void ov112_021E7398(void *arg0);

extern s32 ov112_021E6A6C;

extern s32 ov112_021E6BDC;

extern s32 ov112_021E6F60;



extern OSMutex ov112_021FFB08;

extern OSMessageQueue ov112_021FFB20;

extern OSThread ov112_021FFBD8;

void ov112_021E73C8(u8 *arg0, u32 arg1, u32 arg2);

void ov112_021E73C8(u8 *arg0, u32 arg1, u32 arg2) {
    ov112_021FFAA4.unk_18 = 1;
    ov112_021FFAA4.unk_14 = 1;
    ov112_021FFAA4.unk_44 = NULL;
    ov112_021FFAA4.unk_48 = NULL;
    ov112_021E5964();
    ov112_021E5D5C(1);
    ov112_021E5D38(&ov112_021E6A6C);
    ov112_021E5D44(&ov112_021E6F60);
    ov112_021E5D50(&ov112_021E6BDC);
    CARD_SpiWaitGetStatus();
    if (CARD_SpiWaitGetStatus() == 0xAA) {
        CARD_SetSpiWriteWaitCycles(0);
        CARD_SetSpiReadWaitCycles(0);
    }
    OS_InitMutex(&ov112_021FFB08);
    OS_InitMessageQueue(&ov112_021FFB20, ov112_021FFAA4.msgArray, 1);
    OS_CreateThread(&ov112_021FFBD8, ov112_021E7398, NULL, (void *)(arg0 + arg1), arg1, arg2);
    OS_WakeupThreadDirect(&ov112_021FFBD8);
}
