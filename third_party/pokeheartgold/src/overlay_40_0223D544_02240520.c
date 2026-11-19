#include "global.h"

#include "overlay_40.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_02241204.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_02243538(UnkStruct_ov62_02246BF4 *);

s32 ov40_02240520(UnkStruct_0208C06C *arg0) {
    UnkStruct_ov62_02241204 *v0 = arg0->unk_860;

    if (ov40_02243538(&v0->unk_1D4) == 1) {
        ov40_0222BF80(arg0, 4);
    }
    return 0;
}
