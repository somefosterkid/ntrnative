#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "macros.inc"
	.include "unk_02088288.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.public sub_0208887C
	.public sub_02088894
	.public sub_0208895C
	.public sub_020889D0
	.public sub_02088AF8
	.public sub_02088B08
	.public sub_02088B40
	.public sub_02088D18
	.public sub_02088D34
	.public sub_02088D48
	.public sub_02088E68
	.public sub_02088E98
	.public sub_02089028
	.public sub_02089208
	.public sub_020892F4
	.public sub_02089308
	.public sub_0208931C
	.public sub_0208942C
	.public sub_02089454
	.public sub_02089478
	.public sub_02089608
	.public sub_02089658
	.public sub_02089670
	.public sub_02089680
	.public sub_02089698
	.public sub_02089794
	.public sub_020897C0
	.public sub_02089CB4
	.public PokemonSummary_Exit
	.public PokemonSummary_Init
	.public gOverlayTemplate_PokemonSummary
	.data

	.data

	.global _021038AC
_021038AC:
	.global hgasset_asm_unk_02088288_s__021038AC
	.set hgasset_asm_unk_02088288_s__021038AC, _021038AC
	.byte 0, 0, 0, 0
	.global _021038B0
_021038B0:
	.global hgasset_asm_unk_02088288_s__021038B0
	.set hgasset_asm_unk_02088288_s__021038B0, _021038B0
	.byte 0, 0, 0, 0
	.global _021038B4
_021038B4:
	.global hgasset_asm_unk_02088288_s__021038B4
	.set hgasset_asm_unk_02088288_s__021038B4, _021038B4
	.byte 0, 0, 0, 0
	.global _021038B8
_021038B8:
	.global hgasset_asm_unk_02088288_s__021038B8
	.set hgasset_asm_unk_02088288_s__021038B8, _021038B8
	.byte 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00
	.global _021038C4
_021038C4:
	.global hgasset_asm_unk_02088288_s__021038C4
	.set hgasset_asm_unk_02088288_s__021038C4, _021038C4
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00
	.global _021038D4
_021038D4:
	.global hgasset_asm_unk_02088288_s__021038D4
	.set hgasset_asm_unk_02088288_s__021038D4, _021038D4
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00
	.global _021038E8
_021038E8:
	.global hgasset_asm_unk_02088288_s__021038E8
	.set hgasset_asm_unk_02088288_s__021038E8, _021038E8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.global _02103904
_02103904:
	.global hgasset_asm_unk_02088288_s__02103904
	.set hgasset_asm_unk_02088288_s__02103904, _02103904
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _02103920
_02103920:
	.global hgasset_asm_unk_02088288_s__02103920
	.set hgasset_asm_unk_02088288_s__02103920, _02103920
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _0210393C
_0210393C:
	.global hgasset_asm_unk_02088288_s__0210393C
	.set hgasset_asm_unk_02088288_s__0210393C, _0210393C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _02103958
_02103958:
	.global hgasset_asm_unk_02088288_s__02103958
	.set hgasset_asm_unk_02088288_s__02103958, _02103958
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.global _02103974
_02103974:
	.global hgasset_asm_unk_02088288_s__02103974
	.set hgasset_asm_unk_02088288_s__02103974, _02103974
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.public gOverlayTemplate_PokemonSummary
