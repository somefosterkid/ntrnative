#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "unk_02061284.h"

u32 sub_02062050(LocalMapObject *param_1);

void sub_02062064(LocalMapObject *param_1, void *param_2);

void sub_0206197C(LocalMapObject *param_1) {
    u8 *puVar1;
    s32 iVar2;

    puVar1 = sub_0205F370(param_1, 8);
    iVar2 = sub_02062050(param_1);
    if (iVar2 == 1) {
        sub_02062064(param_1, (puVar1 + 4));
    }
    return;
}
