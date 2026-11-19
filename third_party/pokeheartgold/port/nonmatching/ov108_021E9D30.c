/* PORTOK hk39 */
#include "bg_window.h"
#include "yes_no_prompt.h"
#include "bg_window.h"
#include "constants/heap.h"
#include "yes_no_prompt.h"
extern WindowTemplate ov108_021EAC18[6];

void ov108_021E9D30(enum HeapID *param_1)
{
    WindowTemplate *template;
    Window *windows;
    Window *window;
    u16 baseTile;
    s32 i;

    template = ov108_021EAC18;
    window = (Window *)((u8 *)param_1 + 0x454);
    i = 0;
    do {
        AddWindow(*(BgConfig **)((u8 *)param_1 + 0x438), window, template);
        FillWindowPixelBuffer(window, 0);
        i++;
        template++;
        window++;
    } while (i <= 5);
    baseTile = 0x2E2;
    windows = (Window *)((u8 *)param_1 + 0x454);
    i = 0;
    do {
        AddWindowParameterized(*(BgConfig **)((u8 *)param_1 + 0x438), windows + i + 6, 5, (i % 2) * 0x10 + 1, (i / 2) * 5 + 6, 0xE, 2, 4, baseTile);
        baseTile -= 0x1C;
        i++;
    } while (i < 6);
    *(YesNoPrompt **)((u8 *)param_1 + 0x514) = YesNoPrompt_Create(*param_1);
}
