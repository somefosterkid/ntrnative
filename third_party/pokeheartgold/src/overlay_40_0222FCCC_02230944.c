#include "global.h"
struct UnkStruct_0208C06C;

#include "unk_02005D10.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_02230864();

void ov40_02230944(void);

void ov40_02230944(void) {
    ov40_02230864();
    PlaySE(0x57BU);
}

s32 ov40_02230958(struct UnkStruct_0208C06C *arg0);

s32 ov40_02230958(UnkStruct_0208C06C *arg0) {
    return arg0->unk_14.unk_514;
}

void ov40_02230964(struct UnkStruct_0208C06C *arg0, s32 arg1);

void ov40_02230964(UnkStruct_0208C06C *arg0, s32 arg1) {
    arg0->unk_14.unk_514 = arg1;
}
