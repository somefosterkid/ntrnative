#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "render_window.h"
#include "overlay_74_main_menu_window.h"
#include "message_format.h"
#include "gf_gfx_planes.h"

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

int ov74_0223547C(MainMenuWindow *window, int textEntryID);

s32 ov74_02235568(BgConfig *param0, MainMenuWindow *window, s32 xPos, s32 yPos, s32 textEntryID);

s32 ov74_02235568(BgConfig *param0, MainMenuWindow *window, s32 xPos, s32 yPos, s32 textEntryID) {
    Window *temp_r1;
    s32 temp_r0;
    s32 var_r5;

    temp_r1 = window->window;
    if (temp_r1->bgConfig == NULL) {
        AddWindowParameterized(param0, temp_r1, window->bgLayer, xPos, (s32)yPos, (s32)window->width, (s32)window->height, (s32)window->textPltt, (s32)window->baseTile);
        var_r5 = ov74_0223547C(window, textEntryID);
    } else {
        if (xPos != -1) {
            SetWindowX(temp_r1, xPos);
        }
        if (yPos != -1) {
            SetWindowY(window->window, yPos);
        }
        var_r5 = ov74_0223547C(window, textEntryID);
    }
    if (window->drawFrame == 1) {
        temp_r0 = window->scrollable;
        switch (temp_r0) {
        case 0:
            DrawFrameAndWindow1(window->window, 0, window->frameBaseTile, window->framePltt);
            break;
        case 1:
            DrawFrameAndWindow2(window->window, 0, window->frameBaseTile, window->framePltt);
            break;
        case 2:
        default:
            CopyWindowToVram(window->window);
            break;
        }
    }
    return var_r5;
}
