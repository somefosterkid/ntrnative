#include "global.h"
#include "billboard.h"

int sub_020243C4(const BillboardAnim *anim, fx32 *currentFrame, fx32 numFrames);

const BillboardAnim *sub_02024454(const BillboardAnim *anims, int index);

void sub_020243A4(Billboard *billboard, fx32 numFrames);

void sub_020243A4(Billboard *billboard, fx32 numFrames) {
    sub_020243C4(sub_02024454(billboard->anims, billboard->animNum), &billboard->frameNum, numFrames);
}
