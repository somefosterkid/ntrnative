#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

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
#include "constants/heap.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "player_data.h"
#include "game_stats.h"
#include "overlay_87_scratch_off_card_app.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"


struct GameStats;

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_0209C194_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;


typedef struct JournalEntry JournalEntry;

typedef struct MsgData MsgData;

void DestroyMsgData(MsgData *loader);

typedef struct MessageFormat MessageFormat;

void MessageFormat_Delete(MessageFormat *template);

void ov85_021E7350(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E7148(UnkStruct_ov109_021D0F70 *param0);

PlayerProfile *sub_02034818(int netId);
u16 sub_0203769C(void);
void BufferPlayersName(MessageFormat *template, u32 idx, const PlayerProfile *playerInfo);
void StringExpandPlaceholders(const MessageFormat *template, String *dst, const String *fmtString);
void ov85_021E71EC(UnkStruct_ov109_021D0F70 *param0, u32 param1, const PlayerProfile *param2);

void ov85_021E7148(UnkStruct_ov109_021D0F70 *param0) {
    int v0;
    UnkStruct_ov109_021D24F8 *v1 = &param0->unk_C9C;

    for (v0 = 0; v0 < 1; v0++) {
        ClearWindowTilemapAndCopyToVram(&v1->unk_0C[v0]);
        RemoveWindow(&v1->unk_0C[v0]);
    }

    ov85_021E7350(param0);

    DestroyMsgData(v1->unk_04);
    MessageFormat_Delete(v1->unk_08);
    String_Delete(v1->unk_6C);

    for (v0 = 0; v0 < 5; v0++) {
        String_Delete(param0->unk_2C.unk_6C[v0]);
    }
}

void ReadMsgDataIntoString(const MsgData *loader, u32 entryID, String *string);

void ov85_021E7194(UnkStruct_ov109_021D0F70 *param0, u32 param1);

void ov85_021E7194(UnkStruct_ov109_021D0F70 *param0, u32 param1) {
    UnkStruct_ov109_021D24F8 *v0 = &param0->unk_C9C;
    Window *v1 = &v0->unk_0C[0];

    FillWindowPixelBuffer(v1, 15);
    DrawFrameAndWindow2(v1, 1, 1 + 9, 14);
    FillWindowPixelBuffer(v1, 15);
    ReadMsgDataIntoString(v0->unk_04, param1, v0->unk_6C);
    AddTextPrinterParameterized(v1, FONT_MESSAGE, v0->unk_6C, 0, 0, 0xFF, 0L);
    ScheduleWindowCopyToVram(v1);
}

void ov85_021E71EC(UnkStruct_ov109_021D0F70 *param0, u32 param1, const PlayerProfile *param2) {
    String *v0;
    UnkStruct_ov109_021D24F8 *v1 = &param0->unk_C9C;
    Window *v2 = &v1->unk_0C[0];

    BufferPlayersName(v1->unk_08, 1, sub_02034818(sub_0203769C()));
    BufferPlayersName(v1->unk_08, 2, param2);

    v0 = String_New(0x100, HEAP_ID_102);

    ReadMsgDataIntoString(v1->unk_04, param1, v0);
    StringExpandPlaceholders(v1->unk_08, v1->unk_6C, v0);
    String_Delete(v0);
    DrawFrameAndWindow2(v2, 1, 1 + 9, 14);
    FillWindowPixelBuffer(v2, 15);
    AddTextPrinterParameterized(v2, FONT_MESSAGE, v1->unk_6C, 0, 0, 0xFF, 0L);
    ScheduleWindowCopyToVram(v2);
}
