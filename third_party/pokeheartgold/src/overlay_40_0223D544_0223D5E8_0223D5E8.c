#include "global.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_39_unk_struct_ov62_022349_a8.h"

s32 ov39_02227778(UnkStruct_ov62_022349A8 *param0);

void *ov40_0223D540(UnkStruct_0208C06C *param0);

void ov40_0223D5E8(UnkStruct_0208C06C *arg0);

void ov40_0223D5E8(UnkStruct_0208C06C *arg0) {
    if ((arg0->unk_14.unk_54.unk_3B8) != 0) {
        ov39_02227778(ov40_0223D540(arg0));
    }
}

s32 ov39_022272EC(UnkStruct_ov62_022349A8 *param0);

void ov40_0223D600(void *arg0);

void ov40_0223D600(void *arg0) {
    if ((((UnkStruct_0208C06C *)arg0)->unk_14.unk_54.unk_3B8) != 0) {
        ov39_022272EC(ov40_0223D540(arg0));
    }
}
