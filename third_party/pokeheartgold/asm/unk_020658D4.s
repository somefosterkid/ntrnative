#include "constants/sndseq.h"
#include "constants/species.h"
#include "constants/maps.h"
#include "constants/pokemon.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/items.h"
#include "constants/std_script.h"
#include "fielddata/script/scr_seq/event_D24R0204.h"
#include "constants/field_move_response.h"
	.include "macros.inc"
	.include "unk_020658D4.inc"
	.include "global.inc"

	.public sub_020663B4
	.public sub_020663E4
	.public sub_02066420
	.public sub_0206642C
	.public sub_02066444
	.public sub_020664D8
	.public sub_02065A4C
	.public sub_02065B70
	.public sub_02065BE8
	.public sub_02065C2C
	.public sub_02065C48
	.public sub_02065C90
	.public sub_02065CD0
	.public sub_02066064
	.public sub_020660A0
	.public _020FE404
	.public _020FE414
	.public _020FE424
	.text

	.rodata

	.global _020FE3CC
_020FE3CC:
	.word sub_020663B4
	.word sub_020663E4
	.global _020FE3D4
_020FE3D4:
	.word sub_02066064
	.word sub_020660A0
