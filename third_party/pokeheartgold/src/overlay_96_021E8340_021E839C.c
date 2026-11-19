#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"

#include "error_handling.h"

extern u8 _0221A7D8[];

BOOL ov96_021E839C(PokeathlonCourseData *data);

BOOL ov96_021E839C(PokeathlonCourseData *data) {
    PokeathlonSave *save = Save_Pokeathlon_Get(data->args->saveData);
    u32 idx;
    Pokeathlon_CourseEntry *entries;
    u16 cur;

    if (data->filler_D2A == 0xFFFF) {
        GF_AssertFail();
        return FALSE;
    }
    idx = PokeathlonCourse_GetField3D8_ForCurrentParticipant(data);
    if (data->args->mode == 1) {
        entries = PokeathlonSave_GetRecordsLink2(save)[idx].entries;
    } else {
        entries = PokeathlonSave_GetRecordsSolo2(save)[idx].entries;
    }
    cur = entries->unk_0;
    if (cur == 0xFFFF) {
        return TRUE;
    }
    if (_0221A7D8[idx] == 0) {
        if (cur < data->filler_D2A) {
            return TRUE;
        }
    } else {
        if (cur > data->filler_D2A) {
            return TRUE;
        }
    }
    return FALSE;
}
