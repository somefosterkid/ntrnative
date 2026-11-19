#include "global.h"
#include "billboard.h"

const BillboardAnim *sub_02024454(const BillboardAnim *anims, int index);

fx32 sub_02024394(const Billboard *billboard, int animNum);

fx32 sub_02024394(const Billboard *billboard, int animNum) {
    const BillboardAnim *anim = sub_02024454(billboard->anims, animNum);

    return anim->startFrame << 12;
}
