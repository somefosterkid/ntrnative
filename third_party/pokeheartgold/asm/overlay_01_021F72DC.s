#include "constants/sndseq.h"
#include "constants/moves.h"
#include "constants/std_script.h"
#include "constants/species.h"
#include "constants/sprites.h"
#include "constants/maps.h"
#include "constants/mmodel.h"
#include "constants/items.h"
#include "msgdata/msg/msg_0096_D31R0201.h"
#include "msgdata/msg/msg_0066_D23R0102.h"
	.include "macros.inc"
	.include "overlay_01_021F72DC.inc"
	.include "global.inc"

	.public ov01_021F796C
	.public ov01_021F79A0
	.public ov01_021F79DC
	.public ov01_021F7A18
	.public ov01_021F7A54
	.public ov01_021F7A90
	.public ov01_021F7B14
	.public ov01_021F7B90
	.public ov01_021F7C80
	.public ov01_021F7CBC
	.public ov01_021F7EE4
	.public ov01_021F7F54
	.public ov01_021F7FC8
	.public ov01_021F801C
	.public ov01_021F8070
	.public ov01_021F80C4
	.public ov01_021F8164
	.public ov01_021F81FC
	.public ov01_021F829C
	.public ov01_021F8374
	.public ov01_021F8400
	.public ov01_021F847C
	.public ov01_021F856C
	.public ov01_021F85BC
	.public ov01_021F85F4
	.public ov01_021F86B8
	.public ov01_021F8804
	.public ov01_021F883C
	.public ov01_021F8BA8
	.data

	.data

	.global ov01_022089CC
ov01_022089CC: ; 0x022089CC
	.word ov01_021F8BA8

	.global ov01_022089D0
ov01_022089D0: ; 0x022089D0
	.word ov01_021F8804
	.word ov01_021F883C

	.global ov01_022089D8
ov01_022089D8: ; 0x022089D8
	.byte 0x00, 0x30, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
	.byte 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov01_022089E8
ov01_022089E8: ; 0x022089E8
	.word ov01_021F856C
	.word ov01_021F85BC
	.word ov01_021F85F4
	.word ov01_021F86B8

	.global ov01_022089F8
ov01_022089F8:
	.global hgasset_asm_overlay_01_021F72DC_s_ov01_022089F8
	.set hgasset_asm_overlay_01_021F72DC_s_ov01_022089F8, ov01_022089F8 ; 0x022089F8
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov01_02208A08
ov01_02208A08:
	.global hgasset_asm_overlay_01_021F72DC_s_ov01_02208A08
	.set hgasset_asm_overlay_01_021F72DC_s_ov01_02208A08, ov01_02208A08 ; 0x02208A08
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov01_02208A18
ov01_02208A18:
	.global hgasset_asm_overlay_01_021F72DC_s_ov01_02208A18
	.set hgasset_asm_overlay_01_021F72DC_s_ov01_02208A18, ov01_02208A18 ; 0x02208A18
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov01_02208A28
ov01_02208A28:
	.global hgasset_asm_overlay_01_021F72DC_s_ov01_02208A28
	.set hgasset_asm_overlay_01_021F72DC_s_ov01_02208A28, ov01_02208A28 ; 0x02208A28
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov01_02208A38
ov01_02208A38:
	.global hgasset_asm_overlay_01_021F72DC_s_ov01_02208A38
	.set hgasset_asm_overlay_01_021F72DC_s_ov01_02208A38, ov01_02208A38 ; 0x02208A38
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov01_02208A48
ov01_02208A48:
	.global hgasset_asm_overlay_01_021F72DC_s_ov01_02208A48
	.set hgasset_asm_overlay_01_021F72DC_s_ov01_02208A48, ov01_02208A48 ; 0x02208A48
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov01_02208A64
ov01_02208A64:
	.global hgasset_asm_overlay_01_021F72DC_s_ov01_02208A64
	.set hgasset_asm_overlay_01_021F72DC_s_ov01_02208A64, ov01_02208A64 ; 0x02208A64
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov01_02208A80
ov01_02208A80:
	.global hgasset_asm_overlay_01_021F72DC_s_ov01_02208A80
	.set hgasset_asm_overlay_01_021F72DC_s_ov01_02208A80, ov01_02208A80 ; 0x02208A80
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov01_02208AA0
ov01_02208AA0:
	.global hgasset_asm_overlay_01_021F72DC_s_ov01_02208AA0
	.set hgasset_asm_overlay_01_021F72DC_s_ov01_02208AA0, ov01_02208AA0 ; 0x02208AA0
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov01_02208AC0
ov01_02208AC0: ; 0x02208AC0
	.word ov01_021F796C
	.word ov01_021F79A0
	.word ov01_021F79A0
	.word ov01_021F79DC
	.word ov01_021F7A18
	.word ov01_021F7A54
	.word ov01_021F7A90
	.word ov01_021F7B14
	.word ov01_021F7B90

	.global ov01_02208AE4
ov01_02208AE4: ; 0x02208AE4
	.word ov01_021F8374
	.word ov01_021F7F54
	.word ov01_021F7F54
	.word ov01_021F7FC8
	.word ov01_021F801C
	.word ov01_021F80C4
	.word ov01_021F80C4
	.word ov01_021F80C4
	.word ov01_021F8400
	.word ov01_021F847C

	.global ov01_02208B0C
ov01_02208B0C: ; 0x02208B0C
	.word ov01_021F7C80
	.word ov01_021F79A0
	.word ov01_021F79A0
	.word ov01_021F79DC
	.word ov01_021F7A18
	.word ov01_021F7A54
	.word ov01_021F7A90
	.word ov01_021F7B14
	.word ov01_021F7B90
	.word ov01_021F7CBC

	.global ov01_02208B34
ov01_02208B34: ; 0x02208B34
	.word ov01_021F7EE4
	.word ov01_021F7F54
	.word ov01_021F7F54
	.word ov01_021F7FC8
	.word ov01_021F801C
	.word ov01_021F8070
	.word ov01_021F80C4
	.word ov01_021F8164
	.word ov01_021F81FC
	.word ov01_021F829C
