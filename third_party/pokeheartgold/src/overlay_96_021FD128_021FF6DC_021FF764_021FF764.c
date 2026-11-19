#include "global.h"

#include "error_handling.h"

struct UnkStruct_ov96_021FF764;

typedef struct UnkStruct_ov96_021FF764 {
    u8 unk_00[0x7C];
    s32 unk_7C;
    s32 unk_80;
    u8 unk_84[7];
    u8 unk_8B;
    VecFx32 unk_8C;
    u8 unk_98[4];
    u8 unk_9C;
    u8 unk_9D;
    u8 unk_9E[6];
    u8 unk_A4;
    u8 unk_A5[4];
    u8 unk_A9;
    u8 unk_AA[0x26];
    u8 unk_D0;
    u8 unk_D1[3];
} UnkStruct_ov96_021FF764;

void ov96_021EB0A4(void *obj, s32 x, s32 y, s32 *outX, s32 *outY);

void ov96_021EAF78(void *obj, s32 x, s32 y, s32 *outX, s32 *outY, s32 *outRadius);

void ov96_021E8228(void *ctx, u32 param_2, u32 param_3, u32 param_4, u32 param_5);

u32 ov96_021FF764(void *param_1, UnkStruct_ov96_021FF764 *param_2, VecFx32 *param_3);

u32 ov96_021FF764(void *param_1, UnkStruct_ov96_021FF764 *param_2, VecFx32 *param_3) {
    UnkStruct_ov96_021FF764 *entryI;
    UnkStruct_ov96_021FF764 *entryJ;
    void *objI;
    void *objJ;
    u32 result;
    u32 savedId;
    s32 xIOriginal;
    s32 i;
    u32 idI;
    s32 j;
    s32 radiusI;
    s32 radiusJ;
    s32 xI;
    s32 yI;
    s32 xJ;
    VecFx32 posI;
    VecFx32 posJ;
    VecFx32 diff;
    s32 yJ;

    result = 0;
    posI.z = 0;
    posJ.z = 0;
    i = 0;
    entryI = param_2;
    do {
        idI = entryI->unk_8B;
        if (entryI->unk_9D != 0 || entryI->unk_A9 != 0) {
            goto next_i;
        }
        objI = (*(void **)((u8 *)(entryI) + 4 * (entryI)->unk_8B));
        ov96_021EB0A4(objI, entryI->unk_7C / 4096, entryI->unk_80 / 4096, &xI, &yI);
        entryJ = param_2;
        for (j = 0; j < 4; j++, entryJ++) {
            s32 curX;

            if (i == j) {
                continue;
            }
            if (entryJ->unk_9D != 0) {
                continue;
            }
            curX = xI;
            objJ = (*(void **)((u8 *)(entryJ) + 4 * (entryJ)->unk_8B));
            ov96_021EB0A4(objJ, entryJ->unk_7C / 4096, entryJ->unk_80 / 4096, &xJ, &yJ);
            xIOriginal = entryI->unk_7C;
            if ((double)xIOriginal > 3145728.0 && entryJ->unk_7C < 0x100000) {
                xJ += 1024.0;
            } else if ((double)entryJ->unk_7C > 3145728.0 && xIOriginal < 0x100000) {
                curX += 1024.0;
            }
            ov96_021EAF78(objJ, xJ << 12, yJ << 12, &posJ.x, &posJ.y, &radiusJ);
            ov96_021EAF78(objI, curX << 12, yI << 12, &posI.x, &posI.y, &radiusI);
            VEC_Subtract(&posJ, &posI, &diff);
            if (VEC_Mag(&diff) < (radiusJ + radiusI) * 4096) {
                if (entryI->unk_98[j] == 0) {
                    VecFx32 zero = { 0, 0, 0 };

                    if (VEC_DotProduct(&diff, &entryI->unk_8C) > 0) {
                        if (VEC_Mag(&entryI->unk_8C) != 0) {
                            savedId = entryI->unk_8B;
                            VEC_MultAdd((s32)(4096.0f * *(float *)(((u8 *)(entryI) + 0x1c * (savedId)) + 0x20)), &entryI->unk_8C, &zero, &entryJ->unk_8C);
                            if (VEC_Mag(&entryJ->unk_8C) > 0xB000) {
                                VecFx32 zero2 = { 0, 0, 0 };

                                VEC_Normalize(&entryJ->unk_8C, &entryJ->unk_8C);
                                VEC_MultAdd(0xB000, &entryJ->unk_8C, &zero2, &entryJ->unk_8C);
                            }
                        }
                        {
                            u8 *record = ((u8 *)(entryJ) + 0x1c * (idI));

                            if (record[0x30] == 1) {
                                *(u32 *)(record + 0x24) = 0;
                                record[0x30] = 2;
                                entryJ->unk_A4 = record[0x2F];
                                ov96_021E8228(param_1, entryJ->unk_D0, entryJ->unk_8B, 1, 1);
                            }
                        }
                        {
                            VecFx32 zero3 = { 0, 0, 0 };

                            entryI->unk_8C = zero3;
                        }
                        {
                            u8 *record = ((u8 *)(entryI) + 0x1c * (savedId));

                            if (record[0x30] == 1) {
                                *(u32 *)(record + 0x24) = 0;
                                record[0x30] = 2;
                                entryI->unk_A4 = record[0x2F];
                                ov96_021E8228(param_1, entryI->unk_D0, entryI->unk_8B, 1, 1);
                            }
                        }
                        if (entryI->unk_98[j] != 0) {
                            GF_AssertFail();
                        }
                        if (entryJ->unk_98[i] != 0) {
                            GF_AssertFail();
                        }
                        entryJ->unk_98[j] = 1;
                        entryI->unk_98[i] = 1;
                        if (result == 0) {
                            VecFx32 out = { 0, 0, 0 };

                            result = 1;
                            VEC_Subtract(&posJ, &posI, &out);
                            VEC_Normalize(&out, &out);
                            VEC_MultAdd(radiusI << 12, &out, &posI, &out);
                            if ((double)out.x >= 4194304.0) {
                                out.x = (s32)((double)out.x - 4194304.0);
                            }
                            *param_3 = out;
                        }
                        entryJ->unk_A9 = 6;
                        entryI->unk_A9 = 0x12;
                        ov96_021E8228(param_1, entryJ->unk_D0, entryJ->unk_8B, 4, 1);
                        ov96_021E8228(param_1, entryI->unk_D0, entryI->unk_8B, 4, 1);
                        break;
                    }
                }
            } else {
                entryI->unk_98[j] = 0;
                entryJ->unk_98[i] = 0;
            }
        }
next_i:
        entryI++;
        i++;
    } while (i < 4);
    return result;
}
