#include "global.h"

#include "obj_char_transfer.h"
#include "unk_02013534.h"

void ov96_021F91CC(u32 *param_1);

void ov96_021F91CC(u32 *param_1) {
    if ((TextOBJ *)*param_1 != NULL) {
        TextOBJ_Destroy((TextOBJ *)*param_1);
        sub_02021B5C((UnkStruct_02021AC8 *)(param_1 + 1));
        *param_1 = 0;
    }
    return;
}
