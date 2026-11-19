#include <dwc.h>

#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "mystery_gift.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_mystery_gift_app_data.h"

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

void ov74_02236ABC(void);

extern s32 ov74_0223E304;

void ov74_02236A78(MysteryGiftAppData *param0, s32 param1, s32 *param2, s32 param3, s32 param4);

void ov74_02236A78(MysteryGiftAppData *param0, s32 param1, s32 *param2, s32 param3, s32 param4) {
    *(s32 *)((u8 *)(&ov74_0223E304) + 4) = 0;
    *(s32 *)((u8 *)(&ov74_0223E304) + 8) = param1;
    param0->unk_16B0 = param3;
    *param2 = 0x1013;
    if (DWC_NdCleanupAsync(ov74_02236ABC) == 0) {
        *param2 = param4;
    }
}
