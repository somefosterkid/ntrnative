#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_app_021ed1d0.h"

u16 ov96_021E8424(u32 param_1);

void ov96_021EEA88(ManagedSprite *, u32, u32, u32);

void ov96_021EEB74(ManagedSprite **, PokeathlonCourseData *, u32, u32);

u16 ov96_021EEBC8(u32);

s32 ov96_021EEDCC(s32 arg0);

void ov96_021ECAC4(PokeathlonCourseData *param_1);

void ov96_021ECAC4(PokeathlonCourseData *param_1) {
    u8 bVar1;
    Ov96App_021ED1D0 *puVar2;
    u32 uVar3;
    PokeathlonParticipantData *pPVar4;
    u32 index;

    puVar2 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    index = 0;
    do {
        bVar1 = PokeathlonCourse_GetParticipantCount(param_1);
        if (index == 4) {
            uVar3 = ov96_021EEBC8(2);
            ov96_021EEA88(puVar2->sprites[index], uVar3, 0xb, puVar2->heapId);
        } else if ((s32)index < (s32)(u32)bVar1) {
            ov96_021EEB74(puVar2->sprites, param_1, index & 0xff, 0);
        } else {
            pPVar4 = PokeathlonCourse_GetParticipantData(param_1, index);
            uVar3 = ov96_021EEDCC(ov96_021E8424(*(u32 *)pPVar4->field_00 & 0xff));
            ov96_021EEA88(puVar2->sprites[index], uVar3, 0, puVar2->heapId);
        }
        index = index + 1;
    } while ((s32)index < 5);
    return;
}
