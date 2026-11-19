#include "global.h"

#include "pokeathlon/pokeathlon.h"

void ov96_021E7658(PokeathlonCourseData *param_1, s32 param_2);

u32 ov96_021E7030(PokeathlonCourseData *param_1);

u32 ov96_021E7030(PokeathlonCourseData *param_1) {
    u16 uVar1;
    u16 *puVar2;
    u32 iVar3;
    void (*pcVar4)();
    u16(*pauVar5)[2];

    puVar2 = (u16 *)ov96_021E8A20(param_1->filler_304 + 0x78);
    pauVar5 = param_1->field_5F0;
    iVar3 = 0x12;
    do {
        uVar1 = *puVar2;
        puVar2 = puVar2 + 1;
        (*pauVar5)[0] = uVar1;
        pauVar5 = (u16(*)[2])(*pauVar5 + 1);
        iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    pcVar4 = *(void (**)())(param_1->state + 8);
    if (pcVar4 != NULL) {
        (*pcVar4)(param_1, 0);
    }
    ov96_021E7658(param_1, param_1->currentParticipantIndex & 0xff);
    PokeathlonCourse_SetStateField07(param_1, 0x17);
    return 0;
}
