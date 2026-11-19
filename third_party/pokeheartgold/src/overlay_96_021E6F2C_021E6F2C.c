#include "global.h"

#include "pokeathlon/pokeathlon.h"

u32 ov96_021E75E4(PokeathlonCourseData *data);

u32 ov96_021E6F2C(PokeathlonCourseData *data);

u32 ov96_021E6F2C(PokeathlonCourseData *data) {
    u16 *buf;
    int idx;
    int size;
    BOOL ret;
    int i;

    if (*(s32 *)(data->state + 0x10) != 0) {
        buf = (u16 *)ov96_021E8A20(data->dataCopySource);
        idx = ov96_021E5F24(data);
        buf[0] = data->field_5E0[idx][0];
        buf[1] = data->field_5E0[idx][1];
        ret = ov96_021E87B4(0x1d, data->dataCopySource, ov96_021E9A14(), (int)data->system);
        if (ret && !ov96_021E5F24(data)) {
            size = PokeathlonCourse_GetUnkConstant4(data);
            for (i = PokeathlonCourse_GetParticipantCount(data); i < 4; i++) {
                memcpy(ov96_021E8A20(data->dataCopyBuffer1) + i * size, data->field_5E0[i], size);
            }
        }
    } else if (!ov96_021E5F24(data)) {
        ret = ov96_021E75E4(data);
    } else {
        ret = TRUE;
    }
    if (ret) {
        PokeathlonCourse_SetStateField07(data, 0x26);
    }
    return 0;
}
