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
	.public _020FD4F4
	.public _020FD500
	.public _020FD50C
	.public _020FD518
	.public _020FD524
	.public _020FD530
	.public _020FD53C
	.public _020FD554
	.public _020FD560
	.public _020FD570
	.public _020FD580
	.public _020FD590
	.public _020FD5B0
	.public _020FD5C0
	.public _020FD5E0
	.public _020FD5F0
	.public _020FD600
	.public _020FD610
	.public _020FD620
	.public _020FD630
	.public _020FD640
	.public _020FD650
	.public _020FD660
	.public _020FD670
	.public _020FD680
	.public _020FD690
	.public _020FD6A0
	.public _020FD6B0
	.public _020FD6C0
	.public _020FD6D0
	.public _020FD6E0
	.public _020FD6F0
	.public _020FD700
	.public _020FD710
	.public _020FD720
	.public _020FD730
	.public _020FD740
	.public _020FD4EC
	.public _020FD548
	.public _020FD5A0
	.public _020FD5D0

	.data
	.balign 4, 0
	.global _020FD77C
_020FD77C:
	.word 0x00000000
	.word 0, 0, 0, 0
_020FD790:
	.word 0x00000000, 0, 0, 0
	.word 0
	.global _020FD7A4
_020FD7A4:
	.word 0x00000000, 0, 0
	.word 0, 0
	.global _020FD7B8
_020FD7B8:
	.global hgasset_asm_unk_02061284_020FD77C_s__020FD7B8
	.set hgasset_asm_unk_02061284_020FD77C_s__020FD7B8, _020FD7B8
	.word 0, 0
	.word 0, 0, 0
_020FD7CC:
	.word 0x00000000
	.word 0, 0, 0, 0
	.global _020FD7E0
_020FD7E0:
	.global hgasset_asm_unk_02061284_020FD77C_s__020FD7E0
	.set hgasset_asm_unk_02061284_020FD77C_s__020FD7E0, _020FD7E0
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.global _020FD800
_020FD800:
	.global hgasset_asm_unk_02061284_020FD77C_s__020FD800
	.set hgasset_asm_unk_02061284_020FD77C_s__020FD800, _020FD800
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.global _020FD838
_020FD838:
	.word 0, _020FD7CC
	.word 0, _020FD4F4
	.word 0, _020FD518
	.word 0, _020FD500
	.word 0, _020FD524
	.word 0, _020FD6E0
	.word 0, _020FD6A0
	.word 0, _020FD6B0
	.word 0, _020FD6D0
	.word 0, _020FD554
	.word 0, _020FD53C
	.word 0, _020FD740
	.word 0, _020FD530
	.word 0, _020FD50C
	.word 0, _020FD650
	.word 0, _020FD640
	.word 0, _020FD620
	.word 0, _020FD630
	.word 0, _020FD720
	.word 0, _020FD600
	.word 0, _020FD560
	.word 0, _020FD730
	.word 0, _020FD660
	.word 0, _020FD670
	.word 0, _020FD6C0
	.word 0, _020FD6F0
	.word 0, _020FD680
	.word 0, _020FD580
	.word 0, _020FD570
	.word 0, _020FD5B0
	.word 0, _020FD610
	.word 0, _020FD5C0
	.word 0, _020FD690
	.word 0, _020FD700
	.word 0, _020FD590
	.word 0, _020FD5E0
	.word 0, _020FD5F0
	.word 0, _020FD710
	.word 0, _020FD790
	.word 0, 0
