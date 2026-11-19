#include "global.h"

#include "error_handling.h"
#include "billboard_gfx_sequence.h"
#include "billboard.h"

void sub_02024380(Billboard *billboard, const BillboardResources *resources);

void sub_02023EC8(Billboard *billboard, const BillboardResources *resources);

void sub_02023EE0(Billboard *billboard, int animNum);
int sub_02023EF4(const Billboard *billboard);

void sub_02023EC8(Billboard *billboard, const BillboardResources *resources) {
    if (billboard == NULL) {
        GF_AssertFail();
    }
    sub_02024380(billboard, resources);
}

void sub_02023EE0(Billboard *billboard, int animNum) {
    ((billboard) ? (void)0 : GF_AssertFail());
    billboard->animNum = animNum;
}

int sub_02023EF4(const Billboard *billboard) {
    ((billboard) ? (void)0 : GF_AssertFail());
    return billboard->animNum;
}
