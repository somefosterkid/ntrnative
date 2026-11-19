#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "pokeathlon/pokeathlon.h"
#include "heap.h"

typedef struct UnkStruct_ov96_022107F0_Heap_Alloc {
    enum HeapID unk_0;
    PokeathlonCourseData *unk_4;
    u8 unk_8;
    u8 unk_9[0xB3];
    u8 unk_BC;
    u8 unk_BD[0x3];
} UnkStruct_ov96_022107F0_Heap_Alloc;

#include "overlay_96_app_0221040c.h"

s32 ov96_022108BC(void *, s32, PokeathlonCourseData *);
void *ov96_022107F0(PokeathlonCourseData *arg0, enum HeapID arg1);

void ov96_022107D4(Ov96App_0221040C *param_1) {
    s32 iVar1;

    iVar1 = 0;
    do {
        ManagedSprite_SetAnimateFlag(param_1->sprites[iVar1], 0);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 0x1f);
    return;
}

void *ov96_022107F0(PokeathlonCourseData *data, enum HeapID heapID) {
    PokeathlonCourseData *pokeathlonCourseData;
    s32 v0;
    u32 v1;
    UnkStruct_ov96_022107F0_Heap_Alloc *dest;
    void *v2;

    pokeathlonCourseData = data;
    v1 = PokeathlonCourse_GetParticipantCount(data);
    dest = Heap_Alloc(heapID, sizeof(UnkStruct_ov96_022107F0_Heap_Alloc));
    MI_CpuFill8(dest, 0U, sizeof(UnkStruct_ov96_022107F0_Heap_Alloc));
    dest->unk_0 = heapID;
    v0 = 0;
    dest->unk_4 = data;
    dest->unk_BC = (u8)(4 - v1);
    if ((s32) dest->unk_BC > 0) {
        v2 = (void *)&dest->unk_8;
        do {
            ov96_022108BC(v2, (u8)v1, pokeathlonCourseData);
            v0 += 1;
            v1 += 1;
            v2 += 0x3C;
        } while (v0 < (s32) dest->unk_BC);
    }
    return dest;
}
