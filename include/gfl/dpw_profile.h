// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/dpw_profile.h.
#ifndef BWPORT_GFL_DPW_PROFILE_H
#define BWPORT_GFL_DPW_PROFILE_H

#include "types.h"
#include "dpw/dpw_tr.h"
#include "struct_decls.h"

// dpw_profile.c, named after what it does: the one Game Freak function between nhttp_rap_evilcheck.c and the HTTP
// library, which fills the profile that the trade server keeps from the player's

// Fills a profile from the player's, with no e-mail address (pokebw2's DpwProfile_Fill)
void func_ov114_021b9b00(Dpw_Common_Profile *profile, PlayerInfo *playerInfo);

#endif // BWPORT_GFL_DPW_PROFILE_H
