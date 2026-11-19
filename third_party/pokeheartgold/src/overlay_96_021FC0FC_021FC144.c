#include "global.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "math_util.h"

void ov96_021FC144(void *param_1);

void ov96_021FC144(void *param_1) {
    int uVar1;
    u32 remainder;
    uVar1 = LCRandom();
    remainder = ((s32)uVar1) % ((s32)3);
    uVar1 = remainder & 0xff;
    ReadWholeNarcMemberByIdPair((void *)(param_1 + 0x18), NARC_a_2_2_6, uVar1);
    return;
}
