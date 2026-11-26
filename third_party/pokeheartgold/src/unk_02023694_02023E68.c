#include "global.h"

#include "error_handling.h"
#include "billboard.h"
#include "billboard_gfx_sequence.h"

void sub_02023E78(Billboard *billboard, const VecFx32 *scale);
const VecFx32 *sub_02023E94(const Billboard *billboard);
void sub_02023EA4(Billboard *billboard, u8 draw);
u8 sub_02023EB8(const Billboard *billboard);

const VecFx32 *sub_02023E68(const Billboard *billboard) {
    if (billboard == NULL) {
        GF_AssertFail();
    }
    return &billboard->pos;
}

void sub_02023E78(Billboard *billboard, const VecFx32 *scale) {
    ((billboard) ? (void)0 : GF_AssertFail());
    billboard->scale = *scale;
}

const VecFx32 *sub_02023E94(const Billboard *billboard) {
    ((billboard) ? (void)0 : GF_AssertFail());
    return &billboard->scale;
}

void sub_02023EA4(Billboard *billboard, u8 draw) {
    ((billboard) ? (void)0 : GF_AssertFail());
    billboard->draw = draw;
}

u8 sub_02023EB8(const Billboard *billboard) {
    ((billboard) ? (void)0 : GF_AssertFail());
    return billboard->draw;
}
