#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "unk_0206793C.h"

void sub_02067BC0(FieldSystem *fieldSystem) {
    Save_VarsFlags_ClearSafariSysFlag(Save_VarsFlags_Get(fieldSystem->saveData));
}
