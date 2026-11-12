// Black's code at ov021 0x021c06a8-0x021c06cc. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/height_ex.h"

u32 func_ov021_021c06a8(u8 index, HeightExList *list) {
    return list->entries[index].value;
}

u32 func_ov021_021c06b4(u8 index, HeightExList *list) {
    return list->entries[index].flags;
}

void func_ov021_021c06c0(u8 index, u32 height, HeightExList *list) {
    list->entries[index].value = height;
}
