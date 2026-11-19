#include "global.h"
#include "overlay_28.h"

void ov28_0225DD58(DowsingMchnApp *app) {
    s16 sVar1;
    s32 iVar2;
    s32 iVar3;
    u32 uVar4;
    HiddenItemResponse *item;
    u32 i;
    u32 unaff_r7;

    DowsingMchn_GetHiddenItemLocs(app);
    i = 0;
    app->hasNearestItem = 0;
    while (1) {
        item = &app->hiddenItems[i];
        if (item->unk4 == 0xff) {
            break;
        }
        if (app->hasNearestItem == 0) {
            sVar1 = item->x;
            iVar2 = abs((s32)sVar1);
            iVar3 = abs((s32)item->y);
            unaff_r7 = iVar2 + iVar3 & 0xffff;
            app->nearestX = sVar1;
            app->nearestY = app->hiddenItems[i].y;
            app->hasNearestItem = 1;
        } else {
            sVar1 = item->x;
            iVar2 = abs((s32)sVar1);
            iVar3 = abs((s32)item->y);
            uVar4 = iVar2 + iVar3 & 0xffff;
            if (unaff_r7 > uVar4) {
                unaff_r7 = uVar4;
                app->nearestX = sVar1;
                app->nearestY = app->hiddenItems[i].y;
            }
        }
        i++;
    }
    DowsingMchn_FreeHiddenItemLocs(app);
    return;
}
