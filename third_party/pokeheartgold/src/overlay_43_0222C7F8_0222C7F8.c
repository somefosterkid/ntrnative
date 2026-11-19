#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_41_sprite_resource_collection.h"
#include "struct_defs/resdat_narc_structs.h"
#include "msgdata.h"
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
    void *unk_00[2];
    NNSG2dScreenData *unk_08[2];
} UnkStruct_ov64_022300E4;

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

void ov43_0222C7F8(UnkStruct_ov64_02230074 *param0, PalPadGfx *param1, UnkStruct_ov64_022300E4 *param2);

void ov43_0222C7F8(UnkStruct_ov64_02230074 *param0, PalPadGfx *param1, UnkStruct_ov64_022300E4 *param2) {
    NNSG2dScreenData *pNVar1;
    s32 iVar2;
    s32 iVar3;

    iVar3 = (s32)(param0->unk_00).unk_02;
    if (iVar3 < 0) {
        iVar3 = -iVar3;
    }
    iVar2 = (s32)(param0->unk_00).unk_00;
    if (iVar2 < 0) {
        iVar2 = -iVar2;
    }
    pNVar1 = param2->unk_08[0];
    LoadRectToBgTilemapRect(param1->bgConfig, 2, pNVar1->rawData, (u8)iVar2, (u8)iVar3, (u8)(((u32)pNVar1->screenWidth) >> 3), (u8)(((u32)pNVar1->screenHeight) >> 3));
    ScheduleBgTilemapBufferTransfer(param1->bgConfig, 2);
    return;
}
