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
	.global ov70_02245240
ov70_02245240:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245240
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245240, ov70_02245240 ; 0x02245240
	.byte 0, 0x00

	.global ov70_02245242
ov70_02245242:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245242
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245242, ov70_02245242 ; 0x02245242
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00

	.global ov70_0224524C
ov70_0224524C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224524C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224524C, ov70_0224524C ; 0x0224524C
	.byte 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov70_0224525C
ov70_0224525C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224525C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224525C, ov70_0224525C ; 0x0224525C
	.byte 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_0224526C
ov70_0224526C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224526C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224526C, ov70_0224526C ; 0x0224526C
	.byte 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00

	.global ov70_02245284
ov70_02245284:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245284
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245284, ov70_02245284 ; 0x02245284
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_022452A0
ov70_022452A0:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022452A0
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022452A0, ov70_022452A0 ; 0x022452A0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_022452BC
ov70_022452BC:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022452BC
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022452BC, ov70_022452BC ; 0x022452BC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_022452D8
ov70_022452D8:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022452D8
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022452D8, ov70_022452D8 ; 0x022452D8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov70_022452F4
ov70_022452F4:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022452F4
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022452F4, ov70_022452F4 ; 0x022452F4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245310
ov70_02245310:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245310
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245310, ov70_02245310 ; 0x02245310
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_0224532C
ov70_0224532C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224532C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224532C, ov70_0224532C ; 0x0224532C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245348
ov70_02245348:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245348
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245348, ov70_02245348 ; 0x02245348
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov70_02245364
ov70_02245364:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245364
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245364, ov70_02245364 ; 0x02245364
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245380
ov70_02245380:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245380
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245380, ov70_02245380 ; 0x02245380
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_0224539C
ov70_0224539C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224539C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224539C, ov70_0224539C ; 0x0224539C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_022453B8
ov70_022453B8:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022453B8
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022453B8, ov70_022453B8 ; 0x022453B8
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov70_02245498
ov70_02245498:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245498
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245498, ov70_02245498 ; 0x02245498
	.byte 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00

	.global ov70_022454A4
ov70_022454A4:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022454A4
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022454A4, ov70_022454A4 ; 0x022454A4
	.byte 0

	.global ov70_022454A5
ov70_022454A5:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022454A5
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022454A5, ov70_022454A5 ; 0x022454A5
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00

	.global ov70_022454B4
ov70_022454B4:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022454B4
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022454B4, ov70_022454B4 ; 0x022454B4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_022454D0
ov70_022454D0:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022454D0
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022454D0, ov70_022454D0 ; 0x022454D0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_022454EC
ov70_022454EC:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022454EC
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022454EC, ov70_022454EC ; 0x022454EC
	.byte 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov70_0224562C
ov70_0224562C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224562C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224562C, ov70_0224562C ; 0x0224562C
	.byte 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00

	.global ov70_02245634
ov70_02245634:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245634
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245634, ov70_02245634 ; 0x02245634
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00

	.global ov70_02245640
ov70_02245640:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245640
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245640, ov70_02245640 ; 0x02245640
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00

	.global ov70_02245658
ov70_02245658:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245658
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245658, ov70_02245658 ; 0x02245658
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov70_02245674
ov70_02245674:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245674
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245674, ov70_02245674 ; 0x02245674
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245690
ov70_02245690:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245690
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245690, ov70_02245690 ; 0x02245690
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_022456AC
ov70_022456AC:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022456AC
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022456AC, ov70_022456AC ; 0x022456AC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_022456C8
ov70_022456C8:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022456C8
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022456C8, ov70_022456C8 ; 0x022456C8
	.byte 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov70_022456E8
ov70_022456E8:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022456E8
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022456E8, ov70_022456E8 ; 0x022456E8
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00

	.global ov70_022456F0
ov70_022456F0:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022456F0
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022456F0, ov70_022456F0 ; 0x022456F0
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245700
ov70_02245700:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245700
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245700, ov70_02245700 ; 0x02245700
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00

	.global ov70_02245714
ov70_02245714:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245714
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245714, ov70_02245714 ; 0x02245714
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245730
ov70_02245730:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245730
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245730, ov70_02245730 ; 0x02245730
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_0224574C
ov70_0224574C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224574C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224574C, ov70_0224574C ; 0x0224574C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245768
ov70_02245768:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245768
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245768, ov70_02245768 ; 0x02245768
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov70_02245784
ov70_02245784:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245784
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245784, ov70_02245784 ; 0x02245784
	.byte 0, 0x00

	.global ov70_02245786
ov70_02245786:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245786
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245786, ov70_02245786 ; 0x02245786
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00

	.global ov70_02245804
ov70_02245804:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245804
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245804, ov70_02245804 ; 0x02245804
	.byte 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0

	.global ov70_02245884
ov70_02245884:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245884
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245884, ov70_02245884 ; 0x02245884
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov70_0224590C
ov70_0224590C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224590C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224590C, ov70_0224590C ; 0x0224590C
	.byte 0, 0x00, 0x00, 0x00

	.global ov70_02245910
ov70_02245910:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245910
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245910, ov70_02245910 ; 0x02245910
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov70_02245920
ov70_02245920:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245920
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245920, ov70_02245920 ; 0x02245920
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00

	.global ov70_02245938
ov70_02245938:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245938
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245938, ov70_02245938 ; 0x02245938
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov70_02245954
ov70_02245954:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245954
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245954, ov70_02245954 ; 0x02245954
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245970
ov70_02245970:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245970
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245970, ov70_02245970 ; 0x02245970
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_0224598C
ov70_0224598C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224598C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224598C, ov70_0224598C ; 0x0224598C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0, 0x00, 0
	.byte 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_022459C8
ov70_022459C8:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022459C8
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022459C8, ov70_022459C8 ; 0x022459C8
	.byte 0x00, 0x00, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0

	.global ov70_022459F4
ov70_022459F4:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022459F4
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_022459F4, ov70_022459F4 ; 0x022459F4
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00

	.global ov70_02245A4C
ov70_02245A4C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245A4C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245A4C, ov70_02245A4C ; 0x02245A4C
	.byte 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00

	.global ov70_02245AAC
ov70_02245AAC:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245AAC
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245AAC, ov70_02245AAC ; 0x02245AAC
	.byte 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov70_02245B5C
ov70_02245B5C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245B5C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245B5C, ov70_02245B5C ; 0x02245B5C
	.byte 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00

	.global ov70_02245C60
ov70_02245C60:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245C60
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245C60, ov70_02245C60 ; 0x02245C60
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245C70
ov70_02245C70:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245C70
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245C70, ov70_02245C70 ; 0x02245C70
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0, 0, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245C8C
ov70_02245C8C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245C8C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245C8C, ov70_02245C8C ; 0x02245C8C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245CA8
ov70_02245CA8:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245CA8
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245CA8, ov70_02245CA8 ; 0x02245CA8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov70_02245CC4
ov70_02245CC4:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245CC4
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245CC4, ov70_02245CC4 ; 0x02245CC4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245CE0
ov70_02245CE0:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245CE0
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245CE0, ov70_02245CE0 ; 0x02245CE0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245CFC
ov70_02245CFC:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245CFC
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245CFC, ov70_02245CFC ; 0x02245CFC
	.byte 0x00, 0, 0x00, 0
	.byte 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0

	.global ov70_02245D0A
ov70_02245D0A:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D0A
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D0A, ov70_02245D0A ; 0x02245D0A
	.byte 0, 0x00

	.global ov70_02245D0C
ov70_02245D0C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D0C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D0C, ov70_02245D0C ; 0x02245D0C
	.byte 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00

	.global ov70_02245D26
ov70_02245D26:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D26
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D26, ov70_02245D26 ; 0x02245D26
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov70_02245D48
ov70_02245D48:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D48
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D48, ov70_02245D48 ; 0x02245D48
	.byte 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov70_02245D50
ov70_02245D50: ; 0x02245D50
	.word TradeSequence_Init, TradeSequence_Main, TradeSequence_Exit, SDK_OVERLAY_OVY_71_ID

	.global ov70_02245D60
ov70_02245D60:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D60
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D60, ov70_02245D60 ; 0x02245D60
	.byte 0, 0, 0, 0, 0, 0

	.global ov70_02245D66
ov70_02245D66:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D66
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D66, ov70_02245D66 ; 0x02245D66
	.byte 0

	.global ov70_02245D67
ov70_02245D67:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D67
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D67, ov70_02245D67 ; 0x02245D67
	.byte 0, 0, 0, 0, 0, 0, 0

	.global ov70_02245D6E
ov70_02245D6E:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D6E
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D6E, ov70_02245D6E ; 0x02245D6E
	.byte 0, 0
	.byte 0, 0, 0, 0, 0, 0

	.global ov70_02245D76
ov70_02245D76:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D76
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D76, ov70_02245D76 ; 0x02245D76
	.byte 0

	.global ov70_02245D77
ov70_02245D77:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D77
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D77, ov70_02245D77 ; 0x02245D77
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0

	.global ov70_02245D80
ov70_02245D80:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D80
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D80, ov70_02245D80 ; 0x02245D80
	.byte 0

	.global ov70_02245D81
ov70_02245D81:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D81
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D81, ov70_02245D81 ; 0x02245D81
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0

	.global ov70_02245D8A
ov70_02245D8A:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D8A
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D8A, ov70_02245D8A ; 0x02245D8A
	.byte 0

	.global ov70_02245D8B
ov70_02245D8B:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D8B
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D8B, ov70_02245D8B ; 0x02245D8B
	.byte 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0

	.global ov70_02245D96
ov70_02245D96:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D96
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245D96, ov70_02245D96 ; 0x02245D96
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0

	.global ov70_02245DA2
ov70_02245DA2:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DA2
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DA2, ov70_02245DA2 ; 0x02245DA2
	.byte 0, 0, 0x00, 0, 0, 0x00, 0, 0, 0x00, 0, 0, 0x00, 0x00, 0x00

	.global ov70_02245DB0
ov70_02245DB0:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DB0
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DB0, ov70_02245DB0 ; 0x02245DB0
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0

	.global ov70_02245DC0
ov70_02245DC0:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DC0
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DC0, ov70_02245DC0 ; 0x02245DC0
	.byte 0

	.global ov70_02245DC1
ov70_02245DC1:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DC1
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DC1, ov70_02245DC1 ; 0x02245DC1
	.byte 0

	.global ov70_02245DC2
ov70_02245DC2:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DC2
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DC2, ov70_02245DC2 ; 0x02245DC2
	.byte 0

	.global ov70_02245DC3
ov70_02245DC3:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DC3
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DC3, ov70_02245DC3 ; 0x02245DC3
	.byte 0, 0, 0, 0x00, 0, 0, 0, 0, 0x00, 0, 0, 0, 0

	.global ov70_02245DD0
ov70_02245DD0:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DD0
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DD0, ov70_02245DD0 ; 0x02245DD0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00

	.global ov70_02245DE4
ov70_02245DE4:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DE4
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DE4, ov70_02245DE4 ; 0x02245DE4
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov70_02245DF8
ov70_02245DF8:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DF8
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DF8, ov70_02245DF8 ; 0x02245DF8
	.byte 0

	.global ov70_02245DF9
ov70_02245DF9:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DF9
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245DF9, ov70_02245DF9 ; 0x02245DF9
	.byte 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	.global ov70_02245E0E
ov70_02245E0E:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E0E
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E0E, ov70_02245E0E ; 0x02245E0E
	.byte 0, 0

	.global ov70_02245E10
ov70_02245E10:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E10
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E10, ov70_02245E10 ; 0x02245E10
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0

	.global ov70_02245E26
ov70_02245E26:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E26
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E26, ov70_02245E26 ; 0x02245E26
	.byte 0

	.global ov70_02245E27
ov70_02245E27:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E27
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E27, ov70_02245E27 ; 0x02245E27
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	.global ov70_02245E3E
ov70_02245E3E:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E3E
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E3E, ov70_02245E3E ; 0x02245E3E
	.byte 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov70_02245E5E
ov70_02245E5E:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E5E
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E5E, ov70_02245E5E ; 0x02245E5E
	.byte 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov70_02245E84
ov70_02245E84:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E84
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245E84, ov70_02245E84 ; 0x02245E84
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov70_02245EA8
ov70_02245EA8:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EA8
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EA8, ov70_02245EA8 ; 0x02245EA8
	.byte 0

	.global ov70_02245EA9
ov70_02245EA9:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EA9
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EA9, ov70_02245EA9 ; 0x02245EA9
	.byte 0

	.global ov70_02245EAA
ov70_02245EAA:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EAA
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EAA, ov70_02245EAA ; 0x02245EAA
	.byte 0

	.global ov70_02245EAB
ov70_02245EAB:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EAB
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EAB, ov70_02245EAB ; 0x02245EAB
	.byte 0, 0, 0, 0x00, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	.global ov70_02245ED0
ov70_02245ED0:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245ED0
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245ED0, ov70_02245ED0 ; 0x02245ED0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov70_02245EFC
ov70_02245EFC:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EFC
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EFC, ov70_02245EFC ; 0x02245EFC
	.byte 0

	.global ov70_02245EFD
ov70_02245EFD:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EFD
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EFD, ov70_02245EFD ; 0x02245EFD
	.byte 0

	.global ov70_02245EFE
ov70_02245EFE:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EFE
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EFE, ov70_02245EFE ; 0x02245EFE
	.byte 0

	.global ov70_02245EFF
ov70_02245EFF:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EFF
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245EFF, ov70_02245EFF ; 0x02245EFF
	.byte 0
	.byte 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0

	.global ov70_02245F28
ov70_02245F28:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245F28
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245F28, ov70_02245F28 ; 0x02245F28
	.byte 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov70_02245F58
ov70_02245F58: ; 0x02245F58
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov70_02245F5C
ov70_02245F5C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245F5C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245F5C, ov70_02245F5C ; 0x02245F5C
	.byte 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov70_02245FA0
ov70_02245FA0:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245FA0
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_02245FA0, ov70_02245FA0 ; 0x02245FA0
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov70_0224600C
ov70_0224600C:
	.global hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224600C
	.set hgasset_asm_overlay_70_02238430_022387AC_02245240_s_ov70_0224600C, ov70_0224600C ; 0x0224600C
	.byte 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
