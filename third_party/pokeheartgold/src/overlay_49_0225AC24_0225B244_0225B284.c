#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"

struct MsgData;

struct MessageFormat;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_ov65_0222F6EC;

int ov45_0222D7CC(int param0, int param1);

UnkStruct_ov65_0222F6EC *sub_020392D8(void);

typedef struct {
    Window unk_00;
} UnkStruct_ov70_0225F114;

void ov49_0225B24C(UnkStruct_ov70_0225F114 *param0, const String *param1);

String *ov49_0225B388(UnkStruct_ov70_0225F208 *param0, int param1, u32 param2);

void ov49_0225B3A8(UnkStruct_ov70_0225F208 *param0, u32 param1, u32 param2, u32 param3, int param4);

void ov49_0225B284(UnkStruct_ov70_0225F114 *param0, UnkStruct_ov70_0225F208 *param1);

void ov49_0225B284(UnkStruct_ov70_0225F114 *param0, UnkStruct_ov70_0225F208 *param1) {
    u32 v0;
    String *v1;
    UnkStruct_ov65_0222F6EC *v2 = sub_020392D8();
    v0 = ov45_0222D7CC(v2->unk_00, v2->unk_04);

    ov49_0225B3A8(param1, v2->unk_00, 5, 0, 2);
    v1 = ov49_0225B388(param1, 2, v0);
    ov49_0225B24C(param0, v1);
}
