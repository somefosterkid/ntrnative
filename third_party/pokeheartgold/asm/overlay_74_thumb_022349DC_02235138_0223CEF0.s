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
	.public gApp_MainMenu_SelectOption_MigrateFromAgb
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
	.public gApp_MainMenu_SelectOption_MigrateFromAgb


	.data
	.balign 4, 0
ov74_0223CEF0:
	.asciz "mywh_SYSSTATE_IDLE"
	.balign 4, 0
ov74_0223CF04:
	.asciz "mywh_SYSSTATE_BUSY"
	.balign 4, 0
ov74_0223CF18:
	.asciz "mywh_SYSSTATE_STOP"
	.balign 4, 0
ov74_0223CF2C:
	.asciz "mywh_SYSSTATE_ERROR"
	.balign 4, 0
ov74_0223CF40:
	.asciz "mywh_SYSSTATE_SCANNING"
	.balign 4, 0
ov74_0223CF58:
	.asciz "mywh_SYSSTATE_CONNECTED"
	.balign 4, 0
ov74_0223CF70:
	.asciz "mywh_SYSSTATE_KEYSHARING"
	.balign 4, 0
ov74_0223CF8C:
	.asciz "mywh_SYSSTATE_DATASHARING"
	.balign 4, 0
ov74_0223CFA8:
	.asciz "mywh_SYSSTATE_CONNECT_FAIL"
	.balign 4, 0
ov74_0223CFC4:
	.asciz "mywh_SYSSTATE_MEASURECHANNEL"
	.balign 4, 0

	.global ov74_0223CFE4
ov74_0223CFE4: ; 0x0223CFE4
	.word ov74_0223CF18
	.word ov74_0223CEF0
	.word ov74_0223CF40
	.word ov74_0223CF04
	.word ov74_0223CF58
	.word ov74_0223CF8C
	.word ov74_0223CF70
	.word ov74_0223CFC4
	.word ov74_0223CFA8
	.word ov74_0223CF2C

	.global ov74_0223D00C
ov74_0223D00C: ; 0x0223D00C
	.asciz "%s -> "
	.balign 4, 0

	.global ov74_0223D014
ov74_0223D014: ; 0x0223D014
	.asciz "%s\n"
	.balign 4, 0

	.global ov74_0223D018
ov74_0223D018: ; 0x0223D018
	.asciz "not my parent ggid (%d != %d)\n"
	.balign 4, 0

	.global ov74_0223D038
ov74_0223D038: ; 0x0223D038
	.asciz "IPGE"
	.balign 4, 0

	.global ov74_0223D040
ov74_0223D040: ; 0x0223D040
	.asciz "K5WfPEw9T3cvqzHa"
	.balign 4, 0

	.global ov74_0223D054
ov74_0223D054: ; 0x0223D054
	.byte 0x00, 0x00, 0x00, 0x00
