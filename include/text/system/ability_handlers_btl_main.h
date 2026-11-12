// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
// build/include/generated/text/system/ability_handlers_btl_main.h: the lines in use so far.
#ifndef BWPORT_TEXT_SYSTEM_ABILITY_HANDLERS_BTL_MAIN_H
#define BWPORT_TEXT_SYSTEM_ABILITY_HANDLERS_BTL_MAIN_H

#include "types.h"
#include "constants/version.h"

#ifdef BLACK2
// Black 2's, not checked against Black
#define AbilityHandlersBtlMain_Text_Var1 24
#define AbilityHandlersBtlMain_Text_Var0 25
#else
// Black's and White's: the colors' names, White's and Black's (White's code loads 14 where Black's loads 15)
#define AbilityHandlersBtlMain_Text_Var1 14
#define AbilityHandlersBtlMain_Text_Var0 15
#endif

// The name a player gets when a check refuses theirs: the version's color
#if GAME_VERSION == VERSION_WHITE
#define AbilityHandlersBtlMain_Text_VersionName AbilityHandlersBtlMain_Text_Var1
#else
#define AbilityHandlersBtlMain_Text_VersionName AbilityHandlersBtlMain_Text_Var0
#endif

#endif // BWPORT_TEXT_SYSTEM_ABILITY_HANDLERS_BTL_MAIN_H
