#include "global.h"

#include "pokeathlon/pokeathlon.h"

void ov96_021E786C(PokeathlonCourseData *param_1, u8 *param_2);

void ov96_021E786C(PokeathlonCourseData *param_1, u8 *param_2) {
    u16 *puVar1;
    u16 uVar2;
    BOOL index;
    s32 iVar5;
    u16 *puVar6;
    PokeathlonCourseData *pPVar7;
    u32 uVar8;

    puVar6 = (u16 *)(param_2 + (u32)param_1->args->filler_A[2] * 0x2c);
    uVar2 = *(u16 *)(param_1->fieldData[4].data + 0x52);
    if (uVar2 > puVar6[3]) {
        u16 *puVar3;
        puVar6[3] = uVar2;
        if (999 < puVar6[3]) {
            puVar6[3] = 999;
        }
        iVar5 = 0;
        puVar3 = puVar6;
        pPVar7 = param_1;
        do {
            uVar2 = *(u16 *)(pPVar7->fieldData[4].data + 8);
            *(u16 *)puVar3 = uVar2;
            if (999 < uVar2) {
                *(u16 *)puVar3 = 999;
            }
            iVar5 = iVar5 + 1;
            pPVar7 = (PokeathlonCourseData *)((u8 *)pPVar7->field_0 + 2);
            puVar3 = puVar3 + 1;
        } while (iVar5 < 3);
        index = ov96_021E5F24(param_1);
        {
            u16 *src = (u16 *)PokeathlonCourse_GetParticipantUnk04(param_1, index);
            iVar5 = 0;
            u32 *dst = (u32 *)(puVar6 + 4);
            do {
                uVar8 = *dst;
                uVar8 = (uVar8 & 0xFFFFFE00) | (*(u16 *)src & 0x1FF);
                iVar5 = iVar5 + 1;
                *dst = uVar8;
                uVar8 = (uVar8 & 0xFFFFC1FF) | ((src[1] & 0x1F) << 9);
                *dst = uVar8;
                uVar8 = (uVar8 & 0xFFFF3FFF) | ((*(u8 *)((u8 *)src + 0x11) & 3) << 14);
                *dst = uVar8;
                uVar8 = (uVar8 & 0xFFFEFFFF) | ((*(u8 *)((u8 *)src + 0x10) & 1) << 16);
                *dst = uVar8;
                puVar1 = src + 2;
                *(u32 *)(puVar6 + 6) = *(u32 *)puVar1;
                src = src + 0x14;
                dst = dst + 3;
                puVar6 = puVar6 + 6;
            } while (iVar5 < 3);
        }
    }
    return;
}

void ov96_021E7C04(u32 param_1, u32 *param_2, u8 *param_3);

void ov96_021E7C94(PokeathlonCourseData *param_1, u16 *param_2, u16 *param_3);

extern u8 _0221A7D8[];

void ov96_021E7938(PokeathlonCourseData *param_1, Pokeathlon_RecordsSolo *param_2);

void ov96_021E7938(PokeathlonCourseData *param_1, Pokeathlon_RecordsSolo *param_2) {
    u32 bVar1;
    u8 i;
    u8 j;
    u8 k;
    u32 id;
    Pokeathlon_RecordsSolo *e;
    PokeathlonMonEntry *pp;
    u16 sub[16];
    u16 buf[5][16];

    i = 0;
    do {
        id = param_1->field_3D8[i] & 0xff;
        e = &param_2[id];
        if (e->unk_28 < 9999999) {
            e->unk_28 = e->unk_28 + 1;
        }
        bVar1 = _0221A7D8[id];
        j = 0;
        do {
            buf[j][0] = e->entries[j].unk_0;
            k = 0;
            do {
                buf[j][k + 1] = e->entries[j].unk_2[k];
                k++;
            } while (k < 3);
            j++;
        } while (j < 5);
#if defined(SDK_PORT) || defined(NONMATCHING)
        sub[0] = ((u16 *)(param_1->fieldData[4].data + 0x28 + (ov96_021E5F24(param_1) & 0xff) * 8))[i];
#else
        sub[0] = *(u16 *)((u8 *)param_1 + (ov96_021E5F24(param_1) & 0xff) * 8 + i * 2 + 0x8d4);
#endif
        id = ov96_021E5F24(param_1) & 0xff;
        pp = param_1->participants[id].mons;
        k = 0;
        do {
            sub[k + 1] = (pp[k].unk_2 << 10) + pp[k].species;
            k++;
        } while (k < 3);
        ov96_021E7C04(bVar1, (u32 *)sub, (u8 *)buf);
        ov96_021E7C94(param_1, &e->entries[0].unk_0, buf[0]);
        i++;
    } while (i < 3);
}
