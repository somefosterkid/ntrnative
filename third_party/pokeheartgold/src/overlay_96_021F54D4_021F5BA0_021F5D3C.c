#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

void *ov96_021E60C0(PokeathlonCourseData *, s32, s32);

s32 ov96_021EAC0C(s32 *, s32);

s32 ov96_021EAD08(s32 *, s32);

s32 ov96_021EAF94(void *, s32, s32);

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(void *, s32);

s32 ov96_021EB588(void *, s32 *);

s32 sub_0200606C(s32, s32);

void *sub_02006190(int);

extern u8 ov96_0221DC18;

void ov96_021F5D3C(PokeathlonCourseData *arg0);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021F5D3C(PokeathlonCourseData *arg0) {
    s32 i;
    s32 a;
    s32 b;
    UnkStruct_ov96_021F5018 *heap;
    UnkStruct_ov96_021F5018_Ball *ball;
    VecFx32 *pos;
    u8 *flag;

    heap = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    ball = heap->unk_90;
    i = 0;
    flag = &ov96_0221DC18;
    do {
        ball->unk_0C = 0x120000 - (heap->unk_FAC[i].unk_08 << 6);
        ball->unk_08 = ball->unk_1C;
        if (ball->unk_08 < 0x20000) {
            ball->unk_08 = 0x20000;
        } else if (ball->unk_08 > 0xDF000) {
            ball->unk_08 = 0xDF000;
        }
        heap->unk_FA0[i] = (u8)((ball->unk_08 / 4096 + ((u32)(ball->unk_08 / 4096) >> 31)) >> 1);
        a = ball->unk_08 / 4096;
        b = ball->unk_0C / 4096;
        ov96_021EAF94(ball->unk_00, a, b);
        s32 vec[3] = { 0, 0, 0 };
        vec[0] = a << 12;
        vec[1] = b << 12;
        if (((u8 *)ov96_021E60C0(arg0, ov96_021E5F24(arg0), i))[5] != 0) {
            vec[1] -= 0x18000;
        } else {
            vec[1] -= 0x10000;
        }
        ov96_021EB588(ball->unk_04, vec);
        if (ball->unk_26 != 0) {
            ov96_021EB52C(ball->unk_04, 1, 1);
            ov96_021EAD08(ball->unk_00, 0x14);
            if (sub_02006190(*flag) == 0) {
                sub_0200606C(0x890, *flag);
            }
        } else {
            ov96_021EB52C(ball->unk_04, 1, 0);
            ov96_021EAD08(ball->unk_00, 0);
            if (heap->unk_FAC[i].unk_14 != 0) {
                ov96_021EAC0C(ball->unk_00, 1);
            } else {
                ov96_021EAC0C(ball->unk_00, 2);
            }
        }
        ball++;
        flag += 1;
        i++;
    } while (i < 3);
    pos = heap->unk_144;
    i = 0;
    do {
        if (heap->unk_168[i] != 0) {
            heap->unk_168[i] = 0;
            ov96_021EB52C(heap->unk_78[i], 1, 1);
            ov96_021EB588(heap->unk_78[i], (s32 *)pos);
            ov96_021EB564(heap->unk_78[i], 9);
        }
        i++;
        pos++;
    } while (i < 3);
}
#else
void ov96_021F5D3C(PokeathlonCourseData *arg0) {
    s32 i;
    s32 a;
    s32 b;
    u8 *heap;
    u8 *ptr4;
    u8 *ptr6;
    u8 *ptr7;
    u8 *flag;

    heap = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    ptr4 = heap;
    i = 0;
    flag = &ov96_0221DC18;
    ptr4 += 0x90;
    ptr6 = heap;
    ptr7 = ptr6;
    do {
        *(s32 *)(ptr4 + 0xC) = 0x120000 - (*(s32 *)(ptr6 + 0xFB4) << 6);
        *(s32 *)(ptr4 + 8) = *(s32 *)(ptr4 + 0x1C);
        if (*(s32 *)(ptr4 + 8) < 0x20000) {
            *(s32 *)(ptr4 + 8) = 0x20000;
        } else if (*(s32 *)(ptr4 + 8) > 0xDF000) {
            *(s32 *)(ptr4 + 8) = 0xDF000;
        }
        *(u32 *)(ptr7 + 0xFA0) = (u8)((*(s32 *)(ptr4 + 8) / 4096 + ((u32)(*(s32 *)(ptr4 + 8) / 4096) >> 31)) >> 1);
        a = *(s32 *)(ptr4 + 8) / 4096;
        b = *(s32 *)(ptr4 + 0xC) / 4096;
        ov96_021EAF94(*(void **)ptr4, a, b);
        s32 vec[3] = { 0, 0, 0 };
        vec[0] = a << 12;
        vec[1] = b << 12;
        if (((u8 *)ov96_021E60C0(arg0, ov96_021E5F24(arg0), i))[5] != 0) {
            vec[1] -= 0x18000;
        } else {
            vec[1] -= 0x10000;
        }
        ov96_021EB588(*(void **)(ptr4 + 4), vec);
        if (*(u8 *)(ptr4 + 0x26) != 0) {
            ov96_021EB52C(*(void **)(ptr4 + 4), 1, 1);
            ov96_021EAD08(*(void **)ptr4, 0x14);
            if (sub_02006190(*flag) == 0) {
                sub_0200606C(0x890, *flag);
            }
        } else {
            ov96_021EB52C(*(void **)(ptr4 + 4), 1, 0);
            ov96_021EAD08(*(void **)ptr4, 0);
            if (*(s32 *)(ptr6 + 0xFC0) != 0) {
                ov96_021EAC0C(*(void **)ptr4, 1);
            } else {
                ov96_021EAC0C(*(void **)ptr4, 2);
            }
        }
        ptr4 += 0x38;
        ptr6 += 0x1C;
        ptr7 += 4;
        flag += 1;
        i++;
    } while (i < 3);
    ptr4 = heap;
    ptr6 = heap + 0x144;
    i = 0;
    do {
        if (*(u8 *)(heap + i + 0x168) != 0) {
            *(u8 *)(heap + i + 0x168) = 0;
            ov96_021EB52C(*(void **)(ptr4 + 0x78), 1, 1);
            ov96_021EB588(*(void **)(ptr4 + 0x78), (s32 *)ptr6);
            ov96_021EB564(*(void **)(ptr4 + 0x78), 9);
        }
        i++;
        ptr4 += 4;
        ptr6 += 0xC;
    } while (i < 3);
}
#endif
