#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sprite.h"
#include "constants/heap.h"
#include "player_data.h"

void ov96_021EEB84(Sprite *param_1, PokeathlonCourseData *param_2, s32 param_3, u32 param_4);

void ov96_021EEB74(ManagedSprite **param_1, PokeathlonCourseData *param_2, s32 param_3, u32 param_4);

void ov96_021EEA94(Sprite *param_1, u32 param_2, u32 param_3, enum HeapID param_4);
u16 ov96_021EEBC8(u32 param_1);

void ov96_021EEB74(ManagedSprite **param_1, PokeathlonCourseData *param_2, s32 param_3, u32 param_4) {
    ov96_021EEB84(param_1[param_3]->sprite, param_2, param_3, param_4);
}

void ov96_021EEB84(Sprite *param_1, PokeathlonCourseData *param_2, s32 param_3, u32 param_4) {
    PlayerProfile *profile;
    enum HeapID HVar1;
    u32 uVar2;

    profile = PokeathlonCourse_GetPlayerProfileFromData(param_2, param_3);
    HVar1 = PokeathlonCourse_GetHeapID(param_2);
    uVar2 = PlayerProfile_GetTrainerGender(profile);
    if (uVar2 == 0) {
        uVar2 = ov96_021EEBC8(0);
    } else {
        uVar2 = ov96_021EEBC8(1);
    }
    ov96_021EEA94(param_1, uVar2, param_4 & 0xff, HVar1);
    return;
}
