#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "list_menu_items.h"
#include "msgdata.h"
#include "overlay_42.h"
#include "pm_string.h"
#include "sprite.h"
#include "text.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_app_0222b944.h"
#include "overlay_41_sprite_resource_collection.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"

typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

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

void ov43_0222C788(PalPadRosterPage *page, PalPadGfx *gfx, const PalPadPanelLayout *layout, u32 baseTile, u32 heapID);

void ov43_0222A24C(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 left, u8 top, u8 width, u8 height, u8 palette, u16 baseTile, u8 colorMode);

extern const u16 ov43_0222ED74[3];

extern const PalPadPanelLayout ov43_0222EE08[3];

extern const SpriteTemplate ov43_0222EEB0;

extern const s32 ov43_0222EEE0[2][4][2];

void ov43_0222B944(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    SpriteTemplate tmpl;
    String *string;
    int i, j;

    for (i = 0; i < 3; i++) {
        ov43_0222C788(&roster->pages[i], gfx, &ov43_0222EE08[i], ov43_0222ED74[i], heapID);
    }
    roster->trainerIcons = ov42_02227060(gfx->spriteList, NULL, 0x10, heapID);
    ov42_02227228(roster->trainerIcons, 1, 2, heapID);
    ov43_0222A24C(gfx->bgConfig, &roster->returnWindow, 1, 0x15, 21, 9, 2, 0xB, 0x14F, 0);
    string = NewString_ReadMsgData(gfx->msgData, 7);
    AddTextPrinterParameterizedWithColor(&roster->returnWindow, FONT_UNK_4, string, 0, 0, 0xFF, 0x10F00, NULL);
    String_Delete(string);
    for (i = 0; i < 2; i++) {
        roster->menuItems[i] = ListMenuItems_New(4, heapID);
        for (j = 0; j < 4; j++) {
            ListMenuItems_AppendFromMsgData(roster->menuItems[i], gfx->msgData, ov43_0222EEE0[i][j][0], ov43_0222EEE0[i][j][1]);
        }
    }
    ov43_0222A24C(gfx->bgConfig, &roster->msgWindow, 1, 2, 0x13, 0x1B, 4, 0xB, 0x161, 0xF);
    roster->msgText = String_New(0x80, heapID);
    tmpl = ov43_0222EEB0;
    tmpl.spriteList = gfx->spriteList;
    tmpl.header = &gfx->spriteHeader;
    tmpl.heapID = heapID;
    roster->deleteEffect = Sprite_CreateAffine(&tmpl);
    Sprite_SetDrawFlag(roster->deleteEffect, 0);
}
