#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "constants/heap.h"

#include "overlay_manager.h"
#include "screen_fade.h"
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
#include "overlay_112_pokewalker_app.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"

s32 ov112_021F31D8(u8 *);

extern u32 ov112_021FF124;

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
int ov112_021EF640(UnkStruct_ov109_021D0F70 *param0);

u32 ov112_021EF5AC(Ov112PokewalkerApp *param_1) {
    s32 iVar1;
    OverlayManager *pOVar2;
    BOOL BVar3;

    switch ((u32)param_1->unk_8) {
    case 0:
        BVar3 = IsPaletteFadeFinished();
        if (BVar3 == 1) {
            ov112_021E77E4(param_1);
            iVar1 = ov112_021F31D8(param_1->unk_9DFC);
            if (iVar1 != 0) {
                return 3;
            }
            param_1->unk_8 = 1;
        }
        break;
    case 1:
        pOVar2 = OverlayManager_New((OverlayManagerTemplate *)&ov112_021FF124,
            &param_1->unk_1D750,
            HEAP_ID_154);
        param_1->man = pOVar2;
        param_1->unk_8 = 2;
        break;
    case 2:
        BVar3 = OverlayManager_Run(param_1->man);
        if (BVar3 != 0) {
            OverlayManager_Delete(param_1->man);
            param_1->unk_8 = 3;
        }
        break;
    case 3:
        return 3;
    }
    return 2;
}

s32 ov112_021EF62C(Ov112PokewalkerApp *arg0) {
    ov112_021E7768(arg0);
    arg0->unk_8 = 0;
    arg0->unk_4 = 0xA;
    return 1;
}

int ov112_021EF640(UnkStruct_ov109_021D0F70 *param0) {
    return 2;
}
