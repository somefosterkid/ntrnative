#include "global.h"

#include "overlay_40.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_022359B4(UnkStruct_0208C06C *, s32);

s32 ov40_02234A14(void *arg0) {
    if (*((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) == 0) {
        ov40_022359B4(arg0, 1);
        *((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (s32)(*((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
    } else {
        ov40_0222BF80(arg0, 3);
    }
    return 0;
}
