#include "global.h"

#include "constants/heap.h"
#include "overlay_88_unk_struct_ov66_02231428.h"
#include "filesystem_files_def.h"
#include "filesystem.h"

struct NARC;

void ov45_0222D524(const UnkStruct_ov66_02231428 *param0, const UnkStruct_ov66_02231428 *param1, UnkStruct_ov66_02231428 *param2);

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

void ov45_02231018(NARC *param0, u32 param1, u16 param2, u8 param3, fx32 *param4);

UnkStruct_ov66_02231428 ov45_02230F94(u16 param0, u8 param1, UnkStruct_ov66_02231428 param2, enum HeapID heapID);

UnkStruct_ov66_02231428 ov45_02230F94(u16 param0, u8 param1, UnkStruct_ov66_02231428 param2, enum HeapID heapID) {
    fx32 v0;
    fx32 v1;
    u16 v2;
    s32 v3;
    UnkStruct_ov66_02231428 v4;
    NARC *v5 = NARC_New((enum NarcId)98, heapID);

    ov45_02231018(v5, heapID, 219, 3, &v0);
    ov45_02231018(v5, heapID, param0, param1, &v1);

    v2 = v0 - v1;
    v3 = (v2 * 24) / 0xffff;

    if (v3 < 0) {
        v3 += 24;
    }

    if (v3 >= 24) {
        v3 %= 24;
    }

    memset(&v4, 0, sizeof(UnkStruct_ov66_02231428));
    v4.unk_00_val2_unk_00 = v3;

    ov45_0222D524(&v4, &param2, &v4);
    NARC_Delete(v5);

    return v4;
}
