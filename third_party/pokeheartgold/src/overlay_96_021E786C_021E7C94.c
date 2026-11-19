#include "global.h"

#include "pokeathlon/pokeathlon.h"

void ov96_021E7C94(PokeathlonCourseData *param_1, u16 *param_2, u16 *param_3);

void ov96_021E7C94(PokeathlonCourseData *param_1, u16 *param_2, u16 *param_3) {
    u16 *puVar1;
    s32 iVar2;
    s32 iVar3;
    u16 *puVar4;
    u16 *puVar5;

    ov96_021E5F24(param_1);
    iVar2 = 0;
    do {
        iVar3 = 0;
        *param_2 = *param_3;
        puVar4 = param_3;
        puVar5 = param_2;
        do {
            puVar1 = puVar4 + 1;
            iVar3 = iVar3 + 1;
            puVar5[1] = *puVar1;
            puVar4 = puVar4 + 1;
            puVar5 = puVar5 + 1;
        } while (iVar3 < 3);
        iVar2 = iVar2 + 1;
        param_3 = param_3 + 0x10;
        param_2 = param_2 + 4;
    } while (iVar2 < 5);
    return;
}
