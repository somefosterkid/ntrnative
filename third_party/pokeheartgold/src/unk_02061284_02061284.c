#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "unk_02061284.h"

void sub_02061284(LocalMapObject *param_1, u32 param_2);

void sub_02061284(LocalMapObject *param_1, u32 param_2) {
    u8 *puVar1;
    s32 iVar2;

    puVar1 = sub_0205F370(param_1, 8);
    iVar2 = sub_02061E20(_020FD7B8, -1);
    *(s16 *)(puVar1 + 2) = (s16)iVar2;
    *(u32 *)(puVar1 + 4) = param_2;
    sub_0205F328(param_1, 0);
    MapObject_ClearSingleMovement(param_1);
    return;
}

void sub_020612B4(LocalMapObject *param_1) {
    sub_02061284(param_1, 0);
    return;
}

void sub_020612C0(LocalMapObject *param_1) {
    sub_02061284(param_1, 1);
    return;
}

void sub_020612CC(LocalMapObject *param_1) {
    sub_02061284(param_1, 2);
    return;
}

void sub_020612D8(LocalMapObject *param_1) {
    sub_02061284(param_1, 3);
    return;
}

void sub_020612E4(LocalMapObject *param_1) {
    sub_02061284(param_1, 4);
    return;
}

void sub_020612F0(LocalMapObject *param_1) {
    sub_02061284(param_1, 5);
    return;
}

void sub_020612FC(LocalMapObject *param_1) {
    sub_02061284(param_1, 6);
    return;
}

void sub_02061308(LocalMapObject *param_1) {
    sub_02061284(param_1, 7);
    return;
}

void sub_02061314(LocalMapObject *param_1) {
    sub_02061284(param_1, 8);
    return;
}
