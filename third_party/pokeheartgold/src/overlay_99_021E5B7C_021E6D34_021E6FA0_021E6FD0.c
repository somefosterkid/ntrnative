#include "global.h"
#include "constants/heap.h"
#include "pokeathlon/pokeathlon_save.h"
#include "heap.h"
#include "overlay_98.h"
#include "overlay_99.h"
#include "player_data.h"
#include "overlay_99_obj99.h"
#include "overlay_99_pokeathlon_course_summary.h"

u16 ov99_021E6F70(PokeathlonSave *);

u32 ov99_021E6FA0(void *);

void ov99_021E6FD0(Obj99 *arg0, UnkStruct_0203EFA0 *arg1) {
    PlayerProfile *sp8;
    Pokeathlon_RecordsSolo *sp4;
    void *sp0;
    PokeathlonSave *temp_r6;
    PokeathlonCourseSummary *temp_r0;
    s32 var_r2;
    u16 temp_r1;
    PokeathlonSave *var_r5;

    temp_r0 = Heap_Alloc(arg0->heap, sizeof(PokeathlonCourseSummary));
    MI_CpuFill8(temp_r0, 0U, sizeof(PokeathlonCourseSummary));
    temp_r6 = arg1->pokeathlon;
    sp8 = arg1->profile;
    var_r5 = PokeathlonSave_dummy1(temp_r6);
    sp4 = PokeathlonSave_GetRecordsSolo(temp_r6);
    sp0 = PokeathlonSave_GetUnkDC(temp_r6);
    var_r2 = 0;
    do {
        temp_r1 = *(u16 *)((u8 *)var_r5 + 6);
        var_r5 = (void *)((u8 *)var_r5 + (0x2C));
        temp_r0->unk_0C[var_r2] = temp_r1;
        var_r2 += 1;
        temp_r0->unk_04 = (u16)(temp_r0->unk_04 + temp_r1);
    } while (var_r2 < 5);
    temp_r0->profile = sp8;
    temp_r0->unk_06 = ov99_021E6F70(temp_r6);
    temp_r0->unk_0A = ov99_021E6FA0(sp0);
    temp_r0->unk_16 = (s8)ov98_0221F024(sp0);
    temp_r0->unk_17 = ov98_0221F058(sp4);
    temp_r0->unk_08 = (s16)(temp_r0->unk_0A + (temp_r0->unk_04 + temp_r0->unk_06));
    temp_r0->unk_18 = (s8)PlayerProfile_GetTrainerGender(sp8);
    arg0->summary = temp_r0;
}
