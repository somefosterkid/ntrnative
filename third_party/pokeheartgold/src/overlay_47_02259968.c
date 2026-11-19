#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "unk_0200A090.h"
#include "font.h"
#include "text.h"
#include "overlay_45_wifi_plaza_system.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"
#include "msgdata.h"

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct UnkStruct_ov66_0222DFF8_t;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    String *unk_10;
    String *unk_14[3];
} UnkStruct_ov68_0225DB8C;

typedef struct {
    Window unk_00[6];
    u16 unk_60;
    u16 unk_62;
    s32 unk_64;
    s32 unk_68[3];
    s32 unk_74[3];
    void *unk_80;
    NNSG2dCharacterData *unk_84;
    UnkStruct_ov68_0225DB8C unk_88;
    UnkStruct_ov68_0225DB8C unk_A8;
    UnkStruct_ov68_0225DC74 unk_C8;
    UnkStruct_ov68_0225DC74 unk_DC;
} UnkStruct_ov68_0225D388;

typedef struct {
    MsgData *unk_00[9];
    MessageFormat *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

void ov47_02259D24(UnkStruct_ov68_0225DB8C *param0);

void ov47_02259968(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2);

String *ov47_02258CEC(UnkStruct_ov68_0225CB70 *, s32, s32);
void ov47_0225999C(Window *arg0, void *arg1, s32 arg2, u32 arg3);

void ov47_02259968(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2) {
    {
        ov47_02259D24(&param0->unk_88);
        ov47_02259D24(&param0->unk_A8);
    }

    Heap_Free(param0->unk_80);
    param0->unk_80 = NULL;

    {
        int v0;

        for (v0 = 0; v0 < 6; v0++) {
            RemoveWindow(&param0->unk_00[v0]);
        }
    }
}

void ov47_0225999C(Window *window, void *arg1, s32 arg2, u32 color) {
    String *string;

    FillWindowPixelBuffer(window, 0U);
    string = ov47_02258CEC(arg1, 0, arg2);
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, (u32)(0x90 - FontID_String_GetWidth(FONT_MESSAGE, string, 0U)) >> 1, 4U, 0xFFU, color, NULL);
    ScheduleWindowCopyToVram(window);
}
