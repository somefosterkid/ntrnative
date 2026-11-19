#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "macros.inc"
	.include "unk_02087E70.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.public sub_02087EF8
	.public sub_02087F04
	.public sub_02087F10
	.public sub_02087F2C
	.text

	.rodata

	.public _021028B4
_021028B4:
	.word sub_02087EF8, sub_02087F10, sub_02087F2C, SDK_OVERLAY_OVY_40_ID
	.public _021028C4
_021028C4:
	.word sub_02087F04, sub_02087F10, sub_02087F2C, SDK_OVERLAY_OVY_40_ID
