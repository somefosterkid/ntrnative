#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_unk_struct_ov64_0222_f038.h"
#include "struct_defs/resdat_narc_structs.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "list_menu_2d.h"
#include "overlay_43_palpad.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

typedef u32 TextColor;

struct ListMenuCursor {
    TextColor color;
    String *string;
};

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

void ov43_0222A9F4(PalPadGfx *, Window *, s32);

s32 ov43_0222AD74(PalPadGfx *, s32, s32);

s32 ov43_0222AD98(PalPadGfx *, u8, s32);

s32 ov43_0222B3A4(PalPadMenu *, PalPadGfx *);

void ov43_0222B1FC(PalPadMenu *param0, PalPadShared *param1, PalPadGfx *param2);

void ov43_0222B1FC(PalPadMenu *param0, PalPadShared *param1, PalPadGfx *param2) {
    NNSG2dScreenData *screen;

    ov43_0222A9F4(param2, &param2->titleWindow, 0);
    ScheduleWindowCopyToVram(&param0->menuWindow);
    screen = param0->screen;
    LoadRectToBgTilemapRect(param2->bgConfig, 2U, &screen->rawData, 0U, 0U, screen->screenWidth / 8, screen->screenHeight / 8);
    ScheduleSetBgPosText(param2->bgConfig, 2U, BG_POS_OP_SET_ROT, 0);
    ScheduleSetBgPosText(param2->bgConfig, 2U, BG_POS_OP_SET_XSCALE, 0);
    ov43_0222AD74(param2, 1, 0);
    ov43_0222AD98(param2, 1, 0);
    ov43_0222B3A4(param0, param2);
    ScheduleBgTilemapBufferTransfer(param2->bgConfig, 2U);
}
