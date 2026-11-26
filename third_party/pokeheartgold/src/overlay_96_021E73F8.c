#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"

#include "error_handling.h"
#include "heap.h"
#include "player_data.h"
#include "unk_02031B0C.h"

s32 sub_02031B10(void);

BOOL ov96_021E73F8(PokeathlonCourseData *arg0, u8 *unused);

s32 ov96_021E87EC(s32, u8 *, s32, Ov96System *);
u32 ov96_021E9A18(void);
BOOL ov96_021E74AC(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E73F8(PokeathlonCourseData *data, u8 *unused) {
    PlayerProfile *playerProfile;
    Pokeathlon_UnkSubStruct_B00 *v0;
    SaveApricornBox *saveApricornBox;
    u32 size;

    (*(s32(**)(PokeathlonCourseData *, s32))((u8 *)data->state + 0xC))(data, 0);
    if (data->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    saveApricornBox = Save_ApricornBox_Get(data->args->saveData);
    playerProfile = Save_PlayerData_GetProfile(data->args->saveData);
    v0 = PokeathlonSave_GetUnkB00(Save_Pokeathlon_Get(data->args->saveData));
    size = sub_02031B10() * 4;
    data->heapAllocPtr3 = Heap_AllocAtEnd(data->heapId, size);
    memset(data->heapAllocPtr3, 0, size);
    data->heapAllocPtr2 = sub_020320E0(saveApricornBox, playerProfile, v0->unk70, data->heapId);
    if (data->args->mode == 1U) {
        PokeathlonCourse_SetStateTransitionType(data, 0xEU);
        PokeathlonCourse_SetStateField07(data, 0x23U);
    } else {
        PokeathlonCourse_SetStateField07(data, 0x25U);
    }
    return 0;
}

BOOL ov96_021E74AC(PokeathlonCourseData *data, u8 *unused) {
    ov96_021E5F24(data);
    if (ov96_021E87EC(0x1F, data->filler_B44, ov96_021E9A18(), data->system) != 0) {
        PokeathlonCourse_SetStateField07(data, 0x26U);
    }
    return 0;
}
