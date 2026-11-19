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
	.public PokemonSummary_Main


	.data
	.balign 4, 0
	.global _021039B8
_021039B8:
	.global hgasset_asm_unk_02088288_02088610_021039B8_s__021039B8
	.set hgasset_asm_unk_02088288_02088610_021039B8_s__021039B8, _021039B8
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.word 0, 0, 0
	.global _021039E8
_021039E8:
	.global hgasset_asm_unk_02088288_02088610_021039B8_s__021039E8
	.set hgasset_asm_unk_02088288_02088610_021039B8_s__021039E8, _021039E8
	.byte 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	; File boundary

	.global gOverlayTemplate_PokemonSummary
gOverlayTemplate_PokemonSummary:
	.word PokemonSummary_Init, PokemonSummary_Main, PokemonSummary_Exit, 0xFFFFFFFF
