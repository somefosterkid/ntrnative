#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

u32 _d2f(void);

void _dsub(u64, u32, u32);

u64 _f2d(u32);

fx32 _ffix(void);

void _fmul(u32, u32);

void ov96_021EB52C(s16 *param_1, s32 param_2, s32 param_3);

struct Sprite *ov96_021EB5B8(const SaveData *param_1);

void ov96_021FAFFC(UnkStruct_ov96_021F94A8 *param_1, u8 *param_2);

void ov96_021FB994(UnkStruct_ov96_021F94A8 *param_1);

void ov96_021FB0F4(PokeathlonCourseData *param_1);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021FB0F4(PokeathlonCourseData *param_1) {
    u8 *puVar3;
    UnkStruct_ov96_021F94A8 *heap;
    Sprite *pSVar4;
    VecFx32 *pVVar8;
    u8 iVar10;
    u8 iVar6;
    u8 uVar7;
    u8 uVar11;
    u8 i;
    UnkStruct_ov96_021FAB24 *base;
    UnkStruct_ov96_021FAB24 *elem;
    VecFx32 VStack_20;
    VecFx32 VStack_2c;
    VecFx32 VStack_38;
    VecFx32 VStack_44;

    heap = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    puVar3 = ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(param_1) + 0xf0);
    if (heap->unk_3C7 != 0) {
        heap->unk_3C4 = heap->unk_3C4 + 1;
    }
    if (heap->unk_3C6 == 0) {
        if (heap->unk_3C4 >= 0x96) {
            heap->unk_3C4 = 0;
            heap->unk_3C6 = 1;
            ov96_021FAFFC(heap, (u8 *)puVar3);
            for (i = 0; i < 3; i++) {
                elem = &heap->rows[i];
                pSVar4 = ov96_021EB5B8(elem->unk_48);
                ov96_021EB52C(elem->unk_48, 1, 1);
                iVar10 = (u8)((i + ov96_021E5F24(param_1) * 3) / 2);
                iVar6 = (u8)((i + ov96_021E5F24(param_1) * 3) % 2);
                uVar7 = (puVar3[iVar10 + 0x1c] >> (iVar6 << 2)) & 0xf;
                if ((uVar7 == 0) || (uVar7 > 0xc)) {
                    GF_AssertFail();
                }
                elem->unk_64 = 1.5f;
                VStack_20.x = 4096.0f * elem->unk_64;
                VStack_20.y = 4096.0f * elem->unk_64;
                VStack_20.z = 4096.0f * elem->unk_64;
                elem->unk_68 = uVar7;
                Sprite_SetAffineScale(pSVar4, &VStack_20);
                Sprite_SetAnimCtrlSeq(pSVar4, uVar7 - 1);
            }
        }
    } else if (heap->unk_3C4 >= 0x1e) {
        heap->unk_3C4 = 0;
        ov96_021FAFFC(heap, (u8 *)puVar3);
        uVar11 = 0;
        do {
            base = &heap->rows[uVar11];
            pSVar4 = ov96_021EB5B8(base->unk_48);
            iVar10 = (u8)((uVar11 + ov96_021E5F24(param_1) * 3) / 2);
            iVar6 = (u8)((uVar11 + ov96_021E5F24(param_1) * 3) % 2);
            uVar7 = (puVar3[iVar10 + 0x1c] >> (iVar6 << 2)) & 0xf;
            if ((uVar7 == 0) || (uVar7 > 0xc)) {
                GF_AssertFail();
            }
            if (base->unk_8 != 0) {
                pVVar8 = Sprite_GetMatrixPtr(pSVar4);
                VStack_2c = *pVVar8;
                VStack_2c.y = 0x138000;
                Sprite_SetMatrix(pSVar4, &VStack_2c);
                Sprite_SetAnimCtrlSeq(pSVar4, uVar7 + 0xb);
            }
            if (uVar7 != base->unk_68) {
                base->unk_68 = uVar7;
                if (base->unk_8 == 0) {
                    base->unk_64 = 1.5f;
                    VStack_38.x = 4096.0f * base->unk_64;
                    VStack_38.y = 4096.0f * base->unk_64;
                    VStack_38.z = 4096.0f * base->unk_64;
                    Sprite_SetAffineScale(pSVar4, &VStack_38);
                    Sprite_SetAnimCtrlSeq(pSVar4, uVar7 - 1);
                }
            }
            uVar11 = uVar11 + 1;
        } while (uVar11 < 3);
    }
    for (uVar11 = 0; uVar11 < 3; uVar11++) {
        elem = &heap->rows[uVar11];
        pSVar4 = ov96_021EB5B8(elem->unk_48);
        pVVar8 = Sprite_GetScalePtr(pSVar4);
        if (pVVar8->x > 0x1000) {
            elem->unk_64 = (float)((double)elem->unk_64 - 0.1);
        } else {
            elem->unk_64 = 1.0f;
        }
        VStack_44.x = 4096.0f * elem->unk_64;
        VStack_44.y = 4096.0f * elem->unk_64;
        VStack_44.z = 4096.0f * elem->unk_64;
        Sprite_SetAffineScale(pSVar4, &VStack_44);
    }
    ov96_021FB994(heap);
    return;
}
#else
// DS form: rows are reached as heap + i * 0x6C + (0xE0 + row offset): 0x128 == rows[0].unk_48, 0x144 == unk_64,
// 0x148 == unk_68, 0xE8 == unk_8; the 1.5f / 1.0f scales are stored as their bit patterns.
void ov96_021FB0F4(PokeathlonCourseData *param_1) {
    u8 *puVar3;
    u8 *heap;
    Sprite *pSVar4;
    VecFx32 *pVVar8;
    u8 iVar10;
    u8 iVar6;
    u8 uVar7;
    u8 uVar11;
    u8 i;
    u8 *base;
    u8 *elem;
    VecFx32 VStack_20;
    VecFx32 VStack_2c;
    VecFx32 VStack_38;
    VecFx32 VStack_44;

    heap = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    puVar3 = ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(param_1) + 0xf0);
    if (heap[0x3c7] != 0) {
        ((UnkStruct_ov96_021F94A8 *)heap)->unk_3C4 = ((UnkStruct_ov96_021F94A8 *)heap)->unk_3C4 + 1;
    }
    if (heap[0x3c6] == 0) {
        if (((UnkStruct_ov96_021F94A8 *)heap)->unk_3C4 >= 0x96) {
            ((UnkStruct_ov96_021F94A8 *)heap)->unk_3C4 = 0;
            heap[0x3c6] = 1;
            ov96_021FAFFC((UnkStruct_ov96_021F94A8 *)heap, (u8 *)puVar3);
            for (i = 0; i < 3; i++) {
                elem = heap + i * 0x6c;
                pSVar4 = ov96_021EB5B8(*(void **)(elem + 0x128));
                ov96_021EB52C(*(s16 **)(elem + 0x128), 1, 1);
                iVar10 = (u8)((i + ov96_021E5F24(param_1) * 3) / 2);
                iVar6 = (u8)((i + ov96_021E5F24(param_1) * 3) % 2);
                uVar7 = (puVar3[iVar10 + 0x1c] >> (iVar6 << 2)) & 0xf;
                if ((uVar7 == 0) || (uVar7 > 0xc)) {
                    GF_AssertFail();
                }
                *(u32 *)(elem + 0x144) = 0x3fc00000;
                VStack_20.x = 4096.0f * *(float *)(elem + 0x144);
                VStack_20.y = 4096.0f * *(float *)(elem + 0x144);
                VStack_20.z = 4096.0f * *(float *)(elem + 0x144);
                *(s16 *)(elem + 0x148) = uVar7;
                Sprite_SetAffineScale(pSVar4, &VStack_20);
                Sprite_SetAnimCtrlSeq(pSVar4, uVar7 - 1);
            }
        }
    } else if (((UnkStruct_ov96_021F94A8 *)heap)->unk_3C4 >= 0x1e) {
        ((UnkStruct_ov96_021F94A8 *)heap)->unk_3C4 = 0;
        ov96_021FAFFC((UnkStruct_ov96_021F94A8 *)heap, (u8 *)puVar3);
        uVar11 = 0;
        do {
            base = heap + uVar11 * 0x6c;
            pSVar4 = ov96_021EB5B8(*(void **)(base + 0x128));
            iVar10 = (u8)((uVar11 + ov96_021E5F24(param_1) * 3) / 2);
            iVar6 = (u8)((uVar11 + ov96_021E5F24(param_1) * 3) % 2);
            uVar7 = (puVar3[iVar10 + 0x1c] >> (iVar6 << 2)) & 0xf;
            if ((uVar7 == 0) || (uVar7 > 0xc)) {
                GF_AssertFail();
            }
            if (base[0xe8] != 0) {
                pVVar8 = Sprite_GetMatrixPtr(pSVar4);
                VStack_2c = *pVVar8;
                VStack_2c.y = 0x138000;
                Sprite_SetMatrix(pSVar4, &VStack_2c);
                Sprite_SetAnimCtrlSeq(pSVar4, uVar7 + 0xb);
            }
            if (uVar7 != *(s16 *)(base + 0x148)) {
                *(s16 *)(base + 0x148) = uVar7;
                if (base[0xe8] == 0) {
                    *(u32 *)(base + 0x144) = 0x3fc00000;
                    VStack_38.x = 4096.0f * *(float *)(base + 0x144);
                    VStack_38.y = 4096.0f * *(float *)(base + 0x144);
                    VStack_38.z = 4096.0f * *(float *)(base + 0x144);
                    Sprite_SetAffineScale(pSVar4, &VStack_38);
                    Sprite_SetAnimCtrlSeq(pSVar4, uVar7 - 1);
                }
            }
            uVar11 = uVar11 + 1;
        } while (uVar11 < 3);
    }
    for (uVar11 = 0; uVar11 < 3; uVar11++) {
        elem = heap + uVar11 * 0x6c;
        pSVar4 = ov96_021EB5B8(*(void **)(elem + 0x128));
        pVVar8 = Sprite_GetScalePtr(pSVar4);
        if (pVVar8->x > 0x1000) {
            *(float *)(elem + 0x144) = (float)((double)*(float *)(elem + 0x144) - 0.1);
        } else {
            *(u32 *)(elem + 0x144) = 0x3f800000;
        }
        VStack_44.x = 4096.0f * *(float *)(elem + 0x144);
        VStack_44.y = 4096.0f * *(float *)(elem + 0x144);
        VStack_44.z = 4096.0f * *(float *)(elem + 0x144);
        Sprite_SetAffineScale(pSVar4, &VStack_44);
    }
    ov96_021FB994((UnkStruct_ov96_021F94A8 *)heap);
    return;
}
#endif
