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
	.include "unk_02067A60.inc"
	.include "global.inc"

	.text

	.rodata

	; Movement data
	.global _020FE7AC
_020FE7AC:
	.short 73, 1
	.short 33, 1
	.short 74, 1
	.short 254, 0
