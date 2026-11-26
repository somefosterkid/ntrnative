#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"

s32 ov96_021E87EC(u32 param_1, void *param_2, u32 param_3, Ov96System *);

u32 ov96_021E9A10(void);

BOOL ov96_021E6A00(PokeathlonCourseData *param_1, u8 *unused);

BOOL ov96_021E6A00(PokeathlonCourseData *param_1, u8 *unused) {
    PokeathlonSave *pokeathlonSave;
    Pokeathlon_UnkSubStruct_B00 *src;
    u32 uVar3;

    pokeathlonSave = Save_Pokeathlon_Get(param_1->args->saveData);
    src = PokeathlonSave_GetAgainUnkB00(pokeathlonSave);
    *(Pokeathlon_UnkSubStruct_B00 *)((u8 *)param_1 + 0x900) = *src;
    uVar3 = ov96_021E9A10();
    if (ov96_021E87EC(0x1a, (u8 *)((u8 *)param_1 + 0x900), uVar3, param_1->system) != 0) {
        PokeathlonCourse_SetStateField07(param_1, 0x26);
    }
    return 0;
}
