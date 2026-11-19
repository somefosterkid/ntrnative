#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"
#include "list_menu_2d.h"
#include "overlay_85.h"
#include "unk_0200A090.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

void ov85_021E9458(UnkStruct_ov109_021D5140 *param0, int param1);

struct GameStats;
struct JournalEntry;
struct ListMenu2D;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_0205C22C_t;
struct UnkStruct_ov109_021D0F70_t;
typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;
typedef struct JournalEntry JournalEntry;
typedef struct ListMenu2D ListMenu2D;
void ov85_021E943C(UnkStruct_ov109_021D5140 *param0);
int ov85_021EA17C(int param0);
int ov85_021E9468(UnkStruct_ov109_021D5140 *param0, int param1);
u16 sub_0203769C(void);
BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);
int ov85_021E948C(UnkStruct_ov109_021D5140 *param0, int param1);
extern WindowTemplate ov85_021EA8D8;

void ov85_021E9458(UnkStruct_ov109_021D5140 *param0, int param1) {
    param0->unk_3B8 = 0x1E;
    param0->unk_3BC = param1;
}

int ov85_021E9468(UnkStruct_ov109_021D5140 *param0, int param1) {
    if (ov85_021EA17C(param0->unk_5C)) {
        param0->unk_3B8 = param0->unk_3BC;
    }

    ov85_021E943C(param0);

    return param1;
}

int ov85_021E948C(UnkStruct_ov109_021D5140 *param0, int param1) {
    if (sub_0203769C() == 0) {
        int v0 = 0;

        sub_02096D4C(param0->unk_10, 7, &v0, 1);
    }

    param0->unk_3B8 = 3;
    ov85_021E943C(param0);

    return param1;
}

s32 ov85_021E94BC(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    arg0->unk_394 = Std_CreateYesNoMenu(arg0->unk_14, &ov85_021EA8D8, 0x1FU, 0xBU, HEAP_ID_102);
    arg0->unk_3B8 = 5;
    ov85_021E943C(arg0);
    return arg1;
}
