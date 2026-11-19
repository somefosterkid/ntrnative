#include "global.h"

#include "error_handling.h"
#include "billboard_gfx_sequence.h"
#include "billboard.h"

void sub_020243A4(Billboard *billboard, fx32 numFrames);

void sub_02023F04(Billboard *billboard, fx32 numFrames);

fx32 sub_02024394(const Billboard *billboard, int animNum);
void sub_02023F1C(Billboard *billboard, fx32 frameNum);
fx32 sub_02023F30(const Billboard *billboard);
void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

void sub_02023F04(Billboard *billboard, fx32 numFrames) {
    if (billboard == NULL) {
        GF_AssertFail();
    }
    sub_020243A4(billboard, numFrames);
}

void sub_02023F1C(Billboard *billboard, fx32 frameNum) {
    ((billboard) ? (void)0 : GF_AssertFail());
    billboard->frameNum = frameNum;
}

fx32 sub_02023F30(const Billboard *billboard) {
    ((billboard) ? (void)0 : GF_AssertFail());
    return billboard->frameNum;
}

void sub_02023F40(Billboard *billboard, fx32 animFrameNum) {
    ((billboard) ? (void)0 : GF_AssertFail());

    billboard->frameNum = sub_02024394(billboard, billboard->animNum);
    billboard->frameNum += animFrameNum;
}
