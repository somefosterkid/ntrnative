#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "font_types_def.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "render_window.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "text.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

struct GameStats;

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

void BufferItemName(MessageFormat *template, u32 idx, u32 item);

void ov85_021E7274(UnkStruct_ov109_021D0F70 *param0, u32 param1, u32 param2);

void ov85_021E72E8(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E7274(UnkStruct_ov109_021D0F70 *param0, u32 param1, u32 param2) {
    String *v0;
    UnkStruct_ov109_021D24F8 *v1 = &param0->unk_C9C;
    Window *v2 = &v1->unk_0C[0];

    BufferItemName(v1->unk_08, 0, param2);

    v0 = String_New(0x100, HEAP_ID_102);

    ReadMsgDataIntoString(v1->unk_04, param1, v0);
    StringExpandPlaceholders(v1->unk_08, v1->unk_6C, v0);
    String_Delete(v0);
    DrawFrameAndWindow2(v2, 1, 1 + 9, 14);
    FillWindowPixelBuffer(v2, 15);
    AddTextPrinterParameterized(v2, FONT_MESSAGE, v1->unk_6C, 0, 0, 0xFF, 0L);
    ScheduleWindowCopyToVram(v2);
}

void ov85_021E72E8(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D24F8 *v0 = &param0->unk_C9C;
    Window *v1 = &v0->unk_0C[0];

    ClearFrameAndWindow2(v1, 1);
    FillWindowPixelBuffer(v1, 0);
    ScheduleWindowCopyToVram(v1);
}
