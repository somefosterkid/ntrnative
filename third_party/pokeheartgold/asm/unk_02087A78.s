#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "macros.inc"
	.include "unk_02087A78.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public _021028B4
	.public _021028C4
	.public gNatureStatMods

	.public sub_02087BE8
	.public sub_02087C38
	.public sub_02087BAC
	.public sub_02087B10
	.public sub_02087B1C
	.public sub_02087B28
	.public sub_02087B34
	.public sub_02087B40
	.public sub_02087B4C
	.public sub_02087B58
	.public sub_02087B64
	.text

	.rodata

	.public _02102830
_02102830:
	.word sub_02087B10, sub_02087B64, sub_02087BAC, 0xFFFFFFFF
_02102840:
	.word sub_02087B58, sub_02087B64, sub_02087BAC, 0xFFFFFFFF
_02102850:
	.word sub_02087B4C, sub_02087B64, sub_02087BAC, 0xFFFFFFFF
_02102860:
	.word sub_02087B40, sub_02087B64, sub_02087BAC, 0xFFFFFFFF
_02102870:
	.word sub_02087B34, sub_02087B64, sub_02087BAC, 0xFFFFFFFF
_02102880:
	.word sub_02087B28, sub_02087B64, sub_02087BAC, 0xFFFFFFFF
_02102890:
	.word sub_02087B1C, sub_02087B64, sub_02087BAC, 0xFFFFFFFF
	.data

	.global _02110594
_02110594:
	.word _02102830
	.word _02102890
	.word _02102880
	.word _02102870
	.word _02102860
	.word _02102850
	.word _02102840
