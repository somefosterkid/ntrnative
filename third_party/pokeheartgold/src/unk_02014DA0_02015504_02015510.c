#include "global.h"

extern s32 _020F6078;

void sub_02015510(void *arg0);

void sub_02015510(void *arg0) {
    *(VecFx32 *)((u8 *)arg0) = *(VecFx32 *)((u8 *)&_020F6078);
}
