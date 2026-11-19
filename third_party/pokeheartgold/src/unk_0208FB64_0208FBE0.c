#include "global.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_0208FB64.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "msgdata.h"
#include "game_stats.h"

struct GameStats;

struct JournalEntry;

struct Menu;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_020961E8_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;


typedef struct JournalEntry JournalEntry;





typedef struct Menu Menu;

void ov73_021E705C(UnkStruct_020961E8 *param0, int param1, u32 param2);

void sub_0208FBE0(int param0, int param1, void *param2, void *param3) {
    UnkStruct_020961E8 *v0 = (UnkStruct_020961E8 *)param3;
    ov73_021E705C(v0, 2, 0);
}
