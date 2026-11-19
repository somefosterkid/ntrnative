#include "global.h"

#include "save.h"
#include "save_arrays.h"
#include "system.h"

s32 sub_0202FDA4(SaveData *arg0, struct UnkStruct_0202FBCC *arg1, s32 arg2, u16 *arg3);

s32 sub_0202FDA4(SaveData *saveData, struct UnkStruct_0202FBCC *data, s32 idx, u16 *arg3) {
    s32 v0;
    s32 v1;
    u16 v2;

    v2 = *arg3;
    switch (v2) {
    case 0:
        sub_0201A728(8);
        sub_0201A748(HEAP_ID_FIELD2);
        v0 = sub_02027134(saveData, data, idx);
        if (v0 == 2) {
            Save_PrepareForAsyncWrite(saveData, 2);
            *arg3 += 1;
            return 0;
        }
        sub_0201A738(8);
        return v0;
    case 1:
        v1 = Save_WriteFileAsync(saveData);
        if ((u32)(v1 - 2) <= 1U) {
            *arg3 = 0;
            sub_0201A774();
            sub_0201A738(8);
        }
        return v1;
    default:
        return 0;
    }
}
