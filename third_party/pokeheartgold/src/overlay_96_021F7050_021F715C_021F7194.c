#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

s32 ov96_021E8228(PokeathlonCourseData *, u8, u8, s32, s32);

void ov96_021EAF78(void *, s32, s32, s32 *, s32 *, s32 *);

void ov96_021EB0A4(void *, s32, s32, s32 *, s32 *);

u32 ov96_021F6060(VecFx32 *, s32, VecFx32 *, s32);

s32 sub_0200606C(s32, s32);

extern u8 ov96_0221DC18[4];

s32 ov96_021F7194(PokeathlonCourseData *arg0, UnkStruct_ov96_021F5018 *arg1);

s32 ov96_021F7194(PokeathlonCourseData *arg0, UnkStruct_ov96_021F5018 *arg1) {
    VecFx32 posA;
    VecFx32 posB;
    s32 radA;
    s32 radB;
    s32 outX;
    s32 outY;
    s32 *pY;
    u8 iByte;
    void *valA;
    void *valB;
    u8 r18;
    UnkStruct_ov96_021F5018_Row *rowJ;
    UnkStruct_ov96_021F5018_Row *rowI;
    s32 i;
    UnkStruct_ov96_021F5018 *base;
    PokeathlonCourseData *data;
    UnkStruct_ov96_021F5018_Ball *pi;
    UnkStruct_ov96_021F5018_Ball *pj;
    s32 j;
    s32 tmp;

    data = arg0;
    base = arg1;
    i = 0;
    pi = arg1->unk_90;
    rowI = base->unk_FAC;
    do {
        rowJ = base->unk_FAC;
        j = 0;
        iByte = i;
        pj = base->unk_90;
        pY = &pi->unk_1C;
        do {
            if (i != j) {
                r18 = ov96_021E5F24(data);
                valA = pi->unk_00;
                valB = pj->unk_00;
                ov96_021EB0A4(valA, pi->unk_1C / 0x1000, (0x120000 - (rowI->unk_08 << 6)) / 0x1000, &outX, &outY);
                ov96_021EAF78(valA, outX << 12, outY << 12, &posA.x, &posA.y, &radA);
                posA.z = 0;
                ov96_021EB0A4(valB, pj->unk_1C / 0x1000, (0x120000 - (rowJ->unk_08 << 6)) / 0x1000, &outX, &outY);
                ov96_021EAF78(valB, outX << 12, outY << 12, &posB.x, &posB.y, &radB);
                posB.z = 0;
                if (ov96_021F6060(&posA, radA << 12, &posB, radB << 12) == 0) {
                    pi->unk_29[j] = 0;
                    pj->unk_29[i] = 0;
                } else if (pj->unk_29[i] == 0) {
                    pi->unk_26 = 1;
                    pj->unk_26 = 1;
                    pi->unk_27 = (30 * pi->unk_30) / 30;
                    pj->unk_27 = (30 * pj->unk_30) / 30;
                    {
                        VecFx32 hit = { 0, 0, 0 };
                        VEC_Subtract(&posB, &posA, &hit);
                        VEC_Normalize(&hit, &hit);
                        VEC_MultAdd(radA << 12, &hit, &posA, &hit);
                        base->unk_144[base->unk_143] = hit;
                    }
                    base->unk_168[base->unk_143] = 1;
                    base->unk_143 = (base->unk_143 + 1) % 3;
                    sub_0200606C(0x8A3, ov96_0221DC18[3]);
                    ov96_021E8228(data, r18, iByte, 4, 1);
                    ov96_021E8228(data, r18, j, 4, 1);
                    ov96_021E8228(data, r18, iByte, 1, 1);
                    ov96_021E8228(data, r18, j, 1, 1);
                    if (rowI->unk_10 != 0 && rowJ->unk_10 != 0 && rowI->unk_08 > 0 && rowJ->unk_08 > 0) {
                        if (pi->unk_29[j] == 0 && pj->unk_29[i] == 0) {
                            pi->unk_29[j] = 1;
                            pj->unk_29[i] = 1;
                            tmp = rowJ->unk_00;
                            rowJ->unk_00 = rowI->unk_00;
                            rowI->unk_00 = tmp;
                            tmp = pj->unk_18;
                            pj->unk_18 = pi->unk_18;
                            pi->unk_18 = tmp;
                            if (rowI->unk_00 < 0) {
                                rowI->unk_00 = 0;
                            }
                            if (rowJ->unk_00 < 0) {
                                rowJ->unk_00 = 0;
                            }
                        }
                    } else {
                        if (pi->unk_1C <= pj->unk_1C) {
                            *pY -= 0x10000;
                            pj->unk_1C += 0x10000;
                        } else {
                            *pY += 0x10000;
                            pj->unk_1C -= 0x10000;
                        }
                        if (pi->unk_1C < 0x20000) {
                            pi->unk_1C = 0x20000;
                        } else if (pi->unk_1C > 0xDF000) {
                            pi->unk_1C = 0xDF000;
                        }
                        if (pj->unk_1C < 0x20000) {
                            pj->unk_1C = 0x20000;
                        } else if (pj->unk_1C > 0xDF000) {
                            pj->unk_1C = 0xDF000;
                        }
                    }
                }
            }
            j++;
            pj++;
            rowJ++;
        } while (j < 3);
        pi++;
        rowI++;
        i++;
    } while (i < 3);
    return 0;
}
