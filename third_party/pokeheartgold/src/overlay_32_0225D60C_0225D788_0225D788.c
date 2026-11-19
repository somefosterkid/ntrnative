#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "overlay_32.h"
#include "overlay_32_name_picker_data.h"
#include "pm_string.h"
#include "text.h"

extern const WindowTemplate ov32_0225E24C[18];

void ov32_0225D788(NamePickerData *data) {
    String *string;
    u16 x;
    u32 width;
    u32 i;

    i = 0;
    do {
        AddWindow(data->bgConfig, &data->windows[i], &ov32_0225E24C[i]);
        i = i + 1 & 0xffff;
    } while (i < 0x12);
    FillWindowPixelBuffer(&data->windows[0], 0);
    string = NewString_ReadMsgData(data->msgData, 0x1d);
    x = (u16)(GetWindowWidth(&data->windows[0]) * 8 / 2);
    width = FontID_String_GetWidth(FONT_UNK_4, string, 0);
    x -= width / 2;
    AddTextPrinterParameterizedWithColor(&data->windows[0], FONT_UNK_4, string, x, 0, 0xff, 0x50600, NULL);
    String_Delete(string);
    CopyWindowPixelsToVram_TextMode(&data->windows[0]);
    ScheduleWindowCopyToVram(&data->windows[0]);
    ov32_0225D84C(data);
    ov32_0225D988(data);
}
