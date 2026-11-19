#include "global.h"
struct UnkStruct_ov70_0225EC20;

#include "list_menu_items.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"

void ov49_0225AEE0(struct UnkStruct_ov70_0225EC20 *arg0);

void ov49_0225AEE0(UnkStruct_ov70_0225EC20 *arg0) {
    ListMenuItem *items;

    items = *((ListMenuItem * *)&arg0->unk_34);
    if (items != NULL) {
        ListMenuItems_Delete(items);
        *((ListMenuItem * *)&arg0->unk_34) = NULL;
        *((s16 *)&arg0->unk_38) = 0;
    }
}
