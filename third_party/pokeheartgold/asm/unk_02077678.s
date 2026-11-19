#include "constants/items.h"
#include "constants/moves.h"
	.include "macros.inc"
	.include "unk_02077678.inc"
	.include "global.inc"

	.data

	.data


	.type sMoveCategoryGfxPlttOverrides,@object
	.global sMoveCategoryGfxPlttOverrides
sMoveCategoryGfxPlttOverrides:
	.byte 0, 1, 0
	.size sMoveCategoryGfxPlttOverrides,.-sMoveCategoryGfxPlttOverrides
	.balign 4, 0

	.type sMoveCategoryGfxCharFileIds,@object
	.global sMoveCategoryGfxCharFileIds
sMoveCategoryGfxCharFileIds:
	.global hgasset_asm_unk_02077678_s_sMoveCategoryGfxCharFileIds
	.set hgasset_asm_unk_02077678_s_sMoveCategoryGfxCharFileIds, sMoveCategoryGfxCharFileIds
	.word 0, 0, 0
	.size sMoveCategoryGfxCharFileIds,.-sMoveCategoryGfxCharFileIds

	.type sTypeIconGfxCharFileIds,@object
	.global sTypeIconGfxCharFileIds
sTypeIconGfxCharFileIds:
	.global hgasset_asm_unk_02077678_s_sTypeIconGfxCharFileIds
	.set hgasset_asm_unk_02077678_s_sTypeIconGfxCharFileIds, sTypeIconGfxCharFileIds
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.size sTypeIconGfxCharFileIds,.-sTypeIconGfxCharFileIds

	.type sTypeIconGfxPlttOverrides,@object
	.global sTypeIconGfxPlttOverrides
sTypeIconGfxPlttOverrides:
	.global hgasset_asm_unk_02077678_s_sTypeIconGfxPlttOverrides
	.set hgasset_asm_unk_02077678_s_sTypeIconGfxPlttOverrides, sTypeIconGfxPlttOverrides
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.size sTypeIconGfxPlttOverrides,.-sTypeIconGfxPlttOverrides
