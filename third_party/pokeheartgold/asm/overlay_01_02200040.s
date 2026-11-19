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
	.include "overlay_01_02200040.inc"
	.include "global.inc"

	.public ov01_02200140
	.public ov01_02200184
	.public ov01_02200190
	.public ov01_022001C0
	.public ov01_022000B4
	.text

	.rodata

	.global ov01_02209294
ov01_02209294: ; 0x02209294
	.word 0x00000024
	.word ov01_02200140
	.word ov01_02200184
	.word ov01_02200190
	.word ov01_022001C0

	.global ov01_022092A8
ov01_022092A8: ; 0x022092A8
	.byte 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
