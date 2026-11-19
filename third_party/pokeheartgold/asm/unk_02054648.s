	.include "macros.inc"
	.include "unk_02054648.inc"
	.include "global.inc"

	.public sub_02054648
	.public sub_02054774
	.public sub_020547A4
	.public sub_020547D8
	.public sub_02054824
	.text

	.rodata

	.global _020FC5FC
_020FC5FC:
	.short 0x00D0
	.short 0x00D2
	.short 0x00D3
	.short 0x0000
	.global _020FC604
_020FC604:
	.word sub_02054774
	.word sub_020547D8
	.global _020FC60C
_020FC60C:
	.short 0x00D0
	.short 0x00D1
	.short 0x00D2
	.short 0x00D3
	.global _020FC614
_020FC614:
	.word sub_020547A4
	.word sub_02054824
