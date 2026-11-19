#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"
#include "gf_gfx_loader.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "msgdata.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

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

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

typedef struct JournalEntry JournalEntry;

typedef struct ListMenu2D ListMenu2D;

u16 sub_0203769C(void);

void ov85_021EA324(NNSG2dCharacterData *param0[2], NNSG2dPaletteData *param1[2], int param2, int param3, int param4);

void ov85_021EA1AC(UnkStruct_ov109_021D5140 *param0);

void ov85_021EA2C0(UnkStruct_ov109_021D5140 *arg0, NARC *arg1);

void ov85_021EA1AC(UnkStruct_ov109_021D5140 *param0) {
    int v0, v1 = 0;

    for (v0 = 0; v0 < 5; v0++) {
        switch (param0->unk_400[v0]) {
        case 0:
            break;
        case 1: {
            int v2, v3;

            ((param0->unk_3D8[v0][0] != 0L) ? (void)0 : GF_AssertFail());

            v3 = PlayerProfile_GetTrainerGender(param0->unk_3D8[v0][0]);
            v2 = PlayerProfile_GetAvatar(param0->unk_3D8[v0][0]);

            if (sub_0203769C() == v0) {
                Sprite_SetAnimCtrlSeq(param0->unk_29C[v0 + 1], 38 + v3 * 2);
            } else {
                ov85_021EA324(param0->unk_3A0, param0->unk_3B0, v0, v2, v3);
                Sprite_SetAnimCtrlSeq(param0->unk_29C[v0 + 1], 27 + v0 * 2);
            }
        }
            Sprite_SetDrawFlag(param0->unk_29C[v0 + 1], 1);
            param0->unk_400[v0] = 2;
            v1 = 1;
            break;
        case 2:
            break;
        case 3:
            if (sub_0203769C() == v0) {
                int v4 = PlayerProfile_GetTrainerGender(param0->unk_3D8[v0][0]);

                Sprite_SetAnimCtrlSeq(param0->unk_29C[v0 + 1], 38 + v4 * 2 + 1);
            } else {
                Sprite_SetAnimCtrlSeq(param0->unk_29C[v0 + 1], 27 + v0 * 2 + 1);
            }

            param0->unk_400[v0] = 0;
            break;
        }
    }

    if (v1) {
        PlaySE(1614);
    }
}

void ov85_021EA2C0(UnkStruct_ov109_021D5140 *arg0, NARC *narc) {
    arg0->unk_3A8[0] = GfGfxLoader_GetPlttData(NARC_a_1_0_0, 9, arg0->unk_3B0, HEAP_ID_102);
    arg0->unk_3A8[1] = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 7, &arg0->unk_3B0[1], HEAP_ID_102);
    arg0->unk_398[0] = GfGfxLoader_GetCharData(NARC_a_1_0_0, 0x2B, 1, arg0->unk_3A0, HEAP_ID_102);
    arg0->unk_398[1] = GfGfxLoader_GetCharDataFromOpenNarc(narc, 9, 1, &arg0->unk_3A0[1], HEAP_ID_102);
}
