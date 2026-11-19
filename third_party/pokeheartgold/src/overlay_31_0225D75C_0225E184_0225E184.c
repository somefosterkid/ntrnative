#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "overlay_03.h"
#include "overlay_31.h"
#include "pm_string.h"

void ov31_0225E184(MartSubScreen *param_1) {
    String *string;
    u32 uVar1;
    s32 iVar2;
    MartData *iVar2_p;
    s32 iVar4;
    MartData *data;

    iVar2_p = param_1->martData;
    iVar4 = iVar2_p->cursorPos + (u32)iVar2_p->pageOffset;
    string = NewString_ReadMsgData(param_1->itemMsgData, (u32)iVar2_p->items[iVar4]);
    ov31_0225DE00(param_1, &param_1->windows[MART_SUBSCREEN_WIN_SELECTED_ITEM], string, iVar4);
    String_Delete(string);
    iVar2 = ov31_0225E12C(param_1->martData, iVar4, param_1->martData->items[iVar4]);
    if (iVar2 != 0) {
        data = param_1->martData;
        uVar1 = ShopMenu_GetItemPrice(data, data->items[iVar4]);
        ov31_0225DE24(param_1->messageFormat, param_1->msgData, &param_1->windows[MART_SUBSCREEN_WIN_SELECTED_ITEM], uVar1, data->martType);
    }
    ScheduleWindowCopyToVram(&param_1->windows[MART_SUBSCREEN_WIN_SELECTED_ITEM]);
    return;
}
