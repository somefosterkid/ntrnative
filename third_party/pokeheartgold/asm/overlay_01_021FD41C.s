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
	.include "overlay_01_021FD41C.inc"
	.include "global.inc"

	.public ov01_021FD47C
	.public ov01_021FD4F4
	.public ov01_021FD5CC
	.public ov01_021FD60C
	.public ov01_021FD6C8
	.public ov01_021FD714
	.public ov01_021FD718
	.public ov01_021FD784
	.public ov01_021FD7D4
	.public ov01_021FD838
	.public ov01_021FD92C
	.public ov01_021FD980
	.text

	.rodata

	.global ov01_02208E90
ov01_02208E90: ; 0x02208E90
	.word 0x1F
	.word 0x21
	.word 0x22
	.word 0x20

	.global ov01_02208EA0
ov01_02208EA0: ; 0x02208EA0
	.word 0x00000020
	.word ov01_021FD6C8
	.word ov01_021FD714
	.word ov01_021FD718
	.word ov01_021FD7D4

	.global ov01_02208EB4
ov01_02208EB4: ; 0x02208EB4
	.byte 0x00, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x20, 0x01, 0x00
	.byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00

	.global ov01_02208EC8
ov01_02208EC8: ; 0x02208EC8
	.word 0x00000020
	.word ov01_021FD784
	.word ov01_021FD714
	.word ov01_021FD718
	.word ov01_021FD838

	.global ov01_02208EDC
ov01_02208EDC: ; 0x02208EDC
	.word 0x00000020
	.word ov01_021FD6C8
	.word ov01_021FD714
	.word ov01_021FD92C
	.word ov01_021FD980

	.global ov01_02208EF0
ov01_02208EF0: ; 0x02208EF0
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00
