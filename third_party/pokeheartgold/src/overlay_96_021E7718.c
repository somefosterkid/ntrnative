#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"

#include "save_vars_flags.h"
#include "overlay_96_app_021e7718.h"

s32 ov96_021E786C(PokeathlonCourseData *, PokeathlonSave *);

s32 ov96_021E7938(PokeathlonCourseData *, Pokeathlon_RecordsSolo *);

s32 ov96_021E7A2C(PokeathlonCourseData *, Pokeathlon_RecordsLink *);

s32 ov96_021E7BA8(PokeathlonCourseData *, Pokeathlon_RecordsSolo *, void *);

s32 ov96_021E7D6C(PokeathlonCourseData *, Pokeathlon_UnkSubStruct_B00 *);

s32 ov96_021E7FA8(PokeathlonCourseData *);

s32 ov96_021E8060();

s32 ov96_021E8084(PokeathlonCourseData *);

s32 ov96_021E80C4(PokeathlonCourseData *);

void ov96_021E7718(PokeathlonCourseData *arg0);

void ov96_021E7718(PokeathlonCourseData *data) {
    Pokeathlon_RecordsSolo *v0;
    SaveVarsFlags *varsFlags;
    PokeathlonFieldData *pokeathlonFieldData;
    PokeathlonSave *pokeathlonSave;
    u16 amount;

    pokeathlonSave = Save_Pokeathlon_Get(data->args->saveData);
    pokeathlonFieldData = PokeathlonCourse_GetFieldData(data);
    varsFlags = Save_VarsFlags_Get(data->args->saveData);
    if (((u32)(((Ov96App_021E7718 *)pokeathlonFieldData)->unk_1D0 << 0x1F) >> 0x1F) != 0) {
        ov96_021E7FA8(data);
        if ((Save_VarsFlags_CheckFlagInArray(varsFlags, 0xF0U) == 0) && (ov96_021E8084(data) != 0)) {
            Save_VarsFlags_SetFlagInArray(varsFlags, 0xF0U);
        }
    }
    amount = ((Ov96App_021E7718 *)pokeathlonFieldData)->unk_1D2;
    if (data->args->filler_A[2] == 0xA) {
        amount *= 2;
    }
    PokeathlonSave_AddAthletePoints(pokeathlonSave, amount);
    if (data->args->mode == 1) {
        ov96_021E7A2C(data, PokeathlonSave_GetRecordsLink2(pokeathlonSave));
    } else {
        v0 = PokeathlonSave_GetRecordsSolo(pokeathlonSave);
        ov96_021E7BA8(data, v0, PokeathlonSave_GetUnkAEC(pokeathlonSave));
        if (((u32)(((Ov96App_021E7718 *)pokeathlonFieldData)->unk_1D0 << 0x1F) >> 0x1F) != 0) {
            ov96_021E786C(data, PokeathlonSave_dummy2(pokeathlonSave));
            if (Save_VarsFlags_CheckFlagInArray(varsFlags, 0xEFU) == 0) {
                PokeathlonSave_dummy2(pokeathlonSave);
                if (ov96_021E8060() != 0) {
                    Save_VarsFlags_SetFlagInArray(varsFlags, 0xEFU);
                }
            }
        }
        ov96_021E7938(data, PokeathlonSave_GetRecordsSolo2(pokeathlonSave));
    }
    ov96_021E7D6C(data, PokeathlonSave_GetAgainUnkB00(pokeathlonSave));
    if ((data->args->mode == 0) && (Save_VarsFlags_CheckFlagInArray(varsFlags, 0xF1U) == 0) && (ov96_021E80C4(data) != 0)) {
        Save_VarsFlags_SetFlagInArray(varsFlags, 0xF1U);
    }
    *(u16 *)((u8 *)data->args + 0xA) = (u16) ((Ov96App_021E7718 *)pokeathlonFieldData)->unk_1D2;
    *(u16 *)((u8 *)data->args + 8) = amount;
    data->args->filler_A[3] = (u8)((u32)(((Ov96App_021E7718 *)pokeathlonFieldData)->unk_1D0 << 0x1C) >> 0x1E);
}
