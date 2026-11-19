	.include "macros.inc"
	.include "unk_0203A3B0.inc"
	.include "global.inc"

	.public sub_0203A42C
	.public sub_0203A4D4
	.public sub_0203A59C
	.data

	.bss

	.global _021D4154
_021D4154:
	.space 0x4

	.data

_020F6A9C:
	.global hgasset_asm_unk_0203A3B0_s__020F6A9C
	.set hgasset_asm_unk_0203A3B0_s__020F6A9C, _020F6A9C
	.byte 0, 0, 0, 0x00
	.byte 0, 0, 0x00, 0x00
_020F6AA4:
	.global hgasset_asm_unk_0203A3B0_s__020F6AA4
	.set hgasset_asm_unk_0203A3B0_s__020F6AA4, _020F6AA4
	.byte 0, 0, 0, 0x00
	.byte 0, 0, 0x00, 0x00
_020F6AAC:
	.global hgasset_asm_unk_0203A3B0_s__020F6AAC
	.set hgasset_asm_unk_0203A3B0_s__020F6AAC, _020F6AAC
	.byte 0, 0, 0, 0x00
	.byte 0, 0, 0x00, 0x00
_020F6AB4:
	.global hgasset_asm_unk_0203A3B0_s__020F6AB4
	.set hgasset_asm_unk_0203A3B0_s__020F6AB4, _020F6AB4
	.byte 0, 0, 0, 0x00
	.byte 0, 0, 0x00, 0x00
_020F6ABC:
	.global hgasset_asm_unk_0203A3B0_s__020F6ABC
	.set hgasset_asm_unk_0203A3B0_s__020F6ABC, _020F6ABC
	.byte 0, 0, 0, 0x00
	.byte 0, 0, 0, 0x00
	.byte 0, 0, 0, 0x00
	.byte 0, 0, 0, 0x00
	.byte 0, 0, 0x00, 0x00

	.data

	.balign 4, 0
	.global _0210F91C
_0210F91C:
	.word _020F6AB4
	.word _020F6AAC
	.word _020F6AA4
	.word _020F6A9C
	.word _020F6ABC

	.balign 4, 0
	.global _0210F930
_0210F930:
	.asciz "data/pl_wm.NCLR"

	.balign 4, 0
	.global _0210F940
_0210F940:
	.asciz "data/pl_wifi.NCGR"

	.balign 4, 0
	.global _0210F954
_0210F954:
	.asciz "data/pl_wm.NCGR"

	.balign 4, 0
	.global _0210F964
_0210F964:
	.asciz "data/wm256k.NCGR"
