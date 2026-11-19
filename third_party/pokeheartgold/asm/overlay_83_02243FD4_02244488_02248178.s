	.include "macros.inc"
	.include "overlay_83.inc"
	.include "global.inc"

	.public ov83_0223F1C8
	.public ov83_0223F200
	.public ov83_0223F734
	.public ov83_0223FD14
	.public ov83_02240080
	.public ov83_02240170
	.public ov83_02240290
	.public ov83_022402F4
	.public ov83_02240300
	.public ov83_02240334
	.public ov83_02240348
	.public ov83_0224037C
	.public ov83_02240384
	.public ov83_022403B8
	.public ov83_022403C0
	.public ov83_0224042C
	.public ov83_02240514
	.public ov83_02240664
	.public ov83_02240748
	.public ov83_022407FC
	.public ov83_022408E0
	.public ov83_02240C48
	.public ov83_02240C60
	.public ov83_02240DA8
	.public ov83_02240EC4
	.public ov83_02240F48
	.public ov83_02240FAC
	.public ov83_02241208
	.public ov83_02241254
	.public ov83_022412A0
	.public ov83_022412DC
	.public ov83_02241354
	.public ov83_02241368
	.public ov83_0224153C
	.public ov83_022415F4
	.public ov83_022416A0
	.public ov83_0224175C
	.public ov83_02241770
	.public ov83_022418E8
	.public ov83_02241B18
	.public ov83_02241B30
	.public ov83_02241BC4
	.public ov83_022428A8
	.public ov83_02242AB4
	.public ov83_02242DAC
	.public ov83_02242DFC
	.public ov83_02242E88
	.public ov83_02242F18
	.public ov83_02242F2C
	.public ov83_022471FC
	.public ov83_0224753C
	.public ov83_02247630
	.public ov83_02247768
	.public ov83_0224776C
	.public ov83_0224777C
	.public ov83_022477B0
	.public ov83_022477E4
	.public ov83_022477EC
	.public ov83_02247858
	.public ov83_022478B4
	.public ov83_0224791C
	.public ov83_02247944
	.public ov83_02247A18
	.public ov83_02247A20
	.public ov83_02247AD4
	.public ov83_02247B04
	.public ov83_02247B1C
	.public ov83_02247B34
	.public ov83_02247B4C
	.public ov83_02247B64
	.public ov83_02247BC4
	.public ov83_02247C58
	.public ov83_02247C88
	.public ov83_02247CC4
	.public ov83_02247CF0
	.public ov83_0223E008
	.public ov83_0223E14C
	.public ov83_0223EEA0
	.public ov83_0223EFA4
	.public ov83_0223F010




	.data
	.balign 4, 0
	.global ov83_02248178
ov83_02248178: ; 0x02248178
	.byte 0x0E, 0x0E, 0x0E, 0x0E

	.global ov83_0224817C
ov83_0224817C: ; 0x0224817C
	.byte 0x20, 0x00, 0x00, 0x00
	.byte 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x6B, 0x00, 0x00, 0x00

	.global ov83_0224818C
ov83_0224818C: ; 0x0224818C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00

	.global ov83_0224819C
ov83_0224819C:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_0224819C
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_0224819C, ov83_0224819C ; 0x0224819C
	.word ov83_022482C4
	.byte 0, 0x00, 0x00, 0x00
	.word ov83_022481AC
	.byte 0, 0x00, 0x00, 0x00

ov83_022481AC:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022481AC
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022481AC, ov83_022481AC
	.byte 0, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0x00, 0, 0
	.byte 0, 0, 0, 0x00, 0, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0

ov83_022482C4:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022482C4
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022482C4, ov83_022482C4
	.byte 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0, 0x00, 0, 0, 0, 0, 0, 0x00, 0, 0x00, 0, 0
	.byte 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0

	.global ov83_022484F4
ov83_022484F4:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022484F4
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022484F4, ov83_022484F4 ; 0x022484F4
	.byte 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov83_02248500
ov83_02248500: ; 0x02248500
	.word ov83_02247A20
	.word ov83_02247A20
	.word ov83_02247B1C
	.word ov83_02247B34

	.global ov83_02248510
ov83_02248510: ; 0x02248510
	.word ov83_02247A20
	.word ov83_02247A20
	.word ov83_02247B4C
	.word ov83_02247B64

	.global ov83_02248520
ov83_02248520: ; 0x02248520
	.word ov83_02247A20
	.word ov83_02247A20
	.word ov83_02247C58
	.word ov83_02247C88

	.global ov83_02248530
ov83_02248530:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248530
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248530, ov83_02248530 ; 0x02248530
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00

	.global ov83_02248544
ov83_02248544:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248544
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248544, ov83_02248544 ; 0x02248544
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov83_02248558
ov83_02248558:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248558
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248558, ov83_02248558 ; 0x02248558
	.byte 0, 0, 0, 0, 0, 0, 0x00, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov83_02248570
ov83_02248570:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248570
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248570, ov83_02248570 ; 0x02248570
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov83_0224858C
ov83_0224858C:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_0224858C
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_0224858C, ov83_0224858C ; 0x0224858C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov83_022485A8
ov83_022485A8:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022485A8
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022485A8, ov83_022485A8 ; 0x022485A8
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0

	.global ov83_022485C8
ov83_022485C8:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022485C8
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022485C8, ov83_022485C8 ; 0x022485C8
	.byte 0, 0, 0x00, 0, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov83_022485E8
ov83_022485E8:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022485E8
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_022485E8, ov83_022485E8 ; 0x022485E8
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0

	.global ov83_02248610
ov83_02248610:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248610
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248610, ov83_02248610 ; 0x02248610
	.byte 0

	.global ov83_02248611
ov83_02248611:
	.global hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248611
	.set hgasset_asm_overlay_83_02243FD4_02244488_02248178_s_ov83_02248611, ov83_02248611 ; 0x02248611
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0
	; 0x02248648
