#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "font.h"
#include "font_types_def.h"
#include "render_window.h"
#include "text.h"
#include "unk_02035900.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "game_stats.h"
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

extern const WindowTemplate *const ov85_021EA5AC[5 + 1];

void ov85_021E730C(UnkStruct_ov109_021D0F70 *param0, int param1);

void ov85_021E7350(UnkStruct_ov109_021D0F70 *param0);
typedef u32 TextColor;
void ov85_021E7380(UnkStruct_ov109_021D0F70 *param0, String *param1, int param2, TextColor param3);
void ov85_021E73D4(UnkStruct_ov109_021D0F70 *param0, String *param1, int param2, int param3);
void ov85_021E7400(UnkStruct_ov109_021D0F70 *param0, int param1);

void ov85_021E730C(UnkStruct_ov109_021D0F70 *param0, int param1) {
    int v0;
    UnkStruct_ov109_021D24F8 *v1 = &param0->unk_C9C;
    const WindowTemplate *v2 = ov85_021EA5AC[param1];

    v1->unk_00 = param1;

    for (v0 = 0; v0 < param1; v0++) {
        AddWindow(param0->unk_D84, &v1->unk_1C[v0], &v2[v0]);
    }
}

void ov85_021E7350(UnkStruct_ov109_021D0F70 *param0) {
    int v0;
    UnkStruct_ov109_021D24F8 *v1 = &param0->unk_C9C;

    for (v0 = 0; v0 < v1->unk_00; v0++) {
        ClearWindowTilemapAndCopyToVram(&v1->unk_1C[v0]);
        RemoveWindow(&v1->unk_1C[v0]);
    }
}

void ov85_021E7380(UnkStruct_ov109_021D0F70 *param0, String *param1, int param2, TextColor param3) {
    UnkStruct_ov109_021D24F8 *v0 = &param0->unk_C9C;
    Window *v1 = &v0->unk_1C[param2];
    DrawFrameAndWindow1(v1, 1, 1, 14);
    FillWindowPixelBuffer(v1, 15);
    AddTextPrinterParameterizedWithColor(v1, FONT_SYSTEM, param1, FontID_String_GetCenterAlignmentX(FONT_SYSTEM, param1, 0, 8 * 8), 0, 0xFF, param3, 0L);
    ScheduleWindowCopyToVram(v1);
}

void ov85_021E73D4(UnkStruct_ov109_021D0F70 *param0, String *param1, int param2, int param3) {
    TextColor v0 = ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0)));

    if (param3 == sub_0203769C()) {
        v0 = ((TextColor)(((3 & 0xFF) << 16) | ((4 & 0xFF) << 8) | ((0 & 0xFF) << 0)));
    }

    ov85_021E7380(param0, param1, param2, v0);
}

void ov85_021E7400(UnkStruct_ov109_021D0F70 *param0, int param1) {
    UnkStruct_ov109_021D24F8 *v0 = &param0->unk_C9C;
    Window *v1 = &v0->unk_1C[param1];

    ClearFrameAndWindow2(v1, 1);
    FillWindowPixelBuffer(v1, 0);
    ScheduleWindowCopyToVram(v1);
}
