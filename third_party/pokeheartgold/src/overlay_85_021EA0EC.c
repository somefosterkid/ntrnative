#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "unk_0200A090.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

u32 ov85_021EA398(void *);

void ov85_021EA0EC(UnkStruct_ov109_021D5140 *arg0, s32 arg1, s32 arg2);

int ov85_021EA17C(int param0);
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
void ov85_021EA19C(UnkStruct_ov109_021D5140 *param0);

void ov85_021EA0EC(UnkStruct_ov109_021D5140 *arg0, s32 strno, s32 arg2) {
    String *string;

    string = String_New(0xB4U, HEAP_ID_102);
    ReadMsgDataIntoString(arg0->unk_38, strno, string);
    StringExpandPlaceholders(arg0->unk_34, arg0->unk_54, string);
    String_Delete(string);
    FillWindowPixelBuffer(&arg0->unk_35C, 0xFU);
    DrawFrameAndWindow2(&arg0->unk_35C, 0, 1U, 0xAU);
    if (arg2 == 0) {
        arg0->unk_5C = (s32)AddTextPrinterParameterized(&arg0->unk_35C, FONT_MESSAGE, arg0->unk_54, 0U, 0U, ov85_021EA398(arg0), NULL);
        return;
    }
    AddTextPrinterParameterized(&arg0->unk_35C, FONT_MESSAGE, arg0->unk_54, 0U, 0U, 0U, NULL);
    arg0->unk_5C = 0xFF;
}

int ov85_021EA17C(int param0) {
    if (param0 == 0xff) {
        return 1;
    }

    if (TextPrinterCheckActive(param0) == 0) {
        return 1;
    }

    return 0;
}

void ov85_021EA19C(UnkStruct_ov109_021D5140 *param0) {
    ClearFrameAndWindow2(&param0->unk_35C, 0);
}
