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
	.public ov112_021F37AC
	.public ov112_021F3814
	.public ov112_021F387E
	.public ov112_021F38E8
	.public ov112_021F6958
	.public ov112_021F6A18
	.public ov112_021F6AD8
	.public ov112_021F6B98
	.public ov112_021F6C58
	.public ov112_021F6D18
	.public ov112_021F6E98
	.public ov112_021F6F58
	.public ov112_021FEA98
	.public ov112_021FEAA0
	.public ov112_021FEAA8
	.public ov112_021FEAB0
	.public ov112_021FEAB8
	.public ov112_021FEAC0
	.public ov112_021FEAC8
	.public ov112_021FEAD0
	.public ov112_021FEAD8
	.public ov112_021FEAE0
	.public ov112_021FEAE8
	.public ov112_021FEAF0
	.public ov112_021FEAF8
	.public ov112_021FEB00
	.public ov112_021FEB08
	.public ov112_021FEB10
	.public ov112_021FEB18
	.public ov112_021FEB20
	.public ov112_021FEB28
	.public ov112_021FEB30
	.public ov112_021FEB38
	.public ov112_021FEB40
	.public ov112_021FEB48
	.public ov112_021FEB50
	.public ov112_021FEB58
	.public ov112_021FEB60
	.public ov112_021FEB68
	.public ov112_021FEB70
	.public ov112_021FEB78
	.public ov112_021FEB80
	.public ov112_021FEB88
	.public ov112_021FEB90
	.public ov112_021FEB98
	.public ov112_021FEBA0
	.public ov112_021FEBA8
	.public ov112_021FEBB0
	.public ov112_021FEBB8
	.public ov112_021FEBC0
	.public ov112_021FEBC8
	.public ov112_021FEBD0
	.public ov112_021FEBD8
	.public ov112_021FEBE0
	.public ov112_021FEBE8
	.public ov112_021FEBF0
	.public ov112_021FEBF8
	.public ov112_021FEC00
	.public ov112_021FEC08
	.public ov112_021FEC10
	.public ov112_021FEC18
	.public ov112_021FEC20
	.public ov112_021FEC28
	.public ov112_021FEC30
	.public ov112_021FEC38
	.public ov112_021FEC40
	.public ov112_021FEC48
	.public ov112_021FEC50
	.public ov112_021FEC58
	.public ov112_021FEC60
	.public ov112_021FEC68
	.public ov112_021FEC70
	.public ov112_021FEC78
	.public ov112_App_MainMenu_SelectOption_ConnectToPokewalker
	.data

	.data

ov112_021F3664:
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov112_021F366C:
	.byte 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov112_021F3678:
	.byte 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00

	.global ov112_021F3684
ov112_021F3684:
	.global hgasset_asm_overlay_112_s_ov112_021F3684
	.set hgasset_asm_overlay_112_s_ov112_021F3684, ov112_021F3684 ; 0x021F3684
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00

ov112_021F3694:
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00

ov112_021F36A4:
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov112_021F36B8:
	.byte 0x00, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.public ov112_App_MainMenu_SelectOption_ConnectToPokewalker
	.data

	.global _021FF500
_021FF500:
	.byte 0xFF, 0xFF, 0x00, 0x00

	.global ov112_021FF504
ov112_021FF504: ; 0x021FF504
	.word ov112_021F3814
	.word ov112_021F387E
	.word ov112_021F37AC
	.word ov112_021F38E8

	.global ov112_021FF514
ov112_021FF514: ; 0x021FF514
	.word ov112_021F366C
	.word ov112_021F3678
	.word ov112_021F3694
	.word ov112_021F36A4
	.word ov112_021F36B8

	.global ov112_021FF528
ov112_021FF528: ; 0x021FF528
	.word ov112_021F3664
	.word ov112_021F6F58
	.word ov112_021F6958
	.word ov112_021F6D18
	.word ov112_021F6E98
	.word ov112_021F6C58
	.word ov112_021F6A18
	.word ov112_021F6B98
	.word ov112_021F6AD8

	.global ov112_021FF54C
ov112_021FF54C: ; 0x021FF54C
	.word ov112_021EAC18

	.global ov112_021FF550
ov112_021FF550: ; 0x021FF550
	.word ov112_021EAC34

	.global ov112_021FF554
ov112_021FF554: ; 0x021FF554
	.word ov112_021EAC58
	.word ov112_021EBA08
	.word ov112_021EBA28
	.word ov112_021EBA50
	.word ov112_021EBAB4
	.word ov112_021EBB58
	.word ov112_021EBB84
	.word ov112_021EC478
	.word ov112_021EC520
	.word ov112_021EC54C
	.word ov112_021EE604
	.word ov112_021EE608
	.word ov112_021EE624
	.word ov112_021EE628
	.word ov112_021EE688
	.word ov112_021EE6AC
	.word ov112_021EED08
	.word ov112_021EEDFC
	.word ov112_021EEE20
	.word ov112_021EEF68
	.word ov112_021EF03C
	.word ov112_021EF100
	.word ov112_021EF568
	.word ov112_021EF5AC
	.word ov112_021EF62C
	.word ov112_021EF640
	.word ov112_021EF644
	.word ov112_021EF660
	.word ov112_021EF664
	.word ov112_021EF67C
	.word ov112_021EF6A0

	.global ov112_021FF5D0
ov112_021FF5D0: ; 0x021FF5D0
	.word ov112_021FEC30
	.word ov112_021FEC28

	.global ov112_021FF5D8
ov112_021FF5D8: ; 0x021FF5D8
	.word ov112_021FEC68
	.word ov112_021FEC60
	.word ov112_021FEC58
	.word ov112_021FEC50
	.word ov112_021FEC48
	.word ov112_021FEC40

	.global ov112_021FF5F0
ov112_021FF5F0: ; 0x021FF5F0
	.word ov112_021FEB08
	.word ov112_021FEAF8
	.word ov112_021FEAE8
	.word ov112_021FEAD8
	.word ov112_021FEAC8
	.word ov112_021FEAB8

	.global ov112_021FF608
ov112_021FF608: ; 0x021FF608
	.word ov112_021FEAB0
	.word ov112_021FEAA8
	.word ov112_021FEAF0
	.word ov112_021FEAC0
	.word ov112_021FEA98
	.word ov112_021FEAA0
	.word ov112_021FEC78

	.global ov112_021FF624
ov112_021FF624: ; 0x021FF624
	.word ov112_021FEB88
	.word ov112_021FEB80
	.word ov112_021FEC70
	.word ov112_021FEB70
	.word ov112_021FEB68
	.word ov112_021FEC38
	.word ov112_021FEB58

	.global ov112_021FF640
ov112_021FF640: ; 0x021FF640
	.word ov112_021FEC20
	.word ov112_021FEC18
	.word ov112_021FEC10
	.word ov112_021FEC08
	.word ov112_021FEC00
	.word ov112_021FEBF8
	.word ov112_021FEBF0
	.word ov112_021FEBE8

	.global ov112_021FF660
ov112_021FF660: ; 0x021FF660
	.word ov112_021FEBD8
	.word ov112_021FEBD0
	.word ov112_021FEBC8
	.word ov112_021FEBC0
	.word ov112_021FEBB8
	.word ov112_021FEBB0
	.word ov112_021FEBA8
	.word ov112_021FEBA0
	.word ov112_021FEB98

	.global ov112_021FF684
ov112_021FF684: ; 0x021FF684
	.word ov112_021FEB48
	.word ov112_021FEB40
	.word ov112_021FEB38
	.word ov112_021FEBE0
	.word ov112_021FEB28
	.word ov112_021FEB20
	.word ov112_021FEB18
	.word ov112_021FEB10
	.word ov112_021FEB90
	.word ov112_021FEB00
	.word ov112_021FEB78
	.word ov112_021FEB60
	.word ov112_021FEB50
	.word ov112_021FEAE0
	.word ov112_021FEB30
	.word ov112_021FEAD0

	.global ov112_021FF6C4
ov112_021FF6C4: ; 0x021FF6C4
	.word ov112_021EAC5C
	.word ov112_021EAF48
	.word ov112_021EAF5C
	.word ov112_021EAFFC
	.word ov112_021EB1D8
	.word ov112_021EB1E8
	.word ov112_021EB208
	.word ov112_021EB244
	.word ov112_021EB28C
	.word ov112_021EB388
	.word ov112_021EB3A8
	.word ov112_021EB3B8
	.word ov112_021EB3D8
	.word ov112_021EB44C
	.word ov112_021EB568
	.word ov112_021EB5AC
	.word ov112_021EB5BC
	.word ov112_021EB5DC
	.word ov112_021EB664
	.word ov112_021EB780
	.word ov112_021EB7D8
	.word ov112_021EB838
	.word ov112_021EB85C
	.word ov112_021EB864
	.word ov112_021EB8C4
	.word ov112_021EB8E8
	.word ov112_021EB908

	.global ov112_021FF730
ov112_021FF730: ; 0x021FF730
	.word ov112_021EBA54
	.word ov112_021EBA6C
	.word ov112_021EBA8C
	.word ov112_021EBAB0

	.global ov112_021FF740
ov112_021FF740: ; 0x021FF740
	.word ov112_021EBB98
	.word ov112_021EBBB0
	.word ov112_021EBBD8
	.word ov112_021EBC3C
	.word ov112_021EBC7C
	.word ov112_021EBCD8
	.word ov112_021EBD9C
	.word ov112_021EBDE8
	.word ov112_021EBE4C
	.word ov112_021EBE6C
	.word ov112_021EBE78
	.word ov112_021EBF18
	.word ov112_021EBF4C
	.word ov112_021EBF70
	.word ov112_021EBFC8
	.word ov112_021EC000
	.word ov112_021EC038
	.word ov112_021EC044
	.word ov112_021EC1A8
	.word ov112_021EC1E4
	.word ov112_021EC208
	.word ov112_021EC284
	.word ov112_021EC2A8
	.word ov112_021EC2E4
	.word ov112_021EC304
	.word ov112_021EC338
	.word ov112_021EC374
	.word ov112_021EC378
	.word ov112_021EC390
	.word ov112_021EC3B0
	.word ov112_021EC3C8
	.word ov112_021EC3F0
	.word ov112_021EC404
	.word ov112_021EC414
	.word ov112_021EC434

	.global ov112_021FF7CC
ov112_021FF7CC: ; 0x021FF7CC
	.word ov112_021EC55C
	.word ov112_021EC640
	.word ov112_021EC67C
	.word ov112_021EC69C
	.word ov112_021EC6C0
	.word ov112_021EC6D8
	.word ov112_021EC6F8
	.word ov112_021EC70C
	.word ov112_021EC778
	.word ov112_021EC99C
	.word ov112_021ECAA8
	.word ov112_021ECCC4
	.word ov112_021ECE28
	.word ov112_021ECECC
	.word ov112_021ECEEC
	.word ov112_021ECEF8
	.word ov112_021ECF38
	.word ov112_021ECF88
	.word ov112_021ECFD4
	.word ov112_021ED020
	.word ov112_021ED054
	.word ov112_021ED090
	.word ov112_021ED1A0
	.word ov112_021ED224
	.word ov112_021ED264
	.word ov112_021ED35C
	.word ov112_021ED4D4
	.word ov112_021ED66C
	.word ov112_021ED8DC
	.word ov112_021ED918
	.word ov112_021EDA4C
	.word ov112_021EDB24
	.word ov112_021EDD08
	.word ov112_021EDD30
	.word ov112_021EDE1C
	.word ov112_021EDE68
	.word ov112_021EDF08
	.word ov112_021EDF4C
	.word ov112_021EE044
	.word ov112_021EE0F4
	.word ov112_021EE10C
	.word ov112_021EE12C
	.word ov112_021EE178
	.word ov112_021EE1A4
	.word ov112_021EE1BC
	.word ov112_021EE1DC
	.word ov112_021EE1E8
	.word ov112_021EE29C
	.word ov112_021EE2B4
	.word ov112_021EE2D4
	.word ov112_021EE31C
	.word ov112_021EE3E0
	.word ov112_021EE464
	.word ov112_021EE4B8
	.word ov112_021EE4D0
	.word ov112_021EE510
	.word ov112_021EE514
	.word ov112_021EE524
	.word ov112_021EDD5C
	.word ov112_021EDD80
	.word ov112_021EDDB4
	.word ov112_021EDDE8
	.word ov112_021EE52C
	.word ov112_021EE550
	.word ov112_021EE570
	.word ov112_021EE594
	.word ov112_021EE5B8

	.global ov112_021FF8D8
ov112_021FF8D8: ; 0x021FF8D8
	.word ov112_021EE6B0
	.word ov112_021EE6D0
	.word ov112_021EE6F8
	.word ov112_021EE70C
	.word ov112_021EE718
	.word ov112_021EE73C
	.word ov112_021EE754
	.word ov112_021EE778

	.global ov112_021FF8F8
ov112_021FF8F8: ; 0x021FF8F8
	.word ov112_021EEE2C
	.word ov112_021EEE4C
	.word ov112_021EEE8C
	.word ov112_021EEEA0
	.word ov112_021EEEAC
	.word ov112_021EEEC0
	.word ov112_021EEEE0
	.word ov112_021EEF08
	.word ov112_021EEF1C
	.word ov112_021EEF28
	.word ov112_021EEF30

	.global ov112_021FF924
ov112_021FF924: ; 0x021FF924
	.word ov112_021EF700
	.word ov112_021EF758
	.word ov112_021EF8C0
	.word ov112_021EF8D4
	.word ov112_021EF958
	.word ov112_021EF994
	.word ov112_021EF9D4
	.word ov112_021EFA1C
	.word ov112_021EFA44
	.word ov112_021EFA74
	.word ov112_021EFA9C
	.word ov112_021EFAC0
	.word ov112_021EFB14
	.word ov112_021EFC9C
	.word ov112_021EFCB0
	.word ov112_021EFCD8
	.word ov112_021EFCFC
	.word ov112_021EFDA8
	.word ov112_021EFED8
	.word ov112_021EFEEC
	.word ov112_021EFF14
	.word ov112_021EFF38
	.word ov112_021EFF4C
	.word ov112_021EFF70
	.word ov112_021EFF84
	.word ov112_021F00CC
	.word ov112_021F00E0
	.word ov112_021F0114
	.word ov112_021F015C
	.word ov112_021F0180
	.word ov112_021F0194
	.word ov112_021F01B8
	.word ov112_021F01CC
	.word ov112_021F0314
	.word ov112_021F0328
	.word ov112_021F0368
	.word ov112_021F0394

	.global ov112_021FF9B8
ov112_021FF9B8: ; 0x021FF9B8
	.byte 0x99, 0x04, 0x00, 0x00, 0x99, 0x04, 0x00, 0x00
	.byte 0x9A, 0x04, 0x00, 0x00, 0x9A, 0x04, 0x00, 0x00, 0x9B, 0x04, 0x00, 0x00, 0x9D, 0x04, 0x00, 0x00
	.byte 0x99, 0x04, 0x00, 0x00, 0x9C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.bss

	.global _021FF9E0
_021FF9E0:
	.space 0x38

	.global ov112_021FFA18
ov112_021FFA18: ; 0x021FFA18
	.space 0x8

	.global ov112_021FFA20
ov112_021FFA20: ; 0x021FFA20
	.space 0x84

	.global ov112_021FFAA4
ov112_021FFAA4: ; 0x021FFAA4
	.space 0x1C

	.global ov112_021FFAC0
ov112_021FFAC0: ; 0x021FFAC0
	.space 0x30

	.global ov112_021FFAF0
ov112_021FFAF0: ; 0x021FFAF0
	.space 0x8

	.global ov112_021FFAF8
ov112_021FFAF8: ; 0x021FFAF8
	.space 0x10

	.global ov112_021FFB08
ov112_021FFB08: ; 0x021FFB08
	.space 0x18

	.global ov112_021FFB20
ov112_021FFB20: ; 0x021FFB20
	.space 0x4

	.global ov112_021FFB24
ov112_021FFB24: ; 0x021FFB24
	.space 0x40

	.global ov112_021FFB64
ov112_021FFB64: ; 0x021FFB64
	.space 0x74

	.global ov112_021FFBD8
ov112_021FFBD8: ; 0x021FFBD8
	.space 0xc8
