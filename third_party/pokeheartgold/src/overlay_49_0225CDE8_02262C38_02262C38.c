#include "global.h"

#include "list_menu.h"
#include "overlay_49_unk_struct_ov70_02266af0.h"
#include "pm_string.h"

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

void ov49_0225A10C(UnkStruct_ov70_0225DEE8 *param0, u32 param1);

void ov49_0225A144(UnkStruct_ov70_0225DEE8 *param0, const String *param1, u32 param2);

ListMenuItem *ov49_0225A154(const UnkStruct_ov70_0225DEE8 *param0);

String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);

extern const ListMenuTemplate ov49_02269DFC;

void ov49_02262C38(UnkStruct_ov70_02266AF0 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, u32 param3) {
    int v0;
    String *v1;

    ov49_0225A10C(param1, param2);

    param0->unk_38 = param2;

    for (v0 = 0; v0 < param2; v0++) {
        v1 = ov49_0225A30C(param1, 0, param3 + v0);
        ov49_0225A144(param1, v1, v0);
    }

    param0->unk_18 = ov49_02269DFC;
    param0->unk_18.totalItems = param0->unk_38;

    if (param0->unk_18.maxShowed > param0->unk_38) {
        param0->unk_18.maxShowed = param0->unk_38;
    }

    param0->unk_18.items = ov49_0225A154(param1);
}
