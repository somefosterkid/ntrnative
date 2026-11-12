// Black's code at ov114 0x021b9b00-0x021b9b4c. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
// (the same functions in the same order); the name is that file's.

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "constants/language.h"
#include "constants/version.h"
#include "gfl/dpw_profile.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/nhttp.h"
#include "dwc/base.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/str_tool.h"
#include "gs/nonport.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/blact.h"

#pragma thumb on
void func_ov114_021b9b00(Dpw_Common_Profile *profile, PlayerInfo *playerInfo) {
    MI_CpuFill8(profile, 0, sizeof(Dpw_Common_Profile));
    profile->version = GAME_VERSION;
    profile->language = GAME_LANGUAGE;
    profile->country = UnityTowerVisitor_GetCountry(playerInfo);
    profile->region = UnityTowerVisitor_GetProvince(playerInfo);
    profile->playerId = getIDAsUInt(playerInfo);
    wcharsncpy(GetPlayerName(playerInfo), profile->playerName, 8);
    profile->unk18 = 0;
    profile->mailAddr[0] = '\0';
    profile->unk5C = 0;
}
