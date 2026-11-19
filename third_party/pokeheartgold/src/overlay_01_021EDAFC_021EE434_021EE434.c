#include "global.h"

#include "list_menu.h"

void ov01_021EE434(ListMenu *list, u32 arg1, u8 arg2);

void ov01_021EE434(ListMenu *list, u32 arg1, u8 arg2) {
    if (arg1 == -3) {
        ListMenuOverrideSetColors(list, 3U, 0xFU, 4U);
        return;
    }
    ListMenuOverrideSetColors(list, 1U, 0xFU, 2U);
}
