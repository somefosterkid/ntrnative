#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/species.h"
	.include "macros.inc"
	.include "unk_020915B0.inc"
	.include "global.inc"

	.public sub_02091634
	.text

	.rodata

	.public gApp_MainMenu_SelectOption_NintendoWFCSetup
gApp_MainMenu_SelectOption_NintendoWFCSetup:
	.word sub_02091634, 0, 0, 0xFFFFFFFF
