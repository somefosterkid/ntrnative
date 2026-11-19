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

typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_ov64_0223221C;

typedef struct {
    UnkStruct_ov64_0223221C unk_00;
    Window unk_08;
    UnkStruct_ov63_0222BB38 *unk_18[8];
} UnkStruct_ov64_02230074;

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

void ov43_0222C788(UnkStruct_ov64_02230074 *param0, PalPadGfx *param1, const UnkStruct_ov64_0223221C *param2, u32 param3, u32 param4);

s32 ov43_0222A24C();

extern u16 ov43_0222ED74;

extern UnkStruct_ov64_0223221C ov43_0222EE08;

extern s32 ov43_0222EEB0;

extern u16 ov43_0222EEE0;

void ov43_0222B944(s32 arg0, void *arg1, s32 arg2, enum HeapID arg3);

void ov43_0222B944(s32 arg0, void *arg1, s32 arg2, enum HeapID arg3) {
    SpriteTemplate tmpl;
    s32 sp28;
    u16 *sp24;
    s32 sp20;
    enum HeapID sp1C;
    s32 sp18;
    s32 var_r7;
    u16 *var_r5;
    s32 var_r4;
    s32 temp_r0;
    s32 temp_r0_2;
    u8 *var_r6;
    sp18 = arg0;
    sp28 = 0;
    var_r4 = sp18;
    var_r5 = &ov43_0222ED74;
    var_r6 = (u8 *)&ov43_0222EE08;
    var_r7 = arg2;
    sp1C = arg3;
    var_r4 += 8;
    do {
        ov43_0222C788((UnkStruct_ov64_02230074 *)var_r4, (PalPadGfx *)var_r7, (const UnkStruct_ov64_0223221C *)var_r6, (u32)(*var_r5), (u32)sp1C);
        var_r5 += 1;
        var_r6 += 6;
        temp_r0 = sp28 + 1;
        var_r4 += (s32)sizeof(UnkStruct_ov64_02230074);
        sp28 = temp_r0;
    } while (temp_r0 < 3);
    *((UnkStruct_ov42_02227060 **)(((u8 *)sp18) + 0xB4)) = ov42_02227060(*((SpriteList **)(((u8 *)var_r7) + 4)), 0L, 0x10, sp1C);
    ov42_02227228(*((UnkStruct_ov42_02227060 **)(((u8 *)sp18) + 0xB4)), 1U, 2, sp1C);
    ov43_0222A24C(*((s32 *)var_r7), sp18 + 0xB8, 1, 0x15, 21, 9, 2, 0xB, 0x14F, 0);
    var_r4 = (s32)NewString_ReadMsgData(*((MsgData **)(((u8 *)var_r7) + 0x54)), 7);
    AddTextPrinterParameterizedWithColor((struct Window *)(sp18 + 0xB8), FONT_UNK_4, (String *)var_r4, 0U, 0U, 0xFFU, 0x10F00U, 0L);
    String_Delete((String *)var_r4);
    sp20 = 0;
    var_r6 = (u8 *)sp18;
    sp24 = &ov43_0222EEE0;
    do {
        ((Ov43App_0222B944 *)var_r6)->unk_E4 = ListMenuItems_New(4U, sp1C);
        var_r5 = sp24;
        var_r4 = 0;
loop_4:
        ListMenuItems_AppendFromMsgData(((Ov43App_0222B944 *)var_r6)->unk_E4, *((MsgData **)(((u8 *)var_r7) + 0x54)), *((s32 *)var_r5), *((s32 *)(((u8 *)var_r5) + 4)));

        var_r4 += 1;
        var_r5 += 4;
        if (var_r4 < 4) {
            goto loop_4;
        }
        var_r6 += 4;
        sp24 += 16;
        temp_r0_2 = sp20 + 1;
        sp20 = temp_r0_2;
    } while (temp_r0_2 < 2);
    ov43_0222A24C(*((s32 *)var_r7), sp18 + 0xC8, 1, 2, 0x13, 0x1B, 4, 0xB, 0x161, 0xF);
    *((String **)(((u8 *)sp18) + 0xD8)) = String_New(0x80U, sp1C);
    tmpl = *(SpriteTemplate *)&ov43_0222EEB0;
    ((u32 *)&tmpl)[0] = *((u32 *)(((u8 *)var_r7) + 4));
    var_r7 += 0x88;
    ((u32 *)&tmpl)[1] = var_r7;
    tmpl.heapID = sp1C;
    *((Sprite **)(((u8 *)sp18) + 0xF8)) = Sprite_CreateAffine(&tmpl);
    Sprite_SetDrawFlag(*((Sprite **)(((u8 *)sp18) + 0xF8)), 0);
    sp18 += 0xF8;
}
