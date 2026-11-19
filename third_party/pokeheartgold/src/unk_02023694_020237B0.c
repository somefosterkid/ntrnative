#include "global.h"
#include "billboard.h"


void sub_02023950(BillboardList *list);

void BillboardLists_Draw(void);

void BillboardLists_Draw(void) {
    s32 i;
    BillboardList *list;

    i = 0;
    if (_021D2208.count > 0) {
        do {
            list = &_021D2208.lists[i];
            if (list->draw == 1) {
                sub_02023950(list);
            }
            list = &_021D2208.lists[i];
            if (list->redraw == 1) {
                list->redraw = 2U;
            }
            i += 1;
        } while (i < _021D2208.count);
    }
}
