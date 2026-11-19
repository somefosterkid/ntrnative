#include "global.h"

#include "bg_window.h"
#include "overlay_32.h"
#include "overlay_32_name_picker_data.h"

void ov32_0225D834(NamePickerData *data) {
    u32 i;
    Window *window;

    i = 0;
    window = data->windows;
    do {
        RemoveWindow(window);
        i = i + 1;
        window = window + 1;
    } while (i < 0x12);
}
