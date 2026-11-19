#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"

#include "wild_encounter.h"

void ov96_021E7F48(PokeathlonCourseData *param_1);

void ov96_021E7F48(PokeathlonCourseData *param_1) {
    PokeathlonSave_FriendshipRecords *pPVar1;
    u8 uVar2;
    u8 uVar3;
    u8 *base;

    pPVar1 = Save_Pokeathlon_FriendshipRecords_Get(param_1->args->saveData);
    uVar2 = ov96_021E5F24(param_1);
    base = &param_1->participants[uVar2].field_04;
    for (uVar3 = 0; uVar3 < 3; uVar3++) {
        s32 iVar4 = uVar3 * 0x28;
        pPVar1->friendshipRoomStatues[uVar3].species = *(u16 *)(base + iVar4);
        pPVar1->friendshipRoomStatues[uVar3].form = (u8) * (u16 *)(base + iVar4 + 2);
        pPVar1->friendshipRoomStatues[uVar3].gender = base[iVar4 + 0x11];
    }
}
