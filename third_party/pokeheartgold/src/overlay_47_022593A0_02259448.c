#include "global.h"
struct UnkStruct_ov68_0225D128;

#include "list_menu_2d.h"
#include "overlay_47_unk_struct_ov68_0225_d128.h"

void ov47_02259448(struct UnkStruct_ov68_0225D128 *arg0, enum HeapID arg1);

void ov47_02259448(UnkStruct_ov68_0225D128 *arg0, enum HeapID heapID) {
    struct ListMenu2D *menu;

    menu = *((struct ListMenu2D * *)&arg0->unk_20);
    if (menu != NULL) {
        Clear2dMenuWindowAndDelete(menu, heapID);
        *((struct ListMenu2D * *)&arg0->unk_20) = NULL;
    }
}
