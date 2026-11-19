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
	.include "overlay_01_021F4704.inc"
	.include "global.inc"

	.public ov01_021F5A28
	.public ov01_021F5D8C
	.public ov01_021F5E20
	.public ov01_021F5EA0
	.public ov01_021F6328
	.public ov01_021F6614
	.public ov01_021F676C
	.public ov01_021F67B4
	.public ov01_021F4D10
	.public ov01_021F4D88
	.public ov01_021F4E1C
	.text

	.rodata

	.global ov01_02206BC0
ov01_02206BC0: ; 0x02206BC0
	.word ov01_021F5D8C
	.word ov01_021F4D10

	.global ov01_02206BC8
ov01_02206BC8: ; 0x02206BC8
	.word ov01_021F5EA0
	.word ov01_021F4E1C

	.global ov01_02206BD0
ov01_02206BD0: ; 0x02206BD0
	.word ov01_021F5E20
	.word ov01_021F4D88

	.global ov01_02206BD8
ov01_02206BD8: ; 0x02206BD8
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00

	.global ov01_02206BE4
ov01_02206BE4: ; 0x02206BE4
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
