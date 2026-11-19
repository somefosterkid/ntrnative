#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "list_menu_items.h"

extern const Ov59MenuDef ov59_0223C668[];

void ov59_022398CC(Ov59ApricornApp *arg0);

void ov59_022398CC(Ov59ApricornApp *arg0) {
    s32 var_r4;
    u8 temp_r0;
    const Ov59MenuDef *temp_r5;

    temp_r0 = arg0->mode;
    if (temp_r0 != 0) {
        temp_r5 = &ov59_0223C668[temp_r0 - 1];
        arg0->listMenuItems = ListMenuItems_New(temp_r5->count, arg0->heapId);
        var_r4 = 0;
        if ((s32)temp_r5->count > 0) {
            do {
                ListMenuItems_AppendFromMsgData(arg0->listMenuItems, arg0->msgData, temp_r5->items[var_r4], var_r4);
                var_r4 += 1;
            } while (var_r4 < (s32)temp_r5->count);
        }
    }
}
