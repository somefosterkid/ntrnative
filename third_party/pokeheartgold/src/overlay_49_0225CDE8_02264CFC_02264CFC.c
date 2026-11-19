#include "global.h"

#include "overlay_49_unk_struct_ov49_02264cf8.h"

#include "bg_window.h"
#include "list_menu.h"

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_02263344_t;

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_02264CFC(UnkStruct_ov49_02264CF8 *param0, u8 param1, u8 param2, UnkStruct_ov70_02263344 *param3, u8 param4);

void ov49_02264CFC(UnkStruct_ov49_02264CF8 *param0, u8 param1, u8 param2, UnkStruct_ov70_02263344 *param3, u8 param4) {
    param0->unk_06 = param1;
    param0->unk_07 = param2;

    ov49_0225EF8C(param3, param4);
}
