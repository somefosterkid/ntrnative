#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"
#include "save.h"


void ov96_021E8028(void *arg0, s32 arg1);
void ov96_021E7FA8(PokeathlonCourseData *arg0);
s32 ov96_021E8060(s32 arg0);

u32 ov96_021E8084(u8 *param_1);

void ov96_021E7F98(s32 frameCount, u32 maxValue, Pokeathlon_UnkSubStruct_B00 *result) {
    u32 uVar1;
    uVar1 = result->unk0 + frameCount;
    if (uVar1 > maxValue) {
        result->unk0 = maxValue;
        return;
    }
    result->unk0 = uVar1;
    return;
}

void ov96_021E7FA8(PokeathlonCourseData *arg0) {
    u8 v0;

    v0 = *(u8 *)((u8 *)(*((void * *)&arg0->args)) + 0xC);
    switch (v0) {
    case 0:
        ov96_021E8028(arg0, 1);
        return;
    case 1:
        ov96_021E8028(arg0, 2);
        return;
    case 2:
        ov96_021E8028(arg0, 4);
        return;
    case 3:
        ov96_021E8028(arg0, 8);
        return;
    case 4:
        ov96_021E8028(arg0, 0x10);
        return;
    case 5:
        ov96_021E8028(arg0, 0x11);
        return;
    case 6:
        ov96_021E8028(arg0, 0x14);
        return;
    case 7:
        ov96_021E8028(arg0, 6);
        return;
    case 8:
        ov96_021E8028(arg0, 0xA);
        return;
    case 9:
        ov96_021E8028(arg0, 9);
    default:
        return;
    }
}

void ov96_021E8028(void *arg0, s32 shift) {
    PokeathlonSave *pokeathlonSave;
    u8 v0;

    pokeathlonSave = Save_Pokeathlon_Get(**(SaveData ***)((u8 *)arg0 + 0x1F8));
    v0 = 0;
    do {
        PokeathlonSave_SetUnkDC_AtIndex(pokeathlonSave, shift, (s32) * (u16 *)((u8 *)(arg0 + (0x28 * v0)) + 0x204));
        v0 += 1;
    } while ((u32)v0 < 3U);
}

s32 ov96_021E8060(s32 arg0) {
    u8 v0;

    v0 = 0;
loop_1:
    if (*(u16 *)((u8 *)(arg0 + (v0 * 0x2C)) + 6) == 0) {
        return 0;
    }
    v0 += 1;
    if ((u32)v0 >= 5U) {
        return 1;
    }
    goto loop_1;
}

u32 ov96_021E8084(u8 *param_1) {
    PokeathlonSave *pokeathlonSave;
    void *pvVar2;
    u32 uVar3;
    int new_var;
    pokeathlonSave = Save_Pokeathlon_Get((SaveData *)(*(((void **)((PokeathlonCourseData *)param_1)->args))));
    uVar3 = 0;
    do {
        u16 uVar1;
        uVar1 = *((u16 *)((param_1 + (uVar3 * 0x28)) + 0x204));
        new_var = uVar1 - 1;
        pvVar2 = PokeathlonSave_GetUnkDC(pokeathlonSave);
        if ((*((u8 *)(((u8 *)pvVar2) + new_var))) == 0x1f) {
            return 1;
        }
        uVar3 = (uVar3 + 1) & 0xff;
    } while (uVar3 < 3);
    return 0;
}
