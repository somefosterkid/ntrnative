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
#include "sys_task.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

struct OverlayManager;

struct GameStats;

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void *OverlayManager_GetData(OverlayManager *appMan);

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

void ov85_021E5AF0(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E67F4(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E74AC(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E75C8(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E7798(UnkStruct_ov109_021D0F70 *param0);

extern int (*const ov85_021EA800[54])(UnkStruct_ov109_021D0F70 *);

int ov85_021E5AAC(OverlayManager *appMan, int *param1);

int ov85_021E5AAC(OverlayManager *appMan, int *param1) {
    int v0;
    UnkStruct_ov109_021D0F70 *v1 = OverlayManager_GetData(appMan);

    ov85_021E74AC(v1);

    do {
        v0 = ov85_021EA800[v1->unk_00](v1);
    } while (v0 == 1);

    if (v0 == 2) {
        return 1;
    }

    ov85_021E75C8(v1);
    ov85_021E7798(v1);
    ov85_021E67F4(v1);
    ov85_021E5AF0(v1);

    return 0;
}
