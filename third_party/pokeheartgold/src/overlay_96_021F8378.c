#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "error_handling.h"
#include "overlay_01_choose_starter_app.h"
#include "overlay_96_unk_struct_ov96_021_f7934.h"

void *ov96_021E94EC(void *, u32);

s32 ov96_021E9524(ChooseStarterApp *app);

s32 *ov96_021F8830(s32 *, s32, s32, s32 *);

void *ov96_021F8378(UnkStruct_ov96_021F8094 *arg0, PokeathlonCourseData *arg1, s32 arg2, s32 arg3);

void ov96_021F8354(UnkStruct_ov96_021F8128 *param_1);

void ov96_021F8918(void *param_1);

void ov96_021F83BC(UnkStruct_ov96_021F8094 *param_1, UnkStruct_ov96_021F8128 *param_2);

UnkStruct_ov96_021F8128 *ov96_021F83D0(UnkStruct_ov96_021F8094 *param_1);

s32 ov96_021F83DC(PokeathlonCourseData *param_1);

s32 ov96_021F83FC(PokeathlonCourseData *arg0);

void *ov96_021F8378(UnkStruct_ov96_021F8094 *arg0, PokeathlonCourseData *data, s32 arg2, s32 arg3) {
    void *v0;
    void *v1;
    int new_var;

    v1 = PokeathlonCourse_GetGraphicsSystem(data);
    new_var = ov96_021E9524(v1) - 1;
    v0 = ov96_021E94EC(v1, (u8)(new_var - arg0->taken));
    arg0->taken = (u8)(arg0->taken + 1);
    return ov96_021F8830(arg0->scene, arg2, arg3, v0);
}

void ov96_021F83BC(UnkStruct_ov96_021F8094 *param_1, UnkStruct_ov96_021F8128 *param_2) {
    ov96_021F8918(param_2->sprite);
    ov96_021F8354(param_2);
    return;
}

UnkStruct_ov96_021F8128 *ov96_021F83D0(UnkStruct_ov96_021F8094 *param_1) {
    return &param_1->slots[param_1->head];
}

s32 ov96_021F83DC(PokeathlonCourseData *param_1) {
    u32 uVar1;
    u32 uVar2;
    s32 iVar3;

    uVar1 = PokeathlonCourse_GetCurrentParticipantIndex(param_1);
    uVar2 = PokeathlonCourse_GetMode(param_1);
    if (uVar2 == 0) {
        iVar3 = uVar1 + 0xf6;
    } else {
        iVar3 = uVar1 + 0xf9;
    }
    return iVar3;
}

s32 ov96_021F83FC(PokeathlonCourseData *data) {
    u32 v2;
    u8 *v0;
    u8 v1;
    v2 = PokeathlonCourse_GetCurrentParticipantIndex(data);
    v0 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0xF0);
    v1 = (u32)((*v0) & 0xFFFFu);
    if (v1 >= 5U) {
        GF_AssertFail();
    };
    if (PokeathlonCourse_GetMode(data) == 0) {
        return (*v0) + ((v2 * 5) + 0x100);
    }
    return (*v0) + ((v2 * 5) + 0x10F);
}
