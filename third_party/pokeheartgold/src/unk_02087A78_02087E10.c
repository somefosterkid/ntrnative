#include "global.h"
struct UnkStruct_0208C06C;
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

extern s32 _02110594;

s32 sub_02087E10(s32 dir);

void sub_02087E1C(struct UnkStruct_0208C06C *arg0);

s32 sub_02087E10(s32 dir) {
    return *(s32 *)(&_02110594 + dir);
}

void sub_02087E1C(UnkStruct_0208C06C *arg0) {
    Save_VarsFlags_FlypointFlagAction(Save_VarsFlags_Get(arg0->saveData), 2U, 0x1BU);
}
