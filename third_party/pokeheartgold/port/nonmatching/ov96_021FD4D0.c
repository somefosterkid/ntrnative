#include "pokeathlon/pokeathlon.h"

typedef struct UnkSub_021FD4D0 { /* invented */
    f32 unk0;
    f32 unk4;
    u8 unk8[0xE];
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19[3];
} UnkSub_021FD4D0;

typedef struct UnkElem_021FD4D0 { /* invented */
    u8 unk00[0x18];
    UnkSub_021FD4D0 unk18[3];
    u8 unk6C[0x1F];
    u8 unk8B;
    VecFx32 unk8C;
    u8 unk98[5];
    u8 unk9D;
    u8 unk9E;
    u8 unk9F[4];
    u8 unkA3;
    u8 unkA4[5];
    u8 unkA9;
    u8 unkAA;
    u8 unkAB;
    s32 unkAC;
    VecFx32 unkB0;
    VecFx32 unkBC;
    u8 unkC8[8];
    u8 unkD0;
    u8 unkD1[3];
} UnkElem_021FD4D0;

typedef struct UnkFlags_021FD4D0 { /* invented */
    u32 unk0;
    u32 unk4;
    u32 unk8;
} UnkFlags_021FD4D0;

typedef struct UnkHeap_021FD4D0 { /* invented */
    u8 unk000[0x30];
    UnkElem_021FD4D0 unk030[4];
    UnkFlags_021FD4D0 unk380[4];
    u32 unk3B0[4];
    u8 unk3C0;
    u8 unk3C1;
    u8 unk3C2;
    u8 unk3C3[0x19];
    u32 unk3DC;
    u8 unk3E0[0x260];
    u16 unk640;
} UnkHeap_021FD4D0;

typedef struct UnkInput_021FD4D0 { /* invented */
    u32 unk0;
    u8 unk4;
    u8 unk5;
    u8 unk6[2];
    u8 unk8;
    u8 unk9[0x1B];
} UnkInput_021FD4D0;

void ov96_021E8228(PokeathlonCourseData *, u8, u8, u32, u32);
void ov96_021FDA30(UnkHeap_021FD4D0 *, u8 *);
BOOL ov96_021FDE08(UnkHeap_021FD4D0 *, u8, u8, u8);
u32 ov96_021FDE6C(UnkHeap_021FD4D0 *, u8, u8);
void ov96_021FDE7C(PokeathlonCourseData *);
f32 ov96_021FF6DC(UnkElem_021FD4D0 *, s32);
void ov96_021FF72C(const VecFx32 *, const VecFx32 *, VecFx32 *);
void ov96_021FFE5C(UnkHeap_021FD4D0 *, u8, u32);
void ov96_02200EF4(PokeathlonCourseData *, u32, u16);

void ov96_021FD4D0(PokeathlonCourseData *data) {
    s32 i;
    UnkHeap_021FD4D0 *heap;
    UnkInput_021FD4D0 *in;
    u8 *copy;
    BOOL reset;
    UnkElem_021FD4D0 *elem;
    f32 a;
    f32 b;
    f32 limit;
    f32 len;
    fx32 scale;
    fx32 mag;
    VecFx32 tmp;
    VecFx32 diff;

    copy = PokeathlonCourse_GetDataCopyArea(data);
    heap = PokeathlonCourse_GetHeapAllocPtr4(data);
    if (ov96_021E5F24(data)) {
        return;
    }
    if (heap->unk3C0) {
        ov96_021FDA30(heap, ov96_021E8A20(copy + 0x28));
        return;
    }
    *(UnkInput_021FD4D0 *)ov96_021E8A20(copy + 0x50) = *(UnkInput_021FD4D0 *)ov96_021E8A20(copy);
    if (heap->unk640 != 0) {
        heap->unk640--;
    }
    ov96_021E8A20(copy + 0x28);
    for (i = 0; i < 4; i++) {
        in = (UnkInput_021FD4D0 *)ov96_021E8A20(copy + 0x50 + i * 0x28);
        elem = &heap->unk030[i];
        if (in->unk0 != 0) {
            if (heap->unk380[i].unk0 != 0 && heap->unk380[i].unk4 != 0) {
                heap->unk380[i].unk0 = 0;
            } else if (heap->unk380[i].unk0 == 0 && heap->unk380[i].unk4 == 0) {
                heap->unk380[i].unk0 = 1;
                heap->unk380[i].unk4 = 1;
            }
        } else {
            heap->unk380[i].unk0 = 0;
            heap->unk380[i].unk4 = 0;
        }
        ov96_021FFE5C(heap, i, ov96_021FDE6C(heap, in->unk4, in->unk5));
        if (heap->unk030[i].unk9D) {
            if (heap->unk030[i].unk9E == 1 && in->unk8 == 1) {
                heap->unk030[i].unk8B = (heap->unk030[i].unk8B + 1) % 3;
                heap->unk030[i].unk9E = 2;
            } else if (heap->unk030[i].unk9E == 2 && in->unk8 == 2) {
                heap->unk030[i].unk9E = 0;
                heap->unk030[i].unk9D = 0;
                heap->unk030[i].unkA3 = 0;
            }
            heap->unk3B0[i] = 0;
            heap->unk030[i].unkAC = 0;
            heap->unk030[i].unkB0.x = 0;
            heap->unk030[i].unkB0.y = 0;
            heap->unk030[i].unkB0.z = 0;
            heap->unk030[i].unkBC = heap->unk030[i].unkB0;
        } else if (heap->unk030[i].unk18[heap->unk030[i].unk8B].unk18 == 2) {
            heap->unk3B0[i] = 0;
            heap->unk030[i].unkAC = 0;
            heap->unk030[i].unkB0.x = 0;
            heap->unk030[i].unkB0.y = 0;
            heap->unk030[i].unkB0.z = 0;
            heap->unk030[i].unkBC = heap->unk030[i].unkB0;
        } else if (heap->unk030[i].unkA9) {
            heap->unk3B0[i] = 0;
            heap->unk030[i].unkAC = 0;
            heap->unk030[i].unkB0.x = 0;
            heap->unk030[i].unkB0.y = 0;
            heap->unk030[i].unkB0.z = 0;
            heap->unk030[i].unkBC = heap->unk030[i].unkB0;
        } else {
            reset = FALSE;
            if (heap->unk380[i].unk0 != 0) {
                if (ov96_021FDE6C(heap, in->unk4, in->unk5)) {
                    heap->unk030[i].unk9D = 1;
                    heap->unk030[i].unk9E = 1;
                    ov96_021E8228(data, heap->unk030[i].unkD0, heap->unk030[i].unk8B, 7, 1);
                } else if (ov96_021FDE08(heap, i, in->unk4, in->unk5)) {
                    heap->unk3B0[i] = 1;
                    heap->unk030[i].unkB0.x = in->unk4 << 12;
                    heap->unk030[i].unkB0.y = in->unk5 << 12;
                    heap->unk030[i].unkB0.z = 0;
                    heap->unk030[i].unkBC = heap->unk030[i].unkB0;
                }
            } else if (heap->unk380[i].unk4 != 0) {
                if (heap->unk030[i].unkAC < 0xFF) {
                    heap->unk030[i].unkAC++;
                } else {
                    heap->unk3C2 = 1;
                }
                if (heap->unk3B0[i] == 0) {
                    continue;
                }
                heap->unk030[i].unkBC.x = in->unk4 << 12;
                heap->unk030[i].unkBC.y = in->unk5 << 12;
            } else {
                if (heap->unk3B0[i] != 0) {
                    if (heap->unk030[i].unkAC <= 30) {
                        heap->unk030[i].unkAA = 1;
                    }
                    heap->unk3B0[i] = 0;
                    heap->unk3C2 = 0;
                }
                reset = TRUE;
            }
            if (heap->unk030[i].unkAA) {
                VecFx32 zero = { 0, 0, 0 };
                a = heap->unk030[i].unk18[heap->unk030[i].unk8B].unk0;
                b = heap->unk030[i].unk18[heap->unk030[i].unk8B].unk4;
                limit = ov96_021FF6DC(elem, heap->unk030[i].unk8B);
                scale = (a * b) / 8.0 * 4096.0;
                VEC_Subtract(&heap->unk030[i].unkBC, &heap->unk030[i].unkB0, &diff);
                if (diff.x < 0) {
                    if (diff.x < -0x10000) {
                        heap->unk030[i].unk9D = 1;
                        heap->unk030[i].unk9E = 1;
                        ov96_021E8228(data, heap->unk030[i].unkD0, heap->unk030[i].unk8B, 7, 1);
                    }
                    diff.x = 0;
                    diff.y = 0;
                    diff.z = 0;
                }
                VEC_MultAdd(scale, &diff, &zero, &diff);
                len = VEC_Mag(&diff) / FX32_ONE;
                tmp = heap->unk030[i].unk8C;
                ov96_021FF72C(&tmp, &diff, &tmp);
                len = VEC_Mag(&tmp);
                if (len > 4096.0f * limit) {
                    VecFx32 zero2 = { 0, 0, 0 };
                    VEC_Normalize(&tmp, &tmp);
                    VEC_MultAdd(4096.0f * limit, &tmp, &zero2, &heap->unk030[i].unk8C);
                } else {
                    ov96_021FF72C(&heap->unk030[i].unk8C, &diff, &heap->unk030[i].unk8C);
                }
                mag = VEC_Mag(&elem->unk8C);
                if (mag >= (elem->unk18[elem->unk8B].unk16 - 3) * FX32_ONE) {
                    ov96_021E8228(data, elem->unkD0, elem->unk8B, 6, 1);
                }
                heap->unk030[i].unkAA = 0;
            }
            if (reset) {
                heap->unk030[i].unkAC = 0;
                heap->unk030[i].unkB0.x = 0;
                heap->unk030[i].unkB0.y = 0;
                heap->unk030[i].unkB0.z = 0;
                heap->unk030[i].unkBC = heap->unk030[i].unkB0;
            }
        }
    }
    ov96_02200EF4(data, heap->unk3DC, heap->unk640);
    ov96_021FDE7C(data);
    if (heap->unk640 == 0) {
        heap->unk3C0 = TRUE;
    } else {
        heap->unk3C0 = FALSE;
    }
    ov96_021FDA30(heap, ov96_021E8A20(copy + 0x28));
}
