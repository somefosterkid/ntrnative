#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_49_unk_struct_ov70_02261_e10_t.h"

typedef struct UnkStruct_ov49_0225DF18 {
    s16 x;
    s16 y;
} UnkStruct_ov49_0225DF18;

s32 ov49_022589D8(void *, u32, void *, s32 *, s32);

void *ov49_0225CC4C(s32, s32, enum HeapID, s32);

s32 ov49_0225CDEC(void *, s8, s8, enum HeapID, s32);

void *ov49_0225CF28(void *, int, int, s32 *);

s32 ov49_0225D040(void *, s32);

void *ov49_0225D098(void *, int, u32, u32);

s32 ov49_0225D1C4(void *, UnkStruct_ov49_0225DF18);

u16 ov49_0225D1EC(void *);

s32 ov49_0225D214(void *, void *, u8, s32);

s32 ov49_0225D494(void *, s32);

s32 ov49_0225EB00(void *, void *);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov49_0225EE4C(void *);
#else
s32 ov49_0225EE4C(void *);
#endif

extern s32 ov49_02269A88;

extern s32 ov49_02269AAC;

UnkStruct_ov70_02261E10_t *ov49_0225DF18(s8 arg0, s8 arg1, struct UnkStruct_ov70_0225C894_t *arg2, enum HeapID arg3, s32 arg4);

UnkStruct_ov70_02261E10_t *ov49_0225DF18(s8 arg0, s8 arg1, struct UnkStruct_ov70_0225C894_t *arg2, enum HeapID arg3, s32 arg4) {
    s32 sp28[3];
    u32 spC;
    s32 sp8;
    enum HeapID sp4;
    s32 *var_r5;
    s32 *var_r6;
    s32 temp_r0_4;
    s32 var_r7;
    u16 temp_r0_2;
    u32 temp_r0_3;
    u32 temp_r0_5;
    UnkStruct_ov70_02261E10_t *temp_r0;
    u8 sp1C[12];

    sp4 = arg3;
    temp_r0 = Heap_Alloc(arg3, sizeof(UnkStruct_ov70_02261E10_t));
    memset(temp_r0, 0, sizeof(UnkStruct_ov70_02261E10_t));
    temp_r0->unk_00 = arg2;
    temp_r0->unk_610 = arg0;
    temp_r0->unk_611 = arg1;
    temp_r0->unk_04 = ov49_0225CC4C(9, 0x80, sp4, arg4);
    ov49_0225CDEC(temp_r0->unk_04, arg1, arg0, arg3, arg4);
    var_r5 = &ov49_02269AAC;
    spC = 0;
loop_1:

    sp8 = 0;
    if (ov49_022589D8(temp_r0->unk_00, *(u16 *)var_r5, &sp1C[2], (s32 *)sp1C, 0) != 1) {

    } else {
loop_4:
        temp_r0->unk_08[temp_r0->unk_612] = ov49_0225D098(temp_r0->unk_04, *(u16 *)((u8 *)var_r5 + 2), *(u16 *)((u8 *)((s32 *)sp1C) + 2), *(u16 *)((s32 *)sp1C));
        temp_r0_2 = *(u16 *)((u8 *)var_r5 + 2);
        switch ((u32)temp_r0_2) {
        case 6:
        case 7:
        case 8:
            ov49_0225D214(temp_r0->unk_04, temp_r0->unk_08[temp_r0->unk_612], 0, 0);
            temp_r0->unk_22C.unk_00[*(u16 *)((u8 *)var_r5 + 2) - 6] = temp_r0->unk_08[temp_r0->unk_612];
            break;
        case 0:
            ov49_0225D214(temp_r0->unk_04, temp_r0->unk_08[temp_r0->unk_612], 0, 0);
            ov49_0225D214(temp_r0->unk_04, temp_r0->unk_08[temp_r0->unk_612], 1, 0);
            ov49_0225D214(temp_r0->unk_04, temp_r0->unk_08[temp_r0->unk_612], 2, 0);
            break;
        case 10:
            ov49_0225D214(temp_r0->unk_04, temp_r0->unk_08[temp_r0->unk_612], 0, 2);
            temp_r0->unk_22C.unk_0C[*(u16 *)var_r5 - 0x49] = temp_r0->unk_08[temp_r0->unk_612];
            break;
        case 11:
        case 12:
            ov49_0225D214(temp_r0->unk_04, temp_r0->unk_08[temp_r0->unk_612], 0, 2);
            break;
        case 13:
            ov49_0225EB00(&temp_r0->unk_22C.unk_1C[temp_r0->unk_22C.unk_3DC], temp_r0->unk_08[temp_r0->unk_612]);
            temp_r0->unk_22C.unk_3DC++;
            if (temp_r0->unk_22C.unk_3DC > 0x18U) {
                GF_AssertFail();
            }
            ov49_0225D494(temp_r0->unk_08[temp_r0->unk_612], 0);
            if ((u32)(u16)(*(u16 *)var_r5 + 0xFFA4) > 1U) {

            } else {
                ov49_0225EE4C(temp_r0->unk_08[temp_r0->unk_612]);
            }
            break;
        case 14:
            ov49_0225EB00(&temp_r0->unk_22C.unk_13C[temp_r0->unk_22C.unk_3DD], temp_r0->unk_08[temp_r0->unk_612]);
            temp_r0->unk_22C.unk_3DD++;
            if (temp_r0->unk_22C.unk_3DC > 0x18U) {
                GF_AssertFail();
            }
            ov49_0225D494(temp_r0->unk_08[temp_r0->unk_612], 0);
            if ((u32)(u16)(*(u16 *)var_r5 + 0xFFA4) <= 1U) {
                ov49_0225EE4C(temp_r0->unk_08[temp_r0->unk_612]);
            }
            break;
        case 15:
            temp_r0->unk_22C.unk_25C[temp_r0->unk_22C.unk_3DE] = temp_r0->unk_08[temp_r0->unk_612];
            temp_r0->unk_22C.unk_3DE++;
            if (temp_r0->unk_22C.unk_3DE > 0x18U) {
                GF_AssertFail();
            }
            ov49_0225D494(temp_r0->unk_08[temp_r0->unk_612], 0);
            break;
        case 16:
            ov49_0225EB00(&temp_r0->unk_22C.unk_2BC[temp_r0->unk_22C.unk_3DF], temp_r0->unk_08[temp_r0->unk_612]);
            temp_r0->unk_22C.unk_3DF++;
            if (temp_r0->unk_22C.unk_3DF > 0x18U) {
                GF_AssertFail();
            }
            ov49_0225D494(temp_r0->unk_08[temp_r0->unk_612], 0);
            temp_r0_3 = ov49_0225D1EC(temp_r0->unk_08[temp_r0->unk_612]);
            *(u16 *)((u8 *)((s32 *)sp1C) + 4) = temp_r0_3;
            *(u16 *)((u8 *)((s32 *)sp1C) + 6) = (u16)(temp_r0_3 >> 0x10);
            *(u16 *)((u8 *)((s32 *)sp1C) + 8) = (u16) * (u16 *)((u8 *)((s32 *)sp1C) + 4);
            *(u16 *)((u8 *)((s32 *)sp1C) + 0xA) = (u16) * (u16 *)((u8 *)((s32 *)sp1C) + 6);
            *(u16 *)((u8 *)((s32 *)sp1C) + 8) = (u16)((s16) * (u16 *)((u8 *)((s32 *)sp1C) + 8) + 8);
            *(u16 *)((u8 *)((s32 *)sp1C) + 0xA) = (u16)((s16) * (u16 *)((u8 *)((s32 *)sp1C) + 0xA) + 0x14);
            ov49_0225D1C4(temp_r0->unk_08[temp_r0->unk_612], *(UnkStruct_ov49_0225DF18 *)(sp1C + 8));
            break;
        }
        temp_r0->unk_612++;
        temp_r0_4 = sp8 + 1;
        sp8 = temp_r0_4;
        if (ov49_022589D8(temp_r0->unk_00, *(u16 *)var_r5, &sp1C[2], (s32 *)sp1C, temp_r0_4) == 1) {
            goto loop_4;
        }
    }
    var_r5 = (void *)((u8 *)var_r5 + (4));
    temp_r0_5 = spC + 1;
    spC = temp_r0_5;
    if (temp_r0_5 < 0x23U) {
        goto loop_1;
    }
    var_r7 = 0;
    var_r6 = &ov49_02269A88;
    sp28[0] = 0;
    sp28[2] = 10 << 14;
    sp28[1] = 0;
    do {
        temp_r0->unk_208[var_r7] = ov49_0225CF28(temp_r0->unk_04, *(u16 *)var_r6, *(u16 *)((u8 *)var_r6 + 2), sp28);
        ov49_0225D040(temp_r0->unk_208[var_r7], 0);
        var_r6 = (void *)((u8 *)var_r6 + (4));
        var_r7 += 1;
    } while (var_r7 < 9);
    temp_r0->unk_613 = 9;
    return temp_r0;
}
