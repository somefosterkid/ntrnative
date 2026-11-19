#include "global.h"

#include "bg_window.h"
#include "overlay_34.h"

void ov34_0225D77C(Ov34Row *rows, Window *titleWindow) {
    s32 i;

    RemoveWindow(titleWindow);
    i = 0;
    do {
        RemoveWindow(&rows[i].window1);
        RemoveWindow(&rows[i].window0);
        RemoveWindow(&rows[i].window2);
        i = i + 1;
    } while (i < 3);
    return;
}
