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
	.include "unk_02061284.inc"
	.include "global.inc"

	.public sub_02061720
	.public sub_02061754
	.public sub_02061770
	.public sub_02061874
	.public sub_02061894
	.public sub_020618B0
	.public sub_020618C8
	.public sub_020619C0
	.public sub_020619FC
	.public sub_02061ABC
	.public sub_02061C40
	.public sub_02061D50
	.public _020FD77C
	.public _020FD7A4
	.public _020FD7B8
	.public _020FD7E0
	.public _020FD800
	.public _020FD838
	.public sub_020617AC
	.text

	.rodata

	.public _020FD4EC
	.global _020FD4EC
_020FD4EC:
	.word sub_02061C40
	.word sub_02061D50
	.global _020FD4F4
_020FD4F4:
	.word 0, 0, 0
	.global _020FD500
_020FD500:
	.word 0, 0, 0
	.global _020FD50C
_020FD50C:
	.word 0, 0, 0
	.global _020FD518
_020FD518:
	.word 0, 0, 0
	.global _020FD524
_020FD524:
	.word 0, 0, 0
	.global _020FD530
_020FD530:
	.word 0, 0, 0
	.global _020FD53C
_020FD53C:
	.word 0, 0, 0
	.public _020FD548
	.global _020FD548
_020FD548:
	.word sub_020619C0
	.word sub_020619FC
	.word sub_02061ABC
	.global _020FD554
_020FD554:
	.word 0, 0, 0
	.global _020FD560
_020FD560:
	.word 0, 0, 0, 0
	.global _020FD570
_020FD570:
	.word 0, 0, 0, 0
	.global _020FD580
_020FD580:
	.word 0, 0, 0, 0
	.global _020FD590
_020FD590:
	.word 0, 0, 0, 0
	.public _020FD5A0
	.global _020FD5A0
_020FD5A0:
	.word sub_02061874
	.word sub_02061894
	.word sub_020618B0
	.word sub_020618C8
	.global _020FD5B0
_020FD5B0:
	.word 1, 2, 3, 0
	.global _020FD5C0
_020FD5C0:
	.word 1, 3, 0, 2
	.public _020FD5D0
	.global _020FD5D0
_020FD5D0:
	.word sub_02061720
	.word sub_02061754
	.word sub_02061770
	.word sub_020617AC
	.global _020FD5E0
_020FD5E0:
	.word 0, 0, 0x00000000, 0
	.global _020FD5F0
_020FD5F0:
	.word 0, 0x00000000, 0, 0
	.global _020FD600
_020FD600:
	.word 0, 0, 0, 0x00000000
	.global _020FD610
_020FD610:
	.word 0x00000000, 0, 0, 0
	.global _020FD620
_020FD620:
	.word 0, 0x00000000, 0, 0
	.global _020FD630
_020FD630:
	.word 0, 0, 0x00000000, 0
	.global _020FD640
_020FD640:
	.word 0, 0, 0, 0x00000000
	.global _020FD650
_020FD650:
	.word 0x00000000, 0, 0, 0
	.global _020FD660
_020FD660:
	.word 0, 0x00000000, 0, 0
	.global _020FD670
_020FD670:
	.word 0x00000000, 0, 0, 0
	.global _020FD680
_020FD680:
	.word 0, 0x00000000, 0, 0
	.global _020FD690
_020FD690:
	.word 0, 0, 0, 0x00000000
	.global _020FD6A0
_020FD6A0:
	.word 0x00000000, 0, 0, 0
	.global _020FD6B0
_020FD6B0:
	.word 0x00000000, 0, 0, 0
	.global _020FD6C0
_020FD6C0:
	.word 0, 0, 0x00000000, 0
	.global _020FD6D0
_020FD6D0:
	.word 0, 0, 0, 0
	.global _020FD6E0
_020FD6E0:
	.word 0x00000000, 0, 0, 0
	.global _020FD6F0
_020FD6F0:
	.word 0, 0, 0, 0x00000000
	.global _020FD700
_020FD700:
	.word 0, 0x00000000, 0, 0
	.global _020FD710
_020FD710:
	.word 0, 0, 0, 0x00000000
	.global _020FD720
_020FD720:
	.word 0, 0, 0, 0x00000000
	.global _020FD730
_020FD730:
	.word 0, 0, 0x00000000, 0
	.global _020FD740
_020FD740:
	.word 0x00000000, 0, 0, 0
	.word 0
	.public _020FD754
	.public _020FD768
	.public _020FD77C
	.public _020FD7A4
	.public _020FD7B8
	.public _020FD7E0
	.public _020FD800
	.public _020FD838
