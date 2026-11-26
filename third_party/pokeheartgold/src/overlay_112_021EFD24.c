#include "global.h"

#include "error_handling.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EFD24(u16 *arg0, s16 arg1);

void ov112_021EFD24(u16 *arg0, s16 arg1) {
    s32 v1;
    u16 *v0;

    v1 = 0;
    v0 = arg0;
loop_1:
    if (*v0 != 0) {
        v1 += 1;
        v0 += 1;
        if (v1 < 0xE) {
            goto loop_1;
        }
    }
    if (v1 >= 0xE) {
        GF_AssertFail();
    }
    *(u16 *)(arg0 + v1) = arg1;
}

s32 ov112_021EFD4C(Ov112PokewalkerApp *arg0) {
    s32 v0;
    s32 v1;
    void *v2;
    s32 v4;

    v0 = 0;
    v1 = 0;
    do {
        if (arg0->unk_9DAC[v1].unk_0 != 0) {
            v0 += 1;
        }
        v1 += 1;
    } while (v1 < 3);
    v4 = 0;
    v2 = arg0;
    do {
        if (*(u16 *)((u8 *)v2 + offsetof(Ov112PokewalkerApp, unk_9DB8[0].unk_0)) != 0) {
            v0 += 1;
        }
        v4 += 1;
        v2 += sizeof(((Ov112PokewalkerApp *)0)->unk_9DB8[0]);
    } while (v4 < 0xA);
    if ((((u32)(arg0->unk_AABC << 0x19) >> 0x1F) != 0) && (arg0->unk_B002 != 0)) {
        v0 += 1;
    }
    return v0;
}
