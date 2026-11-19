#include "global.h"

#include "billboard.h"

void ov01_021F8C00(Billboard *billboard, int frameDiv);

void sub_02023EE0(Billboard *billboard, int animNum);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

fx32 sub_02023F70(const Billboard *billboard);

void ov01_021F8C3C(Billboard *billboard, int animNum, int frameDiv);

void ov01_021F8C3C(Billboard *billboard, int animNum, int frameDiv) {
    fx32 v0;

    ov01_021F8C00(billboard, frameDiv);
    v0 = sub_02023F70(billboard);
    sub_02023EE0(billboard, animNum);
    sub_02023F40(billboard, v0);
}

void ov01_021F8C30(Billboard *billboard);

void ov01_021F8C64(Billboard *billboard, int animNum);

void ov01_021F8C64(Billboard *billboard, int animNum) {
    fx32 v0;

    ov01_021F8C30(billboard);
    v0 = sub_02023F70(billboard);
    sub_02023EE0(billboard, animNum);
    sub_02023F40(billboard, v0);
}
