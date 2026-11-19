#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "macros.inc"
	.include "unk_0208B1AC.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text

	.rodata
	.data

	.global _021105B0
_021105B0:
	.byte 0xB7, 0x37
	.byte 0xDF, 0x3F
	.byte 0xB7, 0x57
	.byte 0xDF, 0x5F
	.byte 0xB7, 0x77
	.byte 0xDF, 0x7F
