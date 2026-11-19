#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "macros.inc"
	.include "unk_0208DE40.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.data

	.data

	.global _02104EA4
_02104EA4:
	.global hgasset_asm_unk_0208DE40_s__02104EA4
	.set hgasset_asm_unk_0208DE40_s__02104EA4, _02104EA4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00
	.global _02104EB0
_02104EB0:
	.global hgasset_asm_unk_0208DE40_s__02104EB0
	.set hgasset_asm_unk_0208DE40_s__02104EB0, _02104EB0
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0x00
	.byte 0x00, 0x00
	.global _02104EC2
_02104EC2:
	.global hgasset_asm_unk_0208DE40_s__02104EC2
	.set hgasset_asm_unk_0208DE40_s__02104EC2, _02104EC2
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00
	.global _02104ED4
_02104ED4:
	.global hgasset_asm_unk_0208DE40_s__02104ED4
	.set hgasset_asm_unk_0208DE40_s__02104ED4, _02104ED4
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00
	.byte 0, 0x00, 0, 0, 0x00, 0x00
	.global _02104EE6
_02104EE6:
	.global hgasset_asm_unk_0208DE40_s__02104EE6
	.set hgasset_asm_unk_0208DE40_s__02104EE6, _02104EE6
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _02104EF8
_02104EF8:
	.global hgasset_asm_unk_0208DE40_s__02104EF8
	.set hgasset_asm_unk_0208DE40_s__02104EF8, _02104EF8
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0x00, 0x00, 0x00
	.global _02104F0A
_02104F0A:
	.global hgasset_asm_unk_0208DE40_s__02104F0A
	.set hgasset_asm_unk_0208DE40_s__02104F0A, _02104F0A
	.byte 0, 0, 0, 0, 0x00, 0x00
	.byte 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.global _02104F1C
_02104F1C:
	.global hgasset_asm_unk_0208DE40_s__02104F1C
	.set hgasset_asm_unk_0208DE40_s__02104F1C, _02104F1C
	.byte 0, 0, 0, 0
	.byte 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _02104F2E
_02104F2E:
	.global hgasset_asm_unk_0208DE40_s__02104F2E
	.set hgasset_asm_unk_0208DE40_s__02104F2E, _02104F2E
	.byte 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00
	.global _02104F40
_02104F40:
	.global hgasset_asm_unk_0208DE40_s__02104F40
	.set hgasset_asm_unk_0208DE40_s__02104F40, _02104F40
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0x00
	.byte 0x00, 0x00
	.global _02104F52
_02104F52:
	.global hgasset_asm_unk_0208DE40_s__02104F52
	.set hgasset_asm_unk_0208DE40_s__02104F52, _02104F52
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.global _02104F64
_02104F64:
	.global hgasset_asm_unk_0208DE40_s__02104F64
	.set hgasset_asm_unk_0208DE40_s__02104F64, _02104F64
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00
	.byte 0, 0x00, 0, 0, 0x00, 0x00
	.global _02104F76
_02104F76:
	.global hgasset_asm_unk_0208DE40_s__02104F76
	.set hgasset_asm_unk_0208DE40_s__02104F76, _02104F76
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00
	.global _02104F88
_02104F88:
	.global hgasset_asm_unk_0208DE40_s__02104F88
	.set hgasset_asm_unk_0208DE40_s__02104F88, _02104F88
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0
	.byte 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _02104F9A
_02104F9A:
	.global hgasset_asm_unk_0208DE40_s__02104F9A
	.set hgasset_asm_unk_0208DE40_s__02104F9A, _02104F9A
	.byte 0, 0, 0, 0, 0x00, 0x00
	.byte 0, 0, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00
	.global _02104FAC
_02104FAC:
	.global hgasset_asm_unk_0208DE40_s__02104FAC
	.set hgasset_asm_unk_0208DE40_s__02104FAC, _02104FAC
	.byte 0, 0, 0, 0
	.byte 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0x00
	.global _02104FBE
_02104FBE:
	.global hgasset_asm_unk_0208DE40_s__02104FBE
	.set hgasset_asm_unk_0208DE40_s__02104FBE, _02104FBE
	.byte 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00
	.global _02104FD0
_02104FD0:
	.global hgasset_asm_unk_0208DE40_s__02104FD0
	.set hgasset_asm_unk_0208DE40_s__02104FD0, _02104FD0
	.byte 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0
	.global _02104FFC
_02104FFC:
	.global hgasset_asm_unk_0208DE40_s__02104FFC
	.set hgasset_asm_unk_0208DE40_s__02104FFC, _02104FFC
	.byte 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00
