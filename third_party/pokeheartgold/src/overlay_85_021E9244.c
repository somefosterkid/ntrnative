#include "global.h"
#include "overlay_85_ov109_021D5140.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "text.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "msgdata.h"
#include "game_stats.h"
#include "unk_02037C94.h"

typedef u32 TextColor;

void ov85_021E9244(Window *param0, String *param1, int param2);

struct GameStats;
struct JournalEntry;
struct Menu;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_0205C22C_t;
struct UnkStruct_ov109_021D5140_t;
typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;
typedef struct JournalEntry JournalEntry;
typedef struct Menu Menu;
int sub_02037454(void);
u16 sub_0203769C(void);
void ov85_021EA0EC(UnkStruct_ov109_021D5140 *param0, int param1, int param2);
void ov85_021E9458(UnkStruct_ov109_021D5140 *param0, int param1);
int ov85_021E92B8(UnkStruct_ov109_021D5140 *param0, int param1);

void ov85_021E9244(Window *param0, String *param1, int param2) {
    int v0 = FontID_String_GetWidth(FONT_MESSAGE, param1, 0);
    int v1 = (26 * 8 - v0) / 2;

    FillWindowPixelBuffer(param0, 0x0);
    AddTextPrinterParameterizedWithColor(param0, FONT_MESSAGE, param1, v1, 1, param2, ((TextColor)(((1 & 0xFF) << 16) | ((4 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
}

void ov85_021E9288(UnkStruct_ov109_021D5140 *param0);

void ov85_021E9288(UnkStruct_ov109_021D5140 *param0) {
    RemoveWindow(&param0->unk_30C[0]);
    RemoveWindow(&param0->unk_37C);
    RemoveWindow(&param0->unk_36C);
    RemoveWindow(&param0->unk_35C);
    return;
}

int ov85_021E92B8(UnkStruct_ov109_021D5140 *param0, int param1) {
    sub_020398D4(0, 1);

    if (sub_0203769C() == 0) {
        if (sub_02037454() >= 2) {
            ov85_021EA0EC(param0, 2, 0);
        }
    } else {
        ov85_021EA0EC(param0, 8, 0);
    }

    ov85_021E9458(param0, 3);

    return param1;
}
