	.include "macros.inc"
	.include "unk_02014DA0.inc"
	.include "global.inc"

	.public NNS_G3dGlb

	.public sub_020154B8
	.public sub_020154E4
	.public sub_02015104
	.public sub_02015130
	.public sub_0201515C
	.public sub_02015188
	.public sub_020151B4
	.public sub_020151E0
	.public sub_0201520C
	.public sub_02015238
	.public sub_02014FA4
	.public sub_02014FD0
	.public sub_02014FFC
	.public sub_02015028
	.public sub_02015054
	.public sub_02015080
	.public sub_020150AC
	.public sub_020150D8
	.text

	.rodata

	.global _020F6078
_020F6078:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.global _020F6084
_020F6084:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _020F6090
_020F6090:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
	.global _020F609C
_020F609C:
	.word sub_02014FA4
	.word sub_02014FD0
	.word sub_02014FFC
	.word sub_02015028
	.word sub_02015054
	.word sub_02015080
	.word sub_020150AC
	.word sub_020150D8
	.word sub_02015104
	.word sub_02015130
	.word sub_0201515C
	.word sub_02015188
	.word sub_020151B4
	.word sub_020151E0
	.word sub_0201520C
	.word sub_02015238
	.global _020F60DC
_020F60DC:
	.short 0x011F, 0xFFFF ; ‣

	.bss

	.global _021D10A0
_021D10A0:
	.space 8

	.global _021D10A8
_021D10A8:
	.space 0x40
