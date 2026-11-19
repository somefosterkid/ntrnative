#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

u32 ov40_022306C0(Window *, String *);

extern const u8 ov40_022452DC[];

void ov40_02236F38(void *arg0);

void ov40_02236F38(void *arg0) {
    String *string;
    u32 x;
    Window *window;
    const u8 *template;
    u32 baseTile;
    u32 i;

    template = ov40_022452DC;
    baseTile = 1;
    i = 0;
    window = (Window *)(*((u8 **)&((UnkStruct_0208C06C *)arg0)->unk_860) + 0x234);
    do {
        InitWindow(window);
        AddWindowParameterized(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, window, 6, template[0], template[1], template[2], template[3], 0xE, baseTile);
        string = NewString_ReadMsgData(((UnkStruct_0208C06C *)arg0)->unk_14.unk_34, i + 0x3C);
        x = ov40_022306C0(window, string);
        FillWindowPixelBuffer(window, 0);
        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, x, 0, 0xFF, 0xF0D00, NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(string);
        window++;
        baseTile += template[2] * template[3];
        template += 4;
        i++;
    } while (i < 6);
}
