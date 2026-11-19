#include "config.h"
	.include "macros.inc"
	.include "overlay_112.inc"
	.include "global.inc"
	.public gApplication_TitleScreen

	.public ov112_021E76CC
	.public ov112_021E7830
	.public ov112_021E7910
	.public ov112_021EAC18
	.public ov112_021EAC34
	.public ov112_021EAC58
	.public ov112_021EAC5C
	.public ov112_021EAF48
	.public ov112_021EAF5C
	.public ov112_021EAFFC
	.public ov112_021EB1D8
	.public ov112_021EB1E8
	.public ov112_021EB208
	.public ov112_021EB244
	.public ov112_021EB28C
	.public ov112_021EB388
	.public ov112_021EB3A8
	.public ov112_021EB3B8
	.public ov112_021EB3D8
	.public ov112_021EB44C
	.public ov112_021EB568
	.public ov112_021EB5AC
	.public ov112_021EB5BC
	.public ov112_021EB5DC
	.public ov112_021EB664
	.public ov112_021EB780
	.public ov112_021EB7D8
	.public ov112_021EB838
	.public ov112_021EB85C
	.public ov112_021EB864
	.public ov112_021EB8C4
	.public ov112_021EB8E8
	.public ov112_021EB908
	.public ov112_021EBA08
	.public ov112_021EBA28
	.public ov112_021EBA50
	.public ov112_021EBA54
	.public ov112_021EBA6C
	.public ov112_021EBA8C
	.public ov112_021EBAB0
	.public ov112_021EBAB4
	.public ov112_021EBB58
	.public ov112_021EBB84
	.public ov112_021EBB98
	.public ov112_021EBBB0
	.public ov112_021EBBD8
	.public ov112_021EBC3C
	.public ov112_021EBC7C
	.public ov112_021EBCD8
	.public ov112_021EBD9C
	.public ov112_021EBDE8
	.public ov112_021EBE4C
	.public ov112_021EBE6C
	.public ov112_021EBE78
	.public ov112_021EBF18
	.public ov112_021EBF4C
	.public ov112_021EBF70
	.public ov112_021EBFC8
	.public ov112_021EC000
	.public ov112_021EC038
	.public ov112_021EC044
	.public ov112_021EC1A8
	.public ov112_021EC1E4
	.public ov112_021EC208
	.public ov112_021EC284
	.public ov112_021EC2A8
	.public ov112_021EC2E4
	.public ov112_021EC304
	.public ov112_021EC338
	.public ov112_021EC374
	.public ov112_021EC378
	.public ov112_021EC390
	.public ov112_021EC3B0
	.public ov112_021EC3C8
	.public ov112_021EC3F0
	.public ov112_021EC404
	.public ov112_021EC414
	.public ov112_021EC434
	.public ov112_021EC478
	.public ov112_021EC520
	.public ov112_021EC54C
	.public ov112_021EC55C
	.public ov112_021EC640
	.public ov112_021EC67C
	.public ov112_021EC69C
	.public ov112_021EC6C0
	.public ov112_021EC6D8
	.public ov112_021EC6F8
	.public ov112_021EC70C
	.public ov112_021EC778
	.public ov112_021EC99C
	.public ov112_021ECAA8
	.public ov112_021ECCC4
	.public ov112_021ECE28
	.public ov112_021ECECC
	.public ov112_021ECEEC
	.public ov112_021ECEF8
	.public ov112_021ECF38
	.public ov112_021ECF88
	.public ov112_021ECFD4
	.public ov112_021ED020
	.public ov112_021ED054
	.public ov112_021ED090
	.public ov112_021ED1A0
	.public ov112_021ED224
	.public ov112_021ED264
	.public ov112_021ED35C
	.public ov112_021ED4D4
	.public ov112_021ED66C
	.public ov112_021ED8DC
	.public ov112_021ED918
	.public ov112_021EDA4C
	.public ov112_021EDB24
	.public ov112_021EDD08
	.public ov112_021EDD30
	.public ov112_021EDD5C
	.public ov112_021EDD80
	.public ov112_021EDDB4
	.public ov112_021EDDE8
	.public ov112_021EDE1C
	.public ov112_021EDE68
	.public ov112_021EDF08
	.public ov112_021EDF4C
	.public ov112_021EE044
	.public ov112_021EE0F4
	.public ov112_021EE10C
	.public ov112_021EE12C
	.public ov112_021EE178
	.public ov112_021EE1A4
	.public ov112_021EE1BC
	.public ov112_021EE1DC
	.public ov112_021EE1E8
	.public ov112_021EE29C
	.public ov112_021EE2B4
	.public ov112_021EE2D4
	.public ov112_021EE31C
	.public ov112_021EE3E0
	.public ov112_021EE464
	.public ov112_021EE4B8
	.public ov112_021EE4D0
	.public ov112_021EE510
	.public ov112_021EE514
	.public ov112_021EE524
	.public ov112_021EE52C
	.public ov112_021EE550
	.public ov112_021EE570
	.public ov112_021EE594
	.public ov112_021EE5B8
	.public ov112_021EE604
	.public ov112_021EE608
	.public ov112_021EE624
	.public ov112_021EE628
	.public ov112_021EE688
	.public ov112_021EE6AC
	.public ov112_021EE6B0
	.public ov112_021EE6D0
	.public ov112_021EE6F8
	.public ov112_021EE70C
	.public ov112_021EE718
	.public ov112_021EE73C
	.public ov112_021EE754
	.public ov112_021EE778
	.public ov112_021EED08
	.public ov112_021EEDFC
	.public ov112_021EEE20
	.public ov112_021EEE2C
	.public ov112_021EEE4C
	.public ov112_021EEE8C
	.public ov112_021EEEA0
	.public ov112_021EEEAC
	.public ov112_021EEEC0
	.public ov112_021EEEE0
	.public ov112_021EEF08
	.public ov112_021EEF1C
	.public ov112_021EEF28
	.public ov112_021EEF30
	.public ov112_021EEF68
	.public ov112_021EF03C
	.public ov112_021EF100
	.public ov112_021EF568
	.public ov112_021EF5AC
	.public ov112_021EF62C
	.public ov112_021EF640
	.public ov112_021EF644
	.public ov112_021EF660
	.public ov112_021EF664
	.public ov112_021EF67C
	.public ov112_021EF6A0
	.public ov112_021EF700
	.public ov112_021EF758
	.public ov112_021EF8C0
	.public ov112_021EF8D4
	.public ov112_021EF958
	.public ov112_021EF994
	.public ov112_021EF9D4
	.public ov112_021EFA1C
	.public ov112_021EFA44
	.public ov112_021EFA74
	.public ov112_021EFA9C
	.public ov112_021EFAC0
	.public ov112_021EFB14
	.public ov112_021EFC9C
	.public ov112_021EFCB0
	.public ov112_021EFCD8
	.public ov112_021EFCFC
	.public ov112_021EFDA8
	.public ov112_021EFED8
	.public ov112_021EFEEC
	.public ov112_021EFF14
	.public ov112_021EFF38
	.public ov112_021EFF4C
	.public ov112_021EFF70
	.public ov112_021EFF84
	.public ov112_021F00CC
	.public ov112_021F00E0
	.public ov112_021F0114
	.public ov112_021F015C
	.public ov112_021F0180
	.public ov112_021F0194
	.public ov112_021F01B8
	.public ov112_021F01CC
	.public ov112_021F0314
	.public ov112_021F0328
	.public ov112_021F0368
	.public ov112_021F0394
	.public ov112_021F2EF4
	.public ov112_021F2FAC
	.public ov112_021F3018




	.data
	.balign 4, 0
	.global ov112_021FF398
ov112_021FF398:
	.global hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF398
	.set hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF398, ov112_021FF398 ; 0x021FF398
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov112_021FF3CC
ov112_021FF3CC:
	.global hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF3CC
	.set hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF3CC, ov112_021FF3CC ; 0x021FF3CC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov112_021FF400
ov112_021FF400:
	.global hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF400
	.set hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF400, ov112_021FF400 ; 0x021FF400
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov112_021FF434
ov112_021FF434:
	.global hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF434
	.set hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF434, ov112_021FF434 ; 0x021FF434
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov112_021FF4C0
ov112_021FF4C0:
	.global hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF4C0
	.set hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF4C0, ov112_021FF4C0 ; 0x021FF4C0
	.byte 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov112_021FF4C8
ov112_021FF4C8:
	.global hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF4C8
	.set hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF4C8, ov112_021FF4C8 ; 0x021FF4C8
	.byte 0x00, 0, 0, 0, 0, 0, 0, 0
	.byte 0x00, 0, 0x00, 0x00

	.global ov112_021FF4D4
ov112_021FF4D4:
	.global hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF4D4
	.set hgasset_asm_overlay_112_021F0DF4_021FF398_s_ov112_021FF4D4, ov112_021FF4D4 ; 0x021FF4D4
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0
