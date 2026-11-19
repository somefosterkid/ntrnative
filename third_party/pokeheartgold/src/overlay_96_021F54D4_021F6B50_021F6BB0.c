#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

void *ov96_021F6BB0(UnkStruct_ov96_021F5018 *arg0);

#if defined(SDK_PORT) || defined(NONMATCHING)
void *ov96_021F6BB0(UnkStruct_ov96_021F5018 *arg0) {
    s32 var_r1;
    s32 var_r5;

    var_r1 = 0;
    do {
        var_r5 = arg0->unk_FAC[var_r1].unk_14;
        if (var_r5 == 0) {
            var_r5 = arg0->unk_FA0[var_r1];
            var_r5 = arg0->unk_1A0[0][var_r5];
            arg0->unk_FAC[var_r1].unk_08 = var_r5;
        }
        var_r1 += 1;
    } while (var_r1 < 3);
    return &arg0->unk_FAC[var_r1];
}
#else
void *ov96_021F6BB0(UnkStruct_ov96_021F5018 *arg0) {
    s32 var_r1;
    s32 var_r4;
    s32 var_r5;
    void *temp_r2;
    void *var_r0;
    void *var_r3;

    temp_r2 = (void *)((u8 *)arg0 + 0x1A0);
    var_r4 = 0xE;
    var_r1 = 0;
    var_r0 = (void *)((u8 *)arg0 + 0xFAC);
    var_r3 = temp_r2;
    var_r4 = 0xE00;
    do {
        var_r5 = *(s32 *)((u8 *)var_r0 + 0x14);
        if (var_r5 == 0) {
            var_r5 = *(s32 *)((u8 *)var_r3 + 0xE00);
            var_r5 *= 4;
            var_r5 = *(s32 *)(temp_r2 + var_r5);
            *(s32 *)((u8 *)var_r0 + 8) = var_r5;
        }
        var_r1 += 1;
        var_r0 += 0x1C;
        var_r3 += 4;
    } while (var_r1 < 3);
    return (u8 *)var_r0;
}
#endif
