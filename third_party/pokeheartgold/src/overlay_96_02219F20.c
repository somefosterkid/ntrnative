#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "pokeathlon/pokeathlon.h"
#include "heap.h"

typedef struct UnkStruct_ov96_02219F7C_Heap_Alloc {
    enum HeapID unk_0;
    PokeathlonCourseData *unk_4;
    u8 unk_8;
    u8 unk_9[0x23];
    u8 unk_2C;
    u8 unk_2D[0x3];
} UnkStruct_ov96_02219F7C_Heap_Alloc;

void ov96_02219F20(u8 *param_1, u32 param_2, s16 param_3, s16 param_4);

void ov96_02219F50(s32 param_1, s32 param_2, s32 param_3);

s32 ov96_0221A00C(void *, s32, PokeathlonCourseData *);
void *ov96_02219F7C(PokeathlonCourseData *arg0, enum HeapID arg1);

void ov96_02219F20(u8 *param_1, u32 param_2, s16 param_3, s16 param_4) {
    ManagedSprite_SetPositionXYWithSubscreenOffset(*(ManagedSprite **)(param_1 + param_2 * 4 + 0x60), param_3, param_4, FX32_CONST(480));
    ManagedSprite_SetPositionXYWithSubscreenOffset(*(ManagedSprite **)(param_1 + (param_2 * 4 + 0x68)), param_3, param_4 + -0x18, FX32_CONST(480));
    return;
}

void ov96_02219F50(s32 param_1, s32 param_2, s32 param_3) {
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)(param_1 + (param_2 + 2) * 4 + 0x60), (u32)(param_3 == 0));
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)(param_1 + (param_2 + 6) * 4 + 0x60), param_3);
    return;
}

void *ov96_02219F7C(PokeathlonCourseData *data, enum HeapID heapID) {
    PokeathlonCourseData *pokeathlonCourseData;
    s32 v0;
    u32 v1;
    UnkStruct_ov96_02219F7C_Heap_Alloc *dest;
    void *v2;

    pokeathlonCourseData = data;
    v1 = PokeathlonCourse_GetParticipantCount(data);
    dest = Heap_Alloc(heapID, sizeof(UnkStruct_ov96_02219F7C_Heap_Alloc));
    MI_CpuFill8(dest, 0U, sizeof(UnkStruct_ov96_02219F7C_Heap_Alloc));
    dest->unk_0 = heapID;
    v0 = 0;
    dest->unk_4 = data;
    dest->unk_2C = (u8)(4 - v1);
    if ((s32) dest->unk_2C > 0) {
        v2 = (void *)&dest->unk_8;
        do {
            ov96_0221A00C(v2, (u8)v1, pokeathlonCourseData);
            v0 += 1;
            v1 += 1;
            v2 += 0xC;
        } while (v0 < (s32) dest->unk_2C);
    }
    return dest;
}
