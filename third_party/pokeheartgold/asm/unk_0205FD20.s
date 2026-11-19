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
	.include "unk_0205FD20.inc"
	.include "global.inc"

	.public sub_0205FE24
	.public sub_0205FE48
	.public sub_0205FE6C
	.public sub_0205FEA4
	.public sub_02061070
	.text

	.rodata

	; File boundary
	.global _020FD49C
_020FD49C:
	.word -1
	.word  1
	.word  0
	.word  0
	.global _020FD4AC
_020FD4AC:
	.word  0
	.word  0
	.word -1
	.word  1
	.global _020FD4BC
_020FD4BC:
	.word sub_0205B918
	.word sub_0205B8F4
	.word sub_0205B960
	.word sub_0205B93C
	.global _020FD4CC
_020FD4CC:
	.word sub_0205B8F4
	.word sub_0205B918
	.word sub_0205B93C
	.word sub_0205B960
	.global _020FD4DC
_020FD4DC:
	.word 1
	.word 0
	.word 3
	.word 2
