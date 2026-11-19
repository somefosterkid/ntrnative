#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_47.h"
#include "sys_task.h"
#include "overlay_47_unk_struct_ov68_0225_d128.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_45_unk_struct_ov66_02231_e94.h"
#include "overlay_45_unk_struct_ov66_0223177_c_t.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "struct_defs/resdat_narc_structs.h"
#include "list_menu_2d.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct PasswordWordBank PasswordWordBank;

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

struct PasswordWordBank {
    u32 length;
    u32 *words;
};

typedef struct {
    MsgData *unk_00[9];
    MessageFormat *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    String *unk_10;
    String *unk_14[3];
} UnkStruct_ov68_0225DB8C;

typedef struct {
    Window unk_00[1];
    s32 unk_10;
    Sprite *unk_14;
    SpriteResource *unk_18[4];
    u16 unk_28;
    u16 unk_2A;
    UnkStruct_ov68_0225DB8C unk_2C;
} UnkStruct_ov68_0225D0F8;

void ov47_02258DD0(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, u32 param3);

void ov47_02258DA8(UnkStruct_ov68_0225D0F8 *param_1, UnkStruct_ov68_0225CB70 *param_2, UnkStruct_ov68_0225C91C *param_3, u32 param_4, u32 param_5);

SysTask *ov47_0225912C(SysTaskFunc callback, void *param, UnkStruct_ov68_0225C91C *priority);

void ov47_02258DA8(UnkStruct_ov68_0225D0F8 *param_1, UnkStruct_ov68_0225CB70 *param_2, UnkStruct_ov68_0225C91C *param_3, u32 param_4, u32 param_5) {
    ov47_02258DD0(param_1, param_2, param_3, param_5);
    return;
}

typedef struct ListMenuCursor ListMenuCursor;

typedef u32 TextColor;

struct ListMenuCursor {
    TextColor color;
    String *string;
};

typedef struct {
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    int unk_08;
} UnkStruct_ov66_02231374;

BOOL ov47_02258F48(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, u32 param5);

void ov47_02258DB4(UnkStruct_ov68_0225D0F8 *arg0, UnkStruct_ov68_0225CB70 *arg1, UnkStruct_ov68_0225C91C *arg2, UnkStruct_ov68_0225D128 *arg3, UnkStruct_ov66_02231374 *arg4, u32 arg5);

void ov47_02258DB4(UnkStruct_ov68_0225D0F8 *arg0, UnkStruct_ov68_0225CB70 *arg1, UnkStruct_ov68_0225C91C *arg2, UnkStruct_ov68_0225D128 *arg3, UnkStruct_ov66_02231374 *arg4, u32 arg5) {
    ov47_02258F48(arg0, arg1, arg2, arg3, arg4, arg5);
}

SysTask *ov47_02258DC8(SysTaskFunc callback, void *param, void *priority) {
    return ov47_0225912C(callback, param, priority);
}
