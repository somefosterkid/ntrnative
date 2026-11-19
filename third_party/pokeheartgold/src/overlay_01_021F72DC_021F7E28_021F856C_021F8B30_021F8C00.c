#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

fx32 sub_02023F70(const Billboard *billboard);

void ov01_021F8C00(Billboard *param0, int param1);

void ov01_021F8C00(Billboard *param0, int param1) {
    fx32 v0, v1;

    v0 = sub_02023F70(param0);
    v0 /= FX32_ONE;
    v1 = v0 % param1;
    v0 -= v1;
    v0 *= FX32_ONE;

    sub_02023F40(param0, v0);
    sub_02023F04(param0, 0);
}
