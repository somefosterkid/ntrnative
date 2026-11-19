#include "global.h"

void *ov96_021E94EC(void *, u32);

void ov96_0221A730(void *arg0);

void ov96_0221A730(void *arg0) {
    u8 sp10[4];
    u8 spC[4];
    u8 sp8[4] = { 100, 80, 70, 60 };
    void *sp4;
    u16 val;
    u8 cur;
    u8 i;
    u8 j;

    for (i = 0; i < 4; i++) {
        sp10[i] = *((u8 *)ov96_021E94EC(arg0, i) + 9);
        spC[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        cur = sp10[i];
        sp4 = ov96_021E94EC(arg0, i);
        for (j = 0; j < 4; j++) {
            if (i != j && cur == sp10[j]) {
                spC[i]++;
            }
        }
        val = sp8[cur + spC[i]] + *(s32 *)((u8 *)sp4 + 0xC) * 5;
        if ((double)val > 200.0) {
            val = 200;
        }
        *(u16 *)((u8 *)sp4 + 0xA) = val;
    }
}
