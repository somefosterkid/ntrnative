#include "global.h"

#include "bg_window.h"
#include "overlay_29_number_entry_data.h"

void ov29_0225D7D4(NumberEntryData *data);

void ov29_0225D7D4(NumberEntryData *data) {
    u32 uVar1;
    Window *window;

    uVar1 = 0;
    window = data->windows;
    do {
        RemoveWindow(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 < 3);
    return;
}
