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
	.include "unk_0206793C.inc"
	.include "global.inc"

	.public sub_0206793C
	.public Task_NPCTrade
	.text

	.rodata

	.balign 4, 0
	.global _020FE79C
_020FE79C:
	.word TradeSequence_Init, TradeSequence_Main, TradeSequence_Exit, SDK_OVERLAY_OVY_71_ID
