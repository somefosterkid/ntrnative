#include "global.h"

#include "player_avatar.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_02035900.h"
#include "wfcfacility_selector_main.h"
#include "overlay_80_022384D8.h"


s32 sub_02096A34(void *arg0);

s32 sub_02096A34(void *arg0) {
    FrontierSave *frontierSave;
    u32 param0;

    *((s16 *)&((WFCFacilitySelector_main *)arg0)->commBuffer[0]) = (s16) ((WFCFacilitySelector_main *)arg0)->selectedFacility;
    param0 = ov80_022385D8(((WFCFacilitySelector_main *)arg0)->selectedFacility);
    frontierSave = Save_Frontier_GetStatic(*(SaveData **)arg0);
    *((u16 *)&((WFCFacilitySelector_main *)arg0)->commBuffer[1]) = FrontierSave_GetStat(frontierSave, (s32)param0, (s32)sub_0205C268(param0));
    if (sub_02037030(0x39, arg0 + 8, 0x28) == 1) {
        return 1;
    }
    return 0;
}
