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
	.include "unk_0205CB48.inc"
	.include "global.inc"

	.public sub_0205CBEC
	.public sub_0205CC4C
	.public sub_0205CC74
	.public sub_0205CC94
	.public sub_0205D004
	.public sub_0205D09C
	.public sub_0205D0A8
	.public sub_0205D190
	.public sub_0205D340
	.public sub_0205D40C
	.public sub_0205DDD4
	.text

	.rodata

	.global _020FCB7C
_020FCB7C:
	.word sub_0205D09C
	.word sub_0205D0A8
	.word sub_0205D190
	.global _020FCB88
_020FCB88:
	.word MetatileBehavior_IsIce, 1
	.word 0, 3
	.global _020FCB98
_020FCB98:
	.short SEQ_SE_GS_ASHIOTO_A_WALK, 0
	.short SEQ_SE_GS_ASHIOTO_A, 0
	.short SEQ_SE_GS_OCHIBA, 0
	.short SEQ_SE_GS_EDAPAKI, 0
	.short SEQ_SE_GS_KUSA2, 0
	.short SEQ_SE_GS_ASHIOTO_B, 1
	.short SEQ_SE_GS_ASHIOTO_D, 1
	.short SEQ_SE_GS_ASHIOTO_D_2, 0
	.short SEQ_SE_GS_ASHIOTO_E, 0
	.short SEQ_SE_GS_ASHIOTO_B, 1
	.short SEQ_SE_GS_ASHIOTO_F, 1
	.short SEQ_SE_GS_ASHIOTO_D, 0
	.short SEQ_SE_GS_ASHIOTO_H, 0
	.short SEQ_SE_GS_ASHIOTO_G, 0
	.short SEQ_SE_DP_BOX02, 0
	.short SEQ_SE_DP_BOX01, 0
