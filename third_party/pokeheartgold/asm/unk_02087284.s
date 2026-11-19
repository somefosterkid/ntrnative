#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "macros.inc"
	.include "unk_02087284.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.public sub_020872DC
	.text

	.rodata

	.global _02102814
_02102814:
	.word 0x61
	.word 0x62
	.word 0x63
	.word 0x64
	.word 0x65
	.word 0x80
	.word 0x85
