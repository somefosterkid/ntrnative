#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"
	.public gApplication_TitleScreen
	.public sub_020342C0

	.public MainMenuApp_Init
	.public MainMenuApp_Main
	.public MainMenuApp_Exit
	.public ov74_0223B2C0
	.public ov74_0223B2CC
	.public ov74_0223B2D8
	.public ov74_0223B2E8
	.public gApp_MainMenu_SelectOption_ConnectToRanger
	.public gApp_MainMenu_SelectOption_MysteryGift

	.public ov74_02229200
	.public ov74_02229294
	.public ov74_02229450
	.public ov74_02229DA0
	.public ov74_02229DCC
	.public ov74_02229DD4
	.public ov74_02229DE0
	.public ov74_02229F90
	.public ov74_02229FF4
	.public ov74_0222A2A4
	.public ov74_0222A2E0
	.public ov74_0222A35C
	.public ov74_0222A3CC
	.public ov74_0222A494
	.public ov74_0222A4CC
	.public ov74_0222A504
	.public ov74_0222A538
	.public ov74_0222A560
	.public ov74_0222A5AC
	.public ov74_0222A5CC
	.public ov74_0222A650
	.public ov74_0222A688
	.public ov74_0222A6C0
	.public ov74_0222AE6C
	.public ov74_0222C2EC
	.public ov74_0222CD94
	.public ov74_0222D2D4
	.public ov74_0222D308
	.public ov74_0222D358
	.public ov74_0222D414
	.public ov74_0222D490
	.public ov74_0222D494
	.public ov74_0222D4E4
	.public ov74_0222D55C
	.public ov74_0222D5C4
	.public ov74_0222D614
	.public ov74_0222D690
	.public ov74_0222D774
	.public ov74_0222D78C
	.public ov74_0222D7D0
	.public ov74_0222DDFC
	.public ov74_0222E1F4
	.public ov74_0222EB44
	.public ov74_0222EE08
	.public ov74_0222EE0C
	.public ov74_0222EE58
	.public ov74_0222EEB0
	.public ov74_0222EF18
	.public ov74_0222EF64
	.public ov74_0222EF68
	.public ov74_0222F598
	.public ov74_0222F7D4
	.public ov74_0222FB8C
	.public ov74_02233230
	.public ov74_022332F4
	.public ov74_022338D4
	.public ov74_02235FF8








	.data
	.balign 4, 0
	.global _0223B4F8
_0223B4F8:
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _0223B508
_0223B508:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.global _0223B524
_0223B524:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _0223B540
_0223B540:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1E, 0x02, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _0223B55C
_0223B55C:
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x02
	.byte 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.global _0223B578
_0223B578:
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00
	.global sPmAgbRomCodeMappings
sPmAgbRomCodeMappings:
	; Ruby
	.balign 4, 0
	.ascii "AXVJ"
	.byte 0x00, 0x00
	.short LANGUAGE_JAPANESE
	.balign 4, 0
	.ascii "AXVE"
	.byte 0x00, 0x00
	.short LANGUAGE_ENGLISH
	.balign 4, 0
	.ascii "AXVF"
	.byte 0x00, 0x00
	.short LANGUAGE_FRENCH
	.balign 4, 0
	.ascii "AXVD"
	.byte 0x00, 0x00
	.short LANGUAGE_GERMAN
	.balign 4, 0
	.ascii "AXVS"
	.byte 0x00, 0x00
	.short LANGUAGE_SPANISH
	.balign 4, 0
	.ascii "AXVI"
	.byte 0x00, 0x00
	.short LANGUAGE_ITALIAN

	; Sapphire
	.balign 4, 0
	.ascii "AXPJ"
	.byte 0x00, 0x01
	.short LANGUAGE_JAPANESE
	.balign 4, 0
	.ascii "AXPE"
	.byte 0x00, 0x01
	.short LANGUAGE_ENGLISH
	.balign 4, 0
	.ascii "AXPF"
	.byte 0x00, 0x01
	.short LANGUAGE_FRENCH
	.balign 4, 0
	.ascii "AXPD"
	.byte 0x00, 0x01
	.short LANGUAGE_GERMAN
	.balign 4, 0
	.ascii "AXPS"
	.byte 0x00, 0x01
	.short LANGUAGE_SPANISH
	.balign 4, 0
	.ascii "AXPI"
	.byte 0x00, 0x01
	.short LANGUAGE_ITALIAN

	; FireRed
	.balign 4, 0
	.ascii "BPRJ"
	.byte 0x01, 0x03
	.short LANGUAGE_JAPANESE
	.balign 4, 0
	.ascii "BPRE"
	.byte 0x01, 0x03
	.short LANGUAGE_ENGLISH
	.balign 4, 0
	.ascii "BPRF"
	.byte 0x01, 0x03
	.short LANGUAGE_FRENCH
	.balign 4, 0
	.ascii "BPRD"
	.byte 0x01, 0x03
	.short LANGUAGE_GERMAN
	.balign 4, 0
	.ascii "BPRS"
	.byte 0x01, 0x03
	.short LANGUAGE_SPANISH
	.balign 4, 0
	.ascii "BPRI"
	.byte 0x01, 0x03
	.short LANGUAGE_ITALIAN

	; LeafGreen
	.balign 4, 0
	.ascii "BPGJ"
	.byte 0x01, 0x02
	.short LANGUAGE_JAPANESE
	.balign 4, 0
	.ascii "BPGE"
	.byte 0x01, 0x02
	.short LANGUAGE_ENGLISH
	.balign 4, 0
	.ascii "BPGF"
	.byte 0x01, 0x02
	.short LANGUAGE_FRENCH
	.balign 4, 0
	.ascii "BPGD"
	.byte 0x01, 0x02
	.short LANGUAGE_GERMAN
	.balign 4, 0
	.ascii "BPGS"
	.byte 0x01, 0x02
	.short LANGUAGE_SPANISH
	.balign 4, 0
	.ascii "BPGI"
	.byte 0x01, 0x02
	.short LANGUAGE_ITALIAN

	; Emerald
	.balign 4, 0
	.ascii "BPEJ"
	.byte 0x02, 0x04
	.short LANGUAGE_JAPANESE
	.balign 4, 0
	.ascii "BPEE"
	.byte 0x02, 0x04
	.short LANGUAGE_ENGLISH
	.balign 4, 0
	.ascii "BPEF"
	.byte 0x02, 0x04
	.short LANGUAGE_FRENCH
	.balign 4, 0
	.ascii "BPED"
	.byte 0x02, 0x04
	.short LANGUAGE_GERMAN
	.balign 4, 0
	.ascii "BPES"
	.byte 0x02, 0x04
	.short LANGUAGE_SPANISH
	.balign 4, 0
	.ascii "BPEI"
	.byte 0x02, 0x04
	.short LANGUAGE_ITALIAN
	.balign 4, 0
	.global _0223B690
_0223B690:
	.global hgasset_asm_overlay_74_thumb_02231A1C_0223B4F8_s__0223B690
	.set hgasset_asm_overlay_74_thumb_02231A1C_0223B4F8_s__0223B690, _0223B690
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.global _0223B710
_0223B710:
	.global hgasset_asm_overlay_74_thumb_02231A1C_0223B4F8_s__0223B710
	.set hgasset_asm_overlay_74_thumb_02231A1C_0223B4F8_s__0223B710, _0223B710
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
