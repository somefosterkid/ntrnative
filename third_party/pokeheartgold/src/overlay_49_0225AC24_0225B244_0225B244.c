#include "global.h"

#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"

SysTask *RemoveWindow(SysTaskFunc callback, void *param, u32 priority);

SysTask *ov49_0225B244(SysTaskFunc callback, void *param, int priority);

SysTask *ov49_0225B244(SysTaskFunc callback, void *param, int priority) {
    return RemoveWindow(callback, param, priority);
}

struct BgConfig;

struct TextPrinterTemplate;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

void ScheduleWindowCopyToVram(Window *window);

typedef struct TextPrinterTemplate TextPrinterTemplate;

typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);

enum Font {
    FONT_SYSTEM = 0,
    FONT_MESSAGE,
    FONT_SUBSCREEN,
    FONT_UNOWN,

    FONT_MAX,
};

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, TextPrinterCallback callback);

void DrawFrameAndWindow1(Window *window, u8 skipTransfer, u16 tile, u8 palette);

typedef struct {
    Window unk_00;
} UnkStruct_ov70_0225F114;

void ov49_0225B24C(UnkStruct_ov70_0225F114 *param0, const String *param1);

void ov49_0225B24C(UnkStruct_ov70_0225F114 *param0, const String *param1) {
    AddTextPrinterParameterizedWithColor(&param0->unk_00, FONT_SYSTEM, param1, 0, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((15 & 0xFF) << 0))), 0L);
    DrawFrameAndWindow1(&param0->unk_00, 1, (1 + (18 + 12)) + ((18 + 12) + 24), 3);
    ScheduleWindowCopyToVram(&param0->unk_00);
}
