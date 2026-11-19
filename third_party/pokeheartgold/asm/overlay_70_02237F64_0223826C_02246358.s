#include "config.h"
#include "constants/pokemon.h"
	.include "macros.inc"
	.include "overlay_70.inc"
	.include "global.inc"

	.public ov70_02238398
	.public ov70_02238FF4
	.public ov70_022390DC
	.public ov70_022390FC
	.public ov70_02239658
	.public ov70_02239698
	.public ov70_022396A8
	.public ov70_022396CC
	.public ov70_022396E0
	.public ov70_022396F4
	.public ov70_02239834
	.public ov70_02239954
	.public ov70_0223997C
	.public ov70_02239990
	.public ov70_022399D8
	.public ov70_02239A10
	.public ov70_02239AAC
	.public ov70_02239ACC
	.public ov70_02239D8C
	.public ov70_02239E68
	.public ov70_02239E7C
	.public ov70_0223A258
	.public ov70_0223A260
	.public ov70_0223A2D0
	.public ov70_0223A2F8
	.public ov70_0223A324
	.public ov70_0223A384
	.public ov70_0223A3DC
	.public ov70_0223A4A0
	.public ov70_0223A4C0
	.public ov70_0223A8BC
	.public ov70_0223AA90
	.public ov70_0223AB00
	.public ov70_0223AF5C
	.public ov70_0223AFFC
	.public ov70_0223B12C
	.public ov70_0223B188
	.public ov70_0223B1B8
	.public ov70_0223B2C0
	.public ov70_0223B2D0
	.public ov70_0223B300
	.public ov70_0223B344
	.public ov70_0223B55C
	.public ov70_0223B6EC
	.public ov70_0223B788
	.public ov70_0223BD04
	.public ov70_0223BE84
	.public ov70_0223BF00
	.public ov70_0223C008
	.public ov70_0223C0C8
	.public ov70_0223C19C
	.public ov70_0223C1F8
	.public ov70_0223C23C
	.public ov70_0223C274
	.public ov70_0223C2B0
	.public ov70_0223C4B0
	.public ov70_0223C4E4
	.public ov70_0223C50C
	.public ov70_0223C52C
	.public ov70_0223C618
	.public ov70_0223C640
	.public ov70_0223C660
	.public ov70_0223C6F0
	.public ov70_0223C718
	.public ov70_0223C744
	.public ov70_0223C7C4
	.public ov70_0223C7EC
	.public ov70_0223C818
	.public ov70_0223C8B0
	.public ov70_0223C8DC
	.public ov70_0223C928
	.public ov70_0223C930
	.public ov70_0223C958
	.public ov70_0223C978
	.public ov70_0223C9AC
	.public ov70_0223CA18
	.public ov70_0223CA40
	.public ov70_0223CD44
	.public ov70_0223CDD8
	.public ov70_0223CDF8
	.public ov70_0223D44C
	.public ov70_0223D6C8
	.public ov70_0223D934
	.public ov70_0223D998
	.public ov70_0223DB30
	.public ov70_0223DB94
	.public ov70_0223DD0C
	.public ov70_0223DD68
	.public ov70_0223DD94
	.public ov70_0223DDD0
	.public ov70_0223DDFC
	.public ov70_0223DE28
	.public ov70_0223DF14
	.public ov70_0223DF40
	.public ov70_0223DF6C
	.public ov70_0223DFD0
	.public ov70_0223DFF0
	.public ov70_0223E78C
	.public ov70_0223E884
	.public ov70_0223E8A4
	.public ov70_0223ED58
	.public ov70_0223ED8C
	.public ov70_0223EDB0
	.public ov70_0223EDCC
	.public ov70_0223EDD8
	.public ov70_0223EE10
	.public ov70_0223EEE8
	.public ov70_0223EF10
	.public ov70_0223EF38
	.public ov70_0223EFB8
	.public ov70_0223EFE0
	.public ov70_0223F008
	.public ov70_0223F0B4
	.public ov70_0223F0DC
	.public ov70_0223F108
	.public ov70_0223F164
	.public ov70_0223F1B8
	.public ov70_0223F96C
	.public ov70_0223F9B4
	.public ov70_0223F9D4
	.public ov70_0223FC68
	.public ov70_0223FD64
	.public ov70_0223FD98
	.public ov70_0223FE40
	.public ov70_0223FE60
	.public ov70_0223FEE4
	.public ov70_0223FF10
	.public ov70_0223FFCC
	.public ov70_0223FFE8
	.public ov70_0224006C
	.public ov70_022400BC
	.public ov70_02240180
	.public ov70_0224019C
	.public ov70_02240220
	.public ov70_02240240
	.public ov70_022403FC
	.public ov70_02240410
	.public ov70_02240430
	.public ov70_02240540
	.public ov70_02240598
	.public ov70_022405B4
	.public ov70_0224063C
	.public ov70_02240640
	.public ov70_02240658
	.public ov70_02240670
	.public ov70_02240684
	.public ov70_0224069C
	.public ov70_022406B8
	.public ov70_02240748
	.public ov70_022407E4
	.public ov70_0224080C
	.public ov70_02240838
	.public ov70_02240854
	.public ov70_02240884
	.public ov70_0224089C
	.public ov70_022408BC
	.public ov70_022408E0
	.public ov70_022408FC
	.public ov70_02240948
	.public ov70_022409A0
	.public ov70_022413AC
	.public ov70_02241468
	.public ov70_02241494
	.public ov70_022414A0
	.public ov70_02241648
	.public ov70_02241808
	.public ov70_02242FF4
	.public ov70_02242FFC
	.public ov70_0224304C
	.public ov70_02243068
	.public ov70_0224308C
	.public ov70_022430CC
	.public ov70_02243120
	.public ov70_0224316C
	.public ov70_02243254
	.public ov70_022433A8
	.public ov70_02243400
	.public ov70_02243420
	.public ov70_02243598
	.public ov70_02243688
	.public ov70_02243700
	.public ov70_02243750
	.public ov70_02243768
	.public ov70_022437C8
	.public ov70_02243848
	.public ov70_022438D4
	.public ov70_02243928
	.public ov70_02243A6C
	.public ov70_02243AC4
	.public ov70_02243B1C
	.public ov70_02243B2C
	.public ov70_02243C08
	.public ov70_02243C8C
	.public ov70_02243CC8
	.public ov70_02243CEC
	.public ov70_02243D28
	.public ov70_02243D90
	.public ov70_02243DA0
	.public ov70_02243DDC
	.public ov70_02243E74
	.public ov70_02244038
	.public ov70_02244124
	.public ov70_02244154
	.public ov70_022446D0
	.public ov70_02244708
	.public ov70_02244774
	.public ov70_022447A0
	.public ov70_02244804
	.public ov70_02244834
	.public ov70_02244854
	.public ov70_02244888
	.public ov70_022448C0
	.public ov70_02244944
	.public ov70_02244954
	.public ov70_02244A04
	.public ov70_02244A4C
	.public ov70_02244A68
	.public ov70_02244B20
	.public ov70_02244B54
	.public ov70_02244CA8
	.public ov70_02244CCC
	.public ov70_02244D08
	.public ov70_02244D44
	.public ov70_02244D80
	.public ov70_02244E44
	.public ov70_02244F14
	.public ov70_02244F34
	.public ov70_02244F68




	.data
	.balign 4, 0
	.global ov70_02246358
ov70_02246358: ; 0x02246358
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/info.asp"

	.balign 4, 0
	.global ov70_022463A4
ov70_022463A4: ; 0x022463A4
	.asciz "http://gamestats2.gs.nintendowifi.net/pokemondpds/common/setProfile.asp"

	.balign 4, 0
	.global ov70_022463EC
ov70_022463EC: ; 0x022463EC
	.word ov70_02244038, ov70_02244124, ov70_02244154
	.word ov70_02238FF4, ov70_022390DC, ov70_022390FC
	.word ov70_02239D8C, ov70_02239E68, ov70_02239E7C
	.word ov70_0223A8BC, ov70_0223AA90, ov70_0223AB00
	.word ov70_0223B55C, ov70_0223B6EC, ov70_0223B788
	.word ov70_0223CD44, ov70_0223CDD8, ov70_0223CDF8
	.word ov70_0223E78C, ov70_0223E884, ov70_0223E8A4
	.word ov70_0223F96C, ov70_0223F9B4, ov70_0223F9D4
	.word ov70_022413AC, ov70_02241468, ov70_02241494
	.word ov70_022414A0, ov70_02241648, ov70_02241808

	.global ov70_02246464
ov70_02246464: ; 0x02246464
	.word ov70_02239658
	.word ov70_02239698
	.word ov70_022396A8
	.word ov70_022396CC
	.word ov70_022396E0
	.word ov70_022396F4
	.word ov70_02239834
	.word ov70_02239954
	.word ov70_0223997C
	.word ov70_02239990
	.word ov70_02239AAC
	.word ov70_02239ACC
	.word ov70_022399D8
	.word ov70_02239A10

	.global ov70_0224649C
ov70_0224649C: ; 0x0224649C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x06, 0x05, 0x00, 0x00, 0x04, 0x03, 0x00

	.global ov70_022464A8
ov70_022464A8: ; 0x022464A8
	.word ov70_0223A258
	.word ov70_0223A260
	.word ov70_0223A2D0
	.word ov70_0223A4A0
	.word ov70_0223A4C0
	.word ov70_0223A2F8
	.word ov70_0223A324
	.word ov70_0223A384
	.word ov70_0223A3DC

	.global ov70_022464CC
ov70_022464CC: ; 0x022464CC
	.word ov70_0223AF5C
	.word ov70_0223AFFC
	.word ov70_0223B12C
	.word ov70_0223B344
	.word ov70_0223B188
	.word ov70_0223B1B8
	.word ov70_0223B2C0
	.word ov70_0223B2D0
	.word ov70_0223B300

	.global ov70_022464F0
ov70_022464F0: ; 0x022464F0
	.byte 0x00

	.global ov70_022464F1
ov70_022464F1:
	.global hgasset_asm_overlay_70_02237F64_0223826C_02246358_s_ov70_022464F1
	.set hgasset_asm_overlay_70_02237F64_0223826C_02246358_s_ov70_022464F1, ov70_022464F1 ; 0x022464F1
	.byte 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0, 0, 0

	.global ov70_022464FE
ov70_022464FE: ; 0x022464FE
	.byte 0x00, 0x00

	.global ov70_02246500
ov70_02246500: ; 0x02246500
	.byte 0x00, 0x00

	.global ov70_02246502
ov70_02246502:
	.global hgasset_asm_overlay_70_02237F64_0223826C_02246358_s_ov70_02246502
	.set hgasset_asm_overlay_70_02237F64_0223826C_02246358_s_ov70_02246502, ov70_02246502 ; 0x02246502
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00
	.byte 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00

	.global ov70_02246528
ov70_02246528: ; 0x02246528
	.word ov70_0223BD04
	.word ov70_0223BE84
	.word ov70_0223C4B0
	.word ov70_0223C4E4
	.word ov70_0223C50C
	.word ov70_0223C52C
	.word ov70_0223C618
	.word ov70_0223C640
	.word ov70_0223C660
	.word ov70_0223C6F0
	.word ov70_0223C718
	.word ov70_0223C744
	.word ov70_0223C7C4
	.word ov70_0223C7EC
	.word ov70_0223C818
	.word ov70_0223BF00
	.word ov70_0223C008
	.word ov70_0223C0C8
	.word ov70_0223C19C
	.word ov70_0223C1F8
	.word ov70_0223C23C
	.word ov70_0223C958
	.word ov70_0223C978
	.word ov70_0223C8B0
	.word ov70_0223C8DC
	.word ov70_0223C928
	.word ov70_0223C930
	.word ov70_0223C274
	.word ov70_0223C2B0
	.word ov70_0223C9AC
	.word ov70_0223CA18
	.word ov70_0223CA40

	.global ov70_022465A8
ov70_022465A8: ; 0x022465A8
	.word ov70_0223D44C
	.word ov70_0223D6C8
	.word ov70_0223DD0C
	.word ov70_0223DFD0
	.word ov70_0223DFF0
	.word ov70_0223DD68
	.word ov70_0223DD94
	.word ov70_0223D934
	.word ov70_0223D998
	.word ov70_0223DB30
	.word ov70_0223DB94
	.word ov70_0223DDD0
	.word ov70_0223DDFC
	.word ov70_0223DE28
	.word ov70_0223DF14
	.word ov70_0223DF40
	.word ov70_0223DF6C

	.global ov70_022465EC
ov70_022465EC: ; 0x022465EC
	.byte 0x00, 0x06, 0x05, 0x00
	.byte 0x00, 0x04, 0x03, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00
	.byte 0xFE, 0xFF, 0xFF, 0xFF

	.global ov70_02246614
ov70_02246614: ; 0x02246614
	.word ov70_0223ED58
	.word ov70_0223ED8C
	.word ov70_0223F164
	.word ov70_0223EDB0
	.word ov70_0223EDCC
	.word ov70_0223EDD8
	.word ov70_0223EE10
	.word ov70_0223EEE8
	.word ov70_0223EF10
	.word ov70_0223EF38
	.word ov70_0223EFB8
	.word ov70_0223EFE0
	.word ov70_0223F008
	.word ov70_0223F0B4
	.word ov70_0223F0DC
	.word ov70_0223F108
	.word ov70_0223F1B8

	.global ov70_02246658
ov70_02246658: ; 0x02246658
	.word ov70_0223FC68
	.word ov70_0224063C
	.word ov70_0223FD64
	.word ov70_0223FD98
	.word ov70_0223FE40
	.word ov70_0223FE60
	.word ov70_02240640
	.word ov70_0223FEE4
	.word ov70_0223FF10
	.word ov70_0223FFCC
	.word ov70_0223FFE8
	.word ov70_02240658
	.word ov70_0224006C
	.word ov70_022400BC
	.word ov70_02240180
	.word ov70_0224019C
	.word ov70_02240670
	.word ov70_02240748
	.word ov70_02240540
	.word ov70_02240598
	.word ov70_022405B4
	.word ov70_02240684
	.word ov70_0224069C
	.word ov70_022406B8
	.word ov70_02240220
	.word ov70_02240240
	.word ov70_02240410
	.word ov70_02240430
	.word ov70_022403FC
	.word ov70_02240838
	.word ov70_02240854
	.word ov70_02240884
	.word ov70_0224089C
	.word ov70_022408BC
	.word ov70_022408E0
	.word ov70_022408FC
	.word ov70_02240948
	.word ov70_022409A0
	.word ov70_022407E4
	.word ov70_0224080C

	.global ov70_022466F8
ov70_022466F8: ; 0x022466F8
	.word ov70_02242FF4
	.word ov70_02242FFC
	.word ov70_0224304C
	.word ov70_02243068
	.word ov70_0224308C
	.word ov70_022430CC
	.word ov70_02243120
	.word ov70_0224316C
	.word ov70_02243254
	.word ov70_022433A8
	.word ov70_02243400
	.word ov70_02243420
	.word ov70_02243598
	.word ov70_02243688
	.word ov70_02243700
	.word ov70_02243750
	.word ov70_02243768
	.word ov70_022437C8
	.word ov70_02243848
	.word ov70_022438D4
	.word ov70_02243928
	.word ov70_02243A6C
	.word ov70_02243AC4
	.word ov70_02243B1C
	.word ov70_02243B2C
	.word ov70_02243C08
	.word ov70_02243C8C
	.word ov70_02243CC8
	.word ov70_02243CEC
	.word ov70_02243D28
	.word ov70_02243D90
	.word ov70_02243DA0
	.word ov70_02243DDC
	.word ov70_02243E74

	.global ov70_02246780
ov70_02246780: ; 0x02246780
	.word ov70_022446D0
	.word ov70_02244708
	.word ov70_02244888
	.word ov70_022448C0
	.word ov70_02244944
	.word ov70_02244954
	.word ov70_02244A04
	.word ov70_02244A4C
	.word ov70_02244A68
	.word ov70_02244B20
	.word ov70_02244B54
	.word ov70_02244D08
	.word ov70_02244F14
	.word ov70_02244F68
	.word ov70_02244D44
	.word ov70_02244774
	.word ov70_022447A0
	.word ov70_02244804
	.word ov70_02244834
	.word ov70_02244854
	.word ov70_02244F34
	.word ov70_02244D80
	.word ov70_02244E44
	.word ov70_02244CA8
	.word ov70_02244CCC
