#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "overlay_85.h"
#include "unk_0205B3DC.h"
#include "bg_window.h"
#include "heap.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "msgdata.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

extern const u16 ov85_021EA8E0[5];

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
void ov85_021EA368(UnkStruct_ov109_021D5140 *param0);
BOOL ov85_021EA398(OverlayManager *appMan);
void sub_02038C1C(int maxNumConnection);
int sub_02037454(void);
u16 sub_0203769C(void);
void ov85_021EA39C(UnkStruct_ov109_021D5140 *param0, int param1);

void ov85_021EA324(NNSG2dCharacterData *param0[2], NNSG2dPaletteData *param1[2], int param2, int param3, int param4) {
    int v0;
    u8 *v1, *v2;

    v0 = SpriteToUnionRoomAvatarIdx(param4, param3);

    v1 = (u8 *)param0[1]->pRawData;
    v2 = (u8 *)param1[1]->pRawData;

    GX_LoadOBJ(&v1[(3 * 4 * 4) * v0 * 0x20], ov85_021EA8E0[param2], (3 * 4 * 4) * 0x20);
    GX_LoadOBJPltt(&v2[v0 * 32], (param2 + 7) * 32, 32);
}

void ov85_021EA368(UnkStruct_ov109_021D5140 *param0) {
    Heap_Free(param0->unk_3A8[0]);
    Heap_Free(param0->unk_3A8[1]);
    Heap_Free(param0->unk_398[0]);
    Heap_Free(param0->unk_398[1]);
}

BOOL ov85_021EA398(OverlayManager *appMan) {
    return TRUE;
}

void ov85_021EA39C(UnkStruct_ov109_021D5140 *param0, int param1) {
    if (sub_0203769C() == 0) {
        if (param1 == -1) {
            sub_02038C1C(1);
        } else {
            int v0 = sub_02037454() + param1;

            if (v0 > 5) {
                v0 = 5;
            }

            sub_02038C1C(v0);
        }

        if (param1 == -1) {
            param0->unk_4AB5 = 2;
        } else if (param1 == 0) {
            param0->unk_4AB5 = 1;
        } else {
            param0->unk_4AB5 = 0;
        }
    }
}
