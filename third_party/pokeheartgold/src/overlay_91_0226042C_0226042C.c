#include "global.h"

void ov91_0226042C(void *arg0, void *arg1);

void ov91_0226042C(void *arg0, void *arg1) {
    *(s16 *)arg1 = (s16) (0xFF - ((s32) (*(s32 *)arg0 + 0x4B000) / (s32) (0x4B000U >> 7)));
    *(s16 *)((u8 *)arg1 + 2) = (s16) ((s32) (0xFFF1F000 - *(s32 *)((u8 *)arg0 + 8)) / 2400);
}
